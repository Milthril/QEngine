#include "QDefaultProxyParticle.h"
#include "Render\Scene\Component\Particle\QParticleComponent.h"
#include "QDateTime"

QDefaultProxyParticle::QDefaultProxyParticle(std::shared_ptr<QParticleComponent> particle)
	:mParticle(particle)
{
}

void QDefaultProxyParticle::recreateResource() {
	mStaticMeshProxy = mRenderer->createPrimitiveProxy(mParticle->getStaticMesh());
	mStaticMeshProxy->recreateResource();
	for (int i = 0; i < std::size(mParticlesBuffer); i++) {
		mParticlesBuffer[i].reset(mRhi->newBuffer(QRhiBuffer::Static, QRhiBuffer::UsageFlag::VertexBuffer | QRhiBuffer::UsageFlag::StorageBuffer, sizeof(QParticleComponent::ParticleBuffer)));
		mParticlesBuffer[i]->create();
	}
	mMatrixBuffer.reset(mRhi->newBuffer(QRhiBuffer::Static, QRhiBuffer::UsageFlag::VertexBuffer | QRhiBuffer::UsageFlag::StorageBuffer, sizeof(float) * 16 * QParticleComponent::PARTICLE_MAX_SIZE));
	mMatrixBuffer->create();
}

void QDefaultProxyParticle::recreatePipeline(PipelineUsageFlags flags /*= PipelineUsageFlag::Normal*/)
{
	mStaticMeshProxy->recreatePipeline(flags | PipelineUsageFlag::Instancing);

	mComputePipeline.reset(mRhi->newComputePipeline());
	mComputeBindings[0].reset(mRhi->newShaderResourceBindings());
	mComputeBindings[0]->setBindings({
		QRhiShaderResourceBinding::bufferLoad(0, QRhiShaderResourceBinding::ComputeStage, mParticlesBuffer[0].get()),
		QRhiShaderResourceBinding::bufferStore(1, QRhiShaderResourceBinding::ComputeStage, mParticlesBuffer[1].get()),
	});
	mComputeBindings[0]->create();
	mComputeBindings[1].reset(mRhi->newShaderResourceBindings());
	mComputeBindings[1]->setBindings({
		QRhiShaderResourceBinding::bufferLoad(0, QRhiShaderResourceBinding::ComputeStage, mParticlesBuffer[1].get()),
		QRhiShaderResourceBinding::bufferStore(1, QRhiShaderResourceBinding::ComputeStage, mParticlesBuffer[0].get()),
	});
	mComputeBindings[1]->create();
	mComputePipeline->setShaderResourceBindings(mComputeBindings[0].get());
	QShader computeUpdater = QSceneRenderer::createShaderFromCode(QShader::ComputeStage, mParticle->getUpdater());
	mComputePipeline->setShaderStage({ QRhiShaderStage::Compute,computeUpdater });
	Q_ASSERT(mComputePipeline->create());

	mMatrixComputePipline.reset(mRhi->newComputePipeline());
	mMatrixBindings[0].reset(mRhi->newShaderResourceBindings());
	mMatrixBindings[0]->setBindings({
		QRhiShaderResourceBinding::bufferLoad(0, QRhiShaderResourceBinding::ComputeStage, mParticlesBuffer[0].get()),
		QRhiShaderResourceBinding::bufferStore(1, QRhiShaderResourceBinding::ComputeStage,mMatrixBuffer.get()),
	});
	mMatrixBindings[0]->create();
	mMatrixBindings[1].reset(mRhi->newShaderResourceBindings());
	mMatrixBindings[1]->setBindings({
		QRhiShaderResourceBinding::bufferLoad(0, QRhiShaderResourceBinding::ComputeStage, mParticlesBuffer[1].get()),
		QRhiShaderResourceBinding::bufferStore(1, QRhiShaderResourceBinding::ComputeStage,mMatrixBuffer.get()),
	});
	mMatrixBindings[1]->create();
	mMatrixComputePipline->setShaderResourceBindings(mMatrixBindings[0].get());
	QShader matrixCompute = QSceneRenderer::createShaderFromCode(QShader::ComputeStage, R"(
		#version 450
		#define LOCAL_SIZE 256
		#define PARTICLE_MAX_SIZE 100000
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

uint32_t QDefaultProxyParticle::allocIndex()
{
	if (mIndexPool.isEmpty()) {
		return mNumOfParticles++;
	}
	return mIndexPool.takeLast();
}

void QDefaultProxyParticle::uploadResource(QRhiResourceUpdateBatch* batch)
{
	mStaticMeshProxy->uploadResource(batch);
	mLastSecond = QTime::currentTime().msecsSinceStartOfDay() / 1000.0;;
}

void QDefaultProxyParticle::updateResource(QRhiResourceUpdateBatch* batch)
{
	mStaticMeshProxy->updateResource(batch);
}

void QDefaultProxyParticle::updatePrePass(QRhiCommandBuffer* cmdBuffer)
{
	float currentSecond = QTime::currentTime().msecsSinceStartOfDay() / 1000.0;
	mDuration = currentSecond - mLastSecond;
	mLastSecond = currentSecond;
	for (int i = 0; i < mNumOfParticles; i++) {
		float& age = mAgePool[i];
		age += mDuration;
		if (age > mParticle->getLifetime()) {
			mIndexPool.push_back(i);
		}
	}
	const QVector<QParticleComponent::Particle>& particles = mParticle->takeParticles();
	QRhiResourceUpdateBatch* u = mRhi->nextResourceUpdateBatch();
	if (!particles.isEmpty()) {
		int incrememt = qMin(particles.size(), QParticleComponent::PARTICLE_MAX_SIZE + mIndexPool.size() - mNumOfParticles);
		for (int i = 0; i < incrememt; i++) {
			uint32_t index = allocIndex();
			mAgePool[index] = particles[i].life;
			u->uploadStaticBuffer(mParticlesBuffer[mInputIndex].get(), sizeof(QParticleComponent::Particle) * index, sizeof(QParticleComponent::Particle), &particles[i]);
		}
	}
	cmdBuffer->beginComputePass(u);
	cmdBuffer->setComputePipeline(mComputePipeline.get());
	cmdBuffer->setShaderResources(mComputeBindings[mInputIndex].get());
	cmdBuffer->dispatch(qCeil(mNumOfParticles / 256.0), 1, 1);
	cmdBuffer->endComputePass();

	cmdBuffer->beginComputePass();
	cmdBuffer->setComputePipeline(mMatrixComputePipline.get());
	cmdBuffer->setShaderResources(mMatrixBindings[mOutputIndex].get());
	cmdBuffer->dispatch(qCeil(mNumOfParticles / 256.0), 1, 1);
	cmdBuffer->endComputePass();

	QRhiBufferReadbackResult* ret = new QRhiBufferReadbackResult;
	ret->completed = [ret]() {
		float buf[100][16];
		memcpy(buf, ret->data.constData(), ret->data.size());
	};

	u->readBackBuffer(mMatrixBuffer.get(), 0, sizeof(float) * 16 * 100, ret);
	mRhi->finish();

	qSwap(mInputIndex, mOutputIndex);
}

void QDefaultProxyParticle::drawInPass(QRhiCommandBuffer* cmdBuffer, const QRhiViewport& viewport) {
	cmdBuffer->setGraphicsPipeline(mStaticMeshProxy->mPipeline.get());
	cmdBuffer->setViewport(viewport);
	cmdBuffer->setShaderResources();
	QRhiCommandBuffer::VertexInput VertexInputs[] = {
		{mStaticMeshProxy->mVertexBuffer.get(), 0},
		{mMatrixBuffer.get(),0}
	};
	cmdBuffer->setVertexInput(0, 2, VertexInputs, mStaticMeshProxy->mIndexBuffer.get(), 0, QRhiCommandBuffer::IndexUInt32);
	cmdBuffer->drawIndexed(mStaticMeshProxy->mIndexBuffer->size() / sizeof(QPrimitiveComponent::Index), mNumOfParticles);
}