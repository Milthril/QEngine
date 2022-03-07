#include "QDefaultProxyParticle.h"
#include "Render\Scene\Component\QPrimitiveComponent.h"
#include "Render\Scene\Component\QParticleComponent.h"

QDefaultProxyParticle::QDefaultProxyParticle(std::shared_ptr<QParticleComponent> particle)
	:mParticle(particle)
{
	mNumReader.completed = [this]() {
		memcpy(&mNumOfParticles, mNumReader.data.constData(), mNumReader.data.size());
	};;
}

void QDefaultProxyParticle::uploadResource(QRhiResourceUpdateBatch* batch)
{
	mShapeProxy->uploadResource(batch);
}

void QDefaultProxyParticle::updateResource(QRhiResourceUpdateBatch* batch)
{
	mShapeProxy->updateResource(batch);
}

void QDefaultProxyParticle::updatePrePass(QRhiCommandBuffer* cmdBuffer)
{
	const QVector<QParticleComponent::Particle>& particles = mParticle->takeParticles();
	QRhiResourceUpdateBatch* u = mRhi->nextResourceUpdateBatch();
	if (!particles.isEmpty()) {
		int incrememt = qMin(particles.size(), QParticleComponent::PARTICLE_MAX_SIZE - mNumOfParticles);
		u->uploadStaticBuffer(mParticlesBuffer[mInputIndex].get(), offsetof(QParticleComponent::ParticleBuffer, particles) + sizeof(QParticleComponent::Particle) * mNumOfParticles, sizeof(QParticleComponent::Particle) * incrememt, particles.data());
		mNumOfParticles += incrememt;
		u->uploadStaticBuffer(mParticlesBuffer[mInputIndex].get(), 0, sizeof(QParticleComponent::ParticleBuffer::counter), &mNumOfParticles);
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

	u = mRhi->nextResourceUpdateBatch();
	int reset = 0;
	u->uploadStaticBuffer(mParticlesBuffer[mInputIndex].get(), 0, sizeof(QParticleComponent::ParticleBuffer::counter), &reset);
	u->readBackBuffer(mParticlesBuffer[mOutputIndex].get(), 0, sizeof(QParticleComponent::ParticleBuffer::counter), &mNumReader);
	mRhi->finish();
	QRhiBufferReadbackResult* ret = new QRhiBufferReadbackResult;
	ret->completed = [ret]() {
		float mat[QParticleComponent::PARTICLE_MAX_SIZE][16];
		memcpy(&mat, ret->data.constData(), ret->data.size());
	};
	u->readBackBuffer(mMatrixBuffer.get(), 0, sizeof(float) * 16 * QParticleComponent::PARTICLE_MAX_SIZE, ret);

	ret = new QRhiBufferReadbackResult;
	ret->completed = [ret]() {
		QParticleComponent::ParticleBuffer buffer;
		memcpy(&buffer, ret->data.constData(), ret->data.size());
	};
	u->readBackBuffer(mParticlesBuffer[mOutputIndex].get(), 0, sizeof(QParticleComponent::ParticleBuffer), ret);
	cmdBuffer->resourceUpdate(u);

	qSwap(mInputIndex, mOutputIndex);
	qDebug() << mNumOfParticles;
}

void QDefaultProxyParticle::recreateResource() {
	mShapeProxy = mRenderer->createPrimitiveProxy(mParticle->getShape());
	mShapeProxy->recreateResource();
	for (int i = 0; i < std::size(mParticlesBuffer); i++) {
		mParticlesBuffer[i].reset(mRhi->newBuffer(QRhiBuffer::Static, QRhiBuffer::UsageFlag::VertexBuffer | QRhiBuffer::UsageFlag::StorageBuffer, sizeof(QParticleComponent::ParticleBuffer)));
		mParticlesBuffer[i]->create();
	}
	mMatrixBuffer.reset(mRhi->newBuffer(QRhiBuffer::Static, QRhiBuffer::UsageFlag::VertexBuffer | QRhiBuffer::UsageFlag::StorageBuffer, sizeof(float) * 16 * QParticleComponent::PARTICLE_MAX_SIZE));
	mMatrixBuffer->create();

	recreateComputePipeline();
	recreateGraphicsPipline();
}

void QDefaultProxyParticle::recreateGraphicsPipline()
{
	mPipeline.reset(mRhi->newGraphicsPipeline());
	QRhiGraphicsPipeline::TargetBlend blendState;
	blendState.enable = true;
	mPipeline->setTargetBlends({ blendState });
	mPipeline->setTopology(mShapeProxy->mComponent->getTopology());
	mPipeline->setDepthTest(true);
	mPipeline->setDepthWrite(true);
	mPipeline->setSampleCount(mRenderer->getSampleCount());

	QShader vs = QSceneRenderer::createShaderFromCode(QShader::Stage::VertexStage, R"(#version 440
	layout(location = 0) in vec3 position;
	layout(location = 1) in vec3 normal;
	layout(location = 2) in vec3 tangent;
	layout(location = 3) in vec3 bitangent;
	layout(location = 4) in vec2 texCoord;
	layout(location = 5) in vec4 baseColor;

	layout(location = 0) out vec4 vBaseColor;

	out gl_PerVertex{
		vec4 gl_Position;
	};

	layout(std140,binding = 0) uniform buf{
		mat4 mvp;
	}ubuf;

	void main(){
		vBaseColor = baseColor;
		gl_Position = ubuf.mvp * vec4(position,1.0f);
	}
	)");

	QShader fs = QSceneRenderer::createShaderFromCode(QShader::Stage::FragmentStage, R"(#version 440
	layout(location = 0) in vec4 vBaseColor;
	layout(location = 0) out vec4 fragColor;
	void main(){
	    fragColor = vBaseColor;
	}
	)");

	Q_ASSERT(fs.isValid());

	mPipeline->setShaderStages({
		{ QRhiShaderStage::Vertex, vs },
		{ QRhiShaderStage::Fragment, fs }
	});

	QRhiVertexInputLayout inputLayout;
	inputLayout.setBindings({
		sizeof(QPrimitiveComponent::Vertex),
		sizeof(QParticleComponent::Particle)
	});
	inputLayout.setAttributes({
		{ 0, 0, QRhiVertexInputAttribute::Float3, offsetof(QPrimitiveComponent::Vertex,position)},
		{ 0, 1, QRhiVertexInputAttribute::Float3, offsetof(QPrimitiveComponent::Vertex,normal)},
		{ 0, 2, QRhiVertexInputAttribute::Float3, offsetof(QPrimitiveComponent::Vertex,tangent)},
		{ 0, 3, QRhiVertexInputAttribute::Float3, offsetof(QPrimitiveComponent::Vertex,bitangent)},
		{ 0, 4, QRhiVertexInputAttribute::Float3, offsetof(QPrimitiveComponent::Vertex,texCoord)},
		{ 0, 5, QRhiVertexInputAttribute::Float3, offsetof(QPrimitiveComponent::Vertex,baseColor),},

		{ 1, 6, QRhiVertexInputAttribute::Float3, offsetof(QParticleComponent::Particle,position)},
		{ 1, 7, QRhiVertexInputAttribute::Float3, offsetof(QParticleComponent::Particle,rotation)},
		{ 1, 8, QRhiVertexInputAttribute::Float3, offsetof(QParticleComponent::Particle,scaling)},
		{ 1, 9, QRhiVertexInputAttribute::Float3, offsetof(QParticleComponent::Particle,velocity)},
		{ 1, 10, QRhiVertexInputAttribute::Float, offsetof(QParticleComponent::Particle,life)},
	});
	mPipeline->setVertexInputLayout(inputLayout);

	mShaderResourceBindings.reset(mRhi->newShaderResourceBindings());
	mShaderResourceBindings->setBindings({
			QRhiShaderResourceBinding::uniformBuffer(0,QRhiShaderResourceBinding::VertexStage,mUniformBuffer.get())
	});
	mShaderResourceBindings->create();
	mPipeline->setShaderResourceBindings(mShaderResourceBindings.get());

	mPipeline->setRenderPassDescriptor(mRenderer->getRenderPassDescriptor().get());

	Q_ASSERT(mPipeline->create());
}

void QDefaultProxyParticle::recreateComputePipeline()
{
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
		#define PARTICLE_MAX_SIZE 100
		layout (local_size_x = LOCAL_SIZE) in;
		struct Particle {
			vec3 position;
			vec3 rotation;
			vec3 scaling;
			vec3 velocity;
			float life;
		};
		layout(std140,binding = 0) buffer InputParticle{
			int inputCounter;
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
                                  Z.x*Y.y,Z.x*Y.x*X.x-Z.y*X.y,Z.x*Y.x*X.y-Z.y*X.x,0,
                                  -Y.x,Y.y*X.x,Y.y*X.y,0,
                                  0,0,0,1
                                  );
			mat4 matTranslate=mat4(1,0,0,0,
								0,1,0,0,
								0,0,1,0,
								position.x,position.y,position.z,1);
			outputMatrix[index] = matTranslate*matRotation*matScale;
		}
	)");
	mMatrixComputePipline->setShaderStage({ QRhiShaderStage::Compute,matrixCompute });
	Q_ASSERT(mMatrixComputePipline->create());
}

void QDefaultProxyParticle::drawInPass(QRhiCommandBuffer* cmdBuffer, const QRhiViewport& viewport) {
	cmdBuffer->setGraphicsPipeline(mShapeProxy->mPipeline.get());
	cmdBuffer->setViewport(viewport);
	cmdBuffer->setShaderResources();
	const QRhiCommandBuffer::VertexInput VertexInput(mShapeProxy->mVertexBuffer.get(), 0);
	cmdBuffer->setVertexInput(0, 1, &VertexInput);
	cmdBuffer->draw(36);
	//cmdBuffer->setGraphicsPipeline(mPipeline.get());
	//cmdBuffer->setViewport(viewport);
	//cmdBuffer->setShaderResources();
}