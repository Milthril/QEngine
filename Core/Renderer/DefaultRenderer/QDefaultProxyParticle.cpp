#include "QDateTime"
#include "QDefaultProxyParticle.h"
#include "QEngine.h"
#include "Scene\Component\Particle\QParticleEmitter.h"
#include "Scene\Component\Particle\QParticleComponent.h"
#include "QDefaultProxyStaticMesh.h"

QDefaultProxyParticle::QDefaultProxyParticle(std::shared_ptr<QParticleComponent> particle)
	:mParticle(particle)
{
}

void QDefaultProxyParticle::recreateResource() {
	mStaticMeshProxy = std::make_shared<QDefaultProxyStaticMesh>(mParticle->getStaticMesh());
	mStaticMeshProxy->setParentParticle(mParticle);
	mStaticMeshProxy->mRenderer = mRenderer;
	mStaticMeshProxy->recreateResource();
	mParticlesBuffer[0].reset(RHI->newBuffer(QRhiBuffer::Static, QRhiBuffer::UsageFlag::VertexBuffer | QRhiBuffer::UsageFlag::StorageBuffer, sizeof(QParticleSystem::ParticleBuffer)));
	mParticlesBuffer[0]->create();
	mParticlesBuffer[1].reset(RHI->newBuffer(QRhiBuffer::Static, QRhiBuffer::UsageFlag::VertexBuffer | QRhiBuffer::UsageFlag::StorageBuffer, sizeof(QParticleSystem::ParticleBuffer)));
	mParticlesBuffer[1]->create();

	mParticleCounterBuffer.reset(RHI->newBuffer(QRhiBuffer::Static, QRhiBuffer::UsageFlag::StorageBuffer, sizeof(ParticleRunContext)));
	mParticleCounterBuffer->create();

	mMatrixBuffer.reset(RHI->newBuffer(QRhiBuffer::Static, QRhiBuffer::UsageFlag::VertexBuffer | QRhiBuffer::UsageFlag::StorageBuffer, sizeof(float) * 16 * QParticleSystem::PARTICLE_MAX_SIZE));
	mMatrixBuffer->create();
}

void QDefaultProxyParticle::recreatePipeline()
{
	QRhiUniformProxy::UniformInfo uniformInfo = mParticle->getParticleSystem()->getUpdater()->getProxy()->getUniformInfo(3, QRhiShaderResourceBinding::ComputeStage);

	QString particleRunCode = QString(R"(#version 450
	%1
	layout(std140,binding = 0) buffer InputParticlesBuffer{
		Particle inputParticles[PARTICLE_MAX_SIZE];
	};
	layout(std140,binding = 1) buffer OutputParticlesBuffer{
		Particle outputParticles[PARTICLE_MAX_SIZE];
	};
	layout(std140,binding = 2) buffer ParticleRunContext{
		uint inputCounter;
		uint outputCounter;
		float duration;
		float lifetime;
	};
	%2

	#define inID  gl_GlobalInvocationID.x
	#define inParticle inputParticles[inID]
	#define outParticle outputParticles[outID]

	void main(){
		if(inID >= inputCounter||inParticle.life>lifetime)
			return;
		uint outID = atomicAdd(outputCounter,1);
		outParticle.life = inParticle.life + duration;
		%3
	}
	)").arg(getParticleDefine(), uniformInfo.uniformDefineCode, mParticle->getParticleSystem()->getUpdater()->getUpdateCode());
	QShader computeUpdater = QSceneRenderer::createShaderFromCode(QShader::ComputeStage, particleRunCode.toLocal8Bit());
	if (!computeUpdater.isValid()) {
		mComputePipeline.reset(nullptr);
		return;
	}

	mStaticMeshProxy->recreatePipeline();

	mComputePipeline.reset(RHI->newComputePipeline());
	mComputeBindings[0].reset(RHI->newShaderResourceBindings());
	mComputeBindings[1].reset(RHI->newShaderResourceBindings());

	QVector<QRhiShaderResourceBinding> shaderBindings;
	shaderBindings << QRhiShaderResourceBinding::bufferLoad(0, QRhiShaderResourceBinding::ComputeStage, mParticlesBuffer[0].get());
	shaderBindings << QRhiShaderResourceBinding::bufferStore(1, QRhiShaderResourceBinding::ComputeStage, mParticlesBuffer[1].get());
	shaderBindings << QRhiShaderResourceBinding::bufferLoadStore(2, QRhiShaderResourceBinding::ComputeStage, mParticleCounterBuffer.get());
	shaderBindings << uniformInfo.bindings;
	mComputeBindings[0]->setBindings(shaderBindings.begin(), shaderBindings.end());
	mComputeBindings[0]->create();
	shaderBindings[0] = QRhiShaderResourceBinding::bufferLoad(0, QRhiShaderResourceBinding::ComputeStage, mParticlesBuffer[1].get());
	shaderBindings[1] = QRhiShaderResourceBinding::bufferStore(1, QRhiShaderResourceBinding::ComputeStage, mParticlesBuffer[0].get());
	mComputeBindings[1]->setBindings(shaderBindings.begin(), shaderBindings.end());
	mComputeBindings[1]->create();
	mComputePipeline->setShaderResourceBindings(mComputeBindings[mInputIndex].get());

	mComputePipeline->setShaderStage({ QRhiShaderStage::Compute,computeUpdater });
	Q_ASSERT(mComputePipeline->create());

	mMatrixComputePipline.reset(RHI->newComputePipeline());
	mMatrixBindings[0].reset(RHI->newShaderResourceBindings());
	mMatrixBindings[0]->setBindings({
		QRhiShaderResourceBinding::bufferLoad(0, QRhiShaderResourceBinding::ComputeStage, mParticlesBuffer[1].get()),
		QRhiShaderResourceBinding::bufferStore(1, QRhiShaderResourceBinding::ComputeStage,mMatrixBuffer.get()),
									});
	mMatrixBindings[0]->create();

	mMatrixBindings[1].reset(RHI->newShaderResourceBindings());
	mMatrixBindings[1]->setBindings({
		QRhiShaderResourceBinding::bufferLoad(0, QRhiShaderResourceBinding::ComputeStage, mParticlesBuffer[0].get()),
		QRhiShaderResourceBinding::bufferStore(1, QRhiShaderResourceBinding::ComputeStage,mMatrixBuffer.get()),
									});
	mMatrixBindings[1]->create();

	mMatrixComputePipline->setShaderResourceBindings(mMatrixBindings[0].get());
	QShader matrixCompute = QSceneRenderer::createShaderFromCode(QShader::ComputeStage, R"(
		#version 450
		#define LOCAL_SIZE 256
		#define PARTICLE_MAX_SIZE 1000000
		layout (local_size_x = LOCAL_SIZE) in;
		struct Particle {
			vec3 position;
			vec3 rotation;
			vec3 scaling;
			vec3 velocity;
			float life;
		};

		layout(std140,binding = 0) buffer InputParticle{
			Particle intputParticles[PARTICLE_MAX_SIZE];
		};

		layout(std140,binding = 1) buffer OutputParticle{
			mat4 outputMatrix[PARTICLE_MAX_SIZE];
		};

		void main(){
			const uint index = gl_GlobalInvocationID.x ;      //根据工作单元的位置换算出内存上的索引
			vec3 position = intputParticles[index].position;
			vec3 rotation  = intputParticles[index].rotation;
			vec3 scale    = intputParticles[index].scaling;

			mat4 matScale = mat4(scale.x,0,0,0,
								 0,scale.y,0,0,
								 0,0,scale.z,0,
								 0,0,0,1);

	        vec2 X=vec2(sin(rotation.x),cos(rotation.x));
            vec2 Y=vec2(sin(rotation.y),cos(rotation.y));
            vec2 Z=vec2(sin(rotation.z),cos(rotation.z));

			mat4 matRotation=mat4(Z.y*Y.y,Z.y*Y.x*X.x-Z.x*X.y,Z.y*Y.x*X.y-Z.x*X.x,0,
                                  Z.x*Y.y,Z.x*Y.x*X.x+Z.y*X.y,Z.x*Y.x*X.y-Z.y*X.x,0,
                                  -Y.x,Y.y*X.x,Y.y*X.y,0,
                                  0,0,0,1
                                  );
			mat4 matTranslate=mat4(1,0,0,0,
								0,1,0,0,
								0,0,1,0,
								position.x,position.y,position.z,1);
			outputMatrix[index] = matTranslate*matScale*matRotation;
			//outputMatrix[index] = matRotation;
		}
	)");
	mMatrixComputePipline->setShaderStage({ QRhiShaderStage::Compute,matrixCompute });
	Q_ASSERT(mMatrixComputePipline->create());
}

void QDefaultProxyParticle::uploadResource(QRhiResourceUpdateBatch* batch)
{
	mStaticMeshProxy->uploadResource(batch);
}

void QDefaultProxyParticle::updateResource(QRhiResourceUpdateBatch* batch)
{
	if (mParticle->getStaticMesh()->bNeedRecreateResource.receive()) {
		mStaticMeshProxy = std::make_shared<QDefaultProxyStaticMesh>(mParticle->getStaticMesh());
		mStaticMeshProxy->setParentParticle(mParticle);
		mStaticMeshProxy->mRenderer = mRenderer;
		mStaticMeshProxy->recreateResource();
		mStaticMeshProxy->uploadResource(batch);
	}
	if (mParticle->getStaticMesh()->bNeedRecreatePipeline.receive()) {
		mStaticMeshProxy->recreatePipeline();
	}
	mStaticMeshProxy->updateResource(batch);
}

void QDefaultProxyParticle::updatePrePass(QRhiCommandBuffer* cmdBuffer)
{
	if (!mComputePipeline)
		return;
	float currentSecond = QTime::currentTime().msecsSinceStartOfDay() / 1000.0;
	mDuration = currentSecond - mLastSecond;
	mLastSecond = currentSecond;

	QVector<QParticleSystem::Particle> particles = mParticle->getParticleSystem()->getEmitter()->update();
	QRhiResourceUpdateBatch* u = RHI->nextResourceUpdateBatch();
	if (!particles.isEmpty()) {
		int incrememt = qMin((uint)particles.size(), QParticleSystem::PARTICLE_MAX_SIZE - mCtx.inputCounter);
		u->uploadStaticBuffer(mParticlesBuffer[mInputIndex].get(), sizeof(QParticleSystem::Particle) * mCtx.inputCounter, sizeof(QParticleSystem::Particle) * incrememt, particles.data());
		mCtx.inputCounter += incrememt;
	}

	mCtx.lifetime = mParticle->getParticleSystem()->getEmitter()->getLifetime();
	mCtx.duration = mDuration;

	u->uploadStaticBuffer(mParticleCounterBuffer.get(), &mCtx);

	cmdBuffer->beginComputePass(u);
	cmdBuffer->setComputePipeline(mComputePipeline.get());
	cmdBuffer->setShaderResources(mComputeBindings[mInputIndex].get());
	cmdBuffer->dispatch(qCeil(mCtx.inputCounter / 256.0), 1, 1);

	u = RHI->nextResourceUpdateBatch();
	mCtxReader.completed = [this]() {
		ParticleRunContext buffer;
		memcpy(&buffer, mCtxReader.data.constData(), mCtxReader.data.size());
		mCtx.inputCounter = buffer.outputCounter;
		mCtx.outputCounter = 0;
	};
	u->readBackBuffer(mParticleCounterBuffer.get(), 0, sizeof(ParticleRunContext), &mCtxReader);
	cmdBuffer->endComputePass(u);
	RHI->finish();

	cmdBuffer->beginComputePass();
	cmdBuffer->setComputePipeline(mMatrixComputePipline.get());
	cmdBuffer->setShaderResources(mMatrixBindings[mInputIndex].get());
	cmdBuffer->dispatch(qCeil(mCtx.inputCounter / 256.0), 1, 1);
	cmdBuffer->endComputePass();

	qSwap(mInputIndex, mOutputIndex);
}

void QDefaultProxyParticle::drawInPass(QRhiCommandBuffer* cmdBuffer, const QRhiViewport& viewport) {
	if (!mStaticMeshProxy->mPipeline)
		return;
	cmdBuffer->setGraphicsPipeline(mStaticMeshProxy->mPipeline.get());
	cmdBuffer->setViewport(viewport);
	cmdBuffer->setShaderResources();
	QRhiCommandBuffer::VertexInput VertexInputs[] = {
		{mStaticMeshProxy->mVertexBuffer.get(), 0},
		{mMatrixBuffer.get(),0}
	};
	cmdBuffer->setVertexInput(0, 2, VertexInputs, mStaticMeshProxy->mIndexBuffer.get(), 0, QRhiCommandBuffer::IndexUInt32);
	cmdBuffer->drawIndexed(mStaticMeshProxy->mIndexBuffer->size() / sizeof(QPrimitiveComponent::Index), mCtx.inputCounter);
	//qDebug() << mCtx.inputCounter;
}