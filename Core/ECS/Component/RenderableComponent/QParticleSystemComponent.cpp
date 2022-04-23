#include "QParticleSystemComponent.h"
#include "ECS\System\RenderSystem\QRenderSystem.h"
#include "ECS\QEntity.h"
#include "Asset\PartcleSystem\Emitter\QParticleEmitter.h"
#include "QDateTime"
#include "Asset\GAssetMgr.h"

const std::shared_ptr<Asset::StaticMesh>& QParticleSystemComponent::getStaticMesh() const {
	return mStaticMesh;
}

void QParticleSystemComponent::setStaticMesh(std::shared_ptr<Asset::StaticMesh> val) {
	mStaticMesh = val;
	if (mStaticMesh) {
		setMaterial(TheAssetMgr->load<Asset::Material>(mStaticMesh->getMaterialPath()));
		if (mParticleSystem) {
			mParticleSystem->setStaticMeshPath(mStaticMesh->getRefPath());
		}
	}
	bNeedRecreatePipeline.active();
	bNeedRecreateResource.active();
}

std::shared_ptr<Asset::Material> QParticleSystemComponent::getMaterial() {
	return mMaterial;
}

void QParticleSystemComponent::setMaterial(std::shared_ptr<Asset::Material> val) {
	if (mMaterial) {
		mMaterial->removeRef(this);
	}
	mMaterial = val;
	if (val) {
		mStaticMesh->setMaterialPath(val->getRefPath());
		val->addRef(this);
	}
	bNeedRecreatePipeline.active();
}

std::shared_ptr<Asset::ParticleSystem> QParticleSystemComponent::getParticleSystem() const {
	return mParticleSystem;
}

void QParticleSystemComponent::setParticleSystem(std::shared_ptr<Asset::ParticleSystem> val) {
	if (mParticleSystem) {
		mParticleSystem->getUpdater()->removeRef(this);
	}
	mParticleSystem = val;
	if (mParticleSystem) {
		mParticleSystem->getUpdater()->addRef(this);
		setStaticMesh(TheAssetMgr->load<Asset::StaticMesh>(mParticleSystem->getStaticMeshPath()));
	}
	bNeedRecreatePipeline.active();
	bNeedRecreateResource.active();
}

void QParticleSystemComponent::recreateResource() {
	if (!mStaticMesh || !mMaterial)
		return;
	mUniformBuffer.reset(RHI->newBuffer(QRhiBuffer::Type::Dynamic, QRhiBuffer::UniformBuffer, sizeof(UniformMatrix)));
	mUniformBuffer->create();

	mVertexBuffer.reset(RHI->newBuffer(QRhiBuffer::Type::Static, QRhiBuffer::VertexBuffer, sizeof(Asset::StaticMesh::Vertex) * mStaticMesh->getVertices().size()));
	mVertexBuffer->create();

	mIndexBuffer.reset(RHI->newBuffer(QRhiBuffer::Type::Static, QRhiBuffer::IndexBuffer, sizeof(Asset::StaticMesh::Index) * mStaticMesh->getIndices().size()));
	mIndexBuffer->create();

	mParticlesBuffer[0].reset(RHI->newBuffer(QRhiBuffer::Static, QRhiBuffer::UsageFlag::VertexBuffer | QRhiBuffer::UsageFlag::StorageBuffer, sizeof(Asset::ParticleSystem::ParticleBuffer)));
	mParticlesBuffer[0]->create();
	mParticlesBuffer[1].reset(RHI->newBuffer(QRhiBuffer::Static, QRhiBuffer::UsageFlag::VertexBuffer | QRhiBuffer::UsageFlag::StorageBuffer, sizeof(Asset::ParticleSystem::ParticleBuffer)));
	mParticlesBuffer[1]->create();

	mParticleCounterBuffer.reset(RHI->newBuffer(QRhiBuffer::Static, QRhiBuffer::UsageFlag::StorageBuffer, sizeof(ParticleRunContext)));
	mParticleCounterBuffer->create();

	mMatrixBuffer.reset(RHI->newBuffer(QRhiBuffer::Static, QRhiBuffer::UsageFlag::VertexBuffer | QRhiBuffer::UsageFlag::StorageBuffer, sizeof(float) * 16 * Asset::ParticleSystem::PARTICLE_MAX_SIZE));
	mMatrixBuffer->create();
}

void QParticleSystemComponent::recreatePipeline() {
	if (!mStaticMesh|| mStaticMesh->getVertices().isEmpty() ||!mMaterial||!mParticleSystem||!mParticleSystem->getUpdater()||!mParticleSystem->getEmitter())
		return;

	QVector<QRhiVertexInputBinding> inputBindings;
	inputBindings << QRhiVertexInputBinding{ sizeof(Asset::StaticMesh::Vertex) };
	inputBindings << QRhiVertexInputBinding{ sizeof(float) * 16 ,QRhiVertexInputBinding::PerInstance };

	QVector<QRhiVertexInputAttribute> attributeList;
	attributeList << QRhiVertexInputAttribute{ 0, 0, QRhiVertexInputAttribute::Float3, offsetof(Asset::StaticMesh::Vertex, position) };
	attributeList << QRhiVertexInputAttribute{ 0, 1, QRhiVertexInputAttribute::Float3, offsetof(Asset::StaticMesh::Vertex,normal) };
	attributeList << QRhiVertexInputAttribute{ 0, 2, QRhiVertexInputAttribute::Float3, offsetof(Asset::StaticMesh::Vertex,tangent) };
	attributeList << QRhiVertexInputAttribute{ 0, 3, QRhiVertexInputAttribute::Float3, offsetof(Asset::StaticMesh::Vertex,bitangent) };
	attributeList << QRhiVertexInputAttribute{ 0, 4, QRhiVertexInputAttribute::Float3, offsetof(Asset::StaticMesh::Vertex,texCoord) };
	attributeList << QRhiVertexInputAttribute{ 1, 5, QRhiVertexInputAttribute::Float4, 0,0 };
	attributeList << QRhiVertexInputAttribute{ 1, 6, QRhiVertexInputAttribute::Float4, 4 * sizeof(float),1 };
	attributeList << QRhiVertexInputAttribute{ 1, 7, QRhiVertexInputAttribute::Float4, 8 * sizeof(float),2 };
	attributeList << QRhiVertexInputAttribute{ 1, 8, QRhiVertexInputAttribute::Float4, 12 * sizeof(float),3 };
	QString vertexShaderCode = R"(#version 440
	layout(location = 0) in vec3 inPosition;
	layout(location = 1) in vec3 inNormal;
	layout(location = 2) in vec3 inTangent;
	layout(location = 3) in vec3 inBitangent;
	layout(location = 4) in vec2 inUV;
	layout(location = 5) in mat4 insMat;

	layout(location = 0) out vec2 vUV;
	layout(location = 1) out vec3 vWorldPosition;
	layout(location = 2) out mat3 vTangentBasis;

	out gl_PerVertex{
		vec4 gl_Position;
	};

	layout(std140,binding = 0) uniform UniformMatrix{
		mat4 MVP;
		mat4 M;
	}Matrix;
	void main(){
		vUV = inUV;
		vec4 pos =  insMat * vec4(inPosition,1.0f);
		vWorldPosition = vec3(Matrix.M * pos);
		vTangentBasis =  mat3(Matrix.M) * mat3(inTangent, inBitangent, inNormal);
		gl_Position = Matrix.MVP * pos;
	}
	)";

	QRhiVertexInputLayout inputLayout;
	inputLayout.setBindings(inputBindings.begin(), inputBindings.end());
	inputLayout.setAttributes(attributeList.begin(), attributeList.end());
	QShader vs = QRenderSystem::createShaderFromCode(QShader::Stage::VertexStage, vertexShaderCode.toLocal8Bit());
	if (!vs.isValid()) {
		mPipeline.reset(nullptr);
		return;
	}
	const QRhiUniform::UniformInfo& materialInfo = mMaterial->getUniformInfo(1);
	QString defineCode = materialInfo.uniformDefineCode;

	QString outputCode = mMaterial->getShadingCode();

	if (TheRenderSystem->isEnableDebug()) {
		defineCode.prepend("layout (location = 4) out vec4 CompId;\n");
		outputCode.append(QString("CompId = %1;\n").arg(getEntityIdVec4String()));
	}

	QString fragShaderCode = QString(R"(#version 440
	layout(location = 0) in vec2 vUV;
	layout(location = 1) in vec3 vWorldPosition;
	layout(location = 2) in mat3 vTangentBasis;

	layout(location = 0) out vec4 outBaseColor;
	layout(location = 1) out vec4 outPosition;
	layout(location = 2) out vec4 outNormal_Metalness;
	layout(location = 3) out vec4 outTangent_Roughness;
	%1
	#define outNormal		(outNormal_Metalness.rgb)
	#define outMetalness	(outNormal_Metalness.a)
	#define outTangent		(outTangent_Roughness.rgb)
	#define outRoughness	(outTangent_Roughness.a)

	void main(){
		outPosition = vec4(vWorldPosition,1.0);
		outTangent = vTangentBasis[0];
		outNormal = vTangentBasis[2];
		%2
	}
	)").arg(defineCode).arg(outputCode);

	QShader fs = QRenderSystem::createShaderFromCode(QShader::Stage::FragmentStage, fragShaderCode.toLocal8Bit());
	if (!fs.isValid()) {
		mPipeline.reset(nullptr);
		return;
	}

	mPipeline.reset(RHI->newGraphicsPipeline());
	
	mPipeline->setVertexInputLayout(inputLayout);
	const auto& blendStates = TheRenderSystem->renderer()->getDeferPassBlendStates();
	mPipeline->setTargetBlends(blendStates.begin(), blendStates.end());
	mPipeline->setTopology(QRhiGraphicsPipeline::Topology::Triangles);
	mPipeline->setDepthOp(QRhiGraphicsPipeline::LessOrEqual);
	mPipeline->setDepthTest(true);
	mPipeline->setDepthWrite(true);
	mPipeline->setSampleCount(TheRenderSystem->renderer()->getDeferPassSampleCount());

	mPipeline->setShaderStages({
		{ QRhiShaderStage::Vertex, vs },
		{ QRhiShaderStage::Fragment, fs }
							   });
	mShaderResourceBindings.reset(RHI->newShaderResourceBindings());
	QVector<QRhiShaderResourceBinding> shaderBindings;
	shaderBindings << QRhiShaderResourceBinding::uniformBuffer(0, QRhiShaderResourceBinding::VertexStage, mUniformBuffer.get());
	shaderBindings << materialInfo.bindings;
	mShaderResourceBindings->setBindings(shaderBindings.begin(), shaderBindings.end());
	mShaderResourceBindings->create();

	mPipeline->setShaderResourceBindings(mShaderResourceBindings.get());

	mPipeline->setRenderPassDescriptor(TheRenderSystem->renderer()->getDeferPassDescriptor());

	mPipeline->create();

	const QRhiUniform::UniformInfo& uniformInfo = mParticleSystem->getUpdater()->getUniformInfo(3,"UBO", QRhiShaderResourceBinding::ComputeStage);

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
	)").arg(Asset::getParticleDefine(), uniformInfo.uniformDefineCode, mParticleSystem->getUpdater()->getUpdateCode());
	QShader computeUpdater = QRenderSystem::createShaderFromCode(QShader::ComputeStage, particleRunCode.toLocal8Bit());
	if (!computeUpdater.isValid()) {
		mComputePipeline.reset(nullptr);
		return;
	}
	mComputePipeline.reset(RHI->newComputePipeline());
	mComputeBindings[0].reset(RHI->newShaderResourceBindings());
	mComputeBindings[1].reset(RHI->newShaderResourceBindings());

	QVector<QRhiShaderResourceBinding> computeBindings;
	computeBindings << QRhiShaderResourceBinding::bufferLoad(0, QRhiShaderResourceBinding::ComputeStage, mParticlesBuffer[0].get());
	computeBindings << QRhiShaderResourceBinding::bufferStore(1, QRhiShaderResourceBinding::ComputeStage, mParticlesBuffer[1].get());
	computeBindings << QRhiShaderResourceBinding::bufferLoadStore(2, QRhiShaderResourceBinding::ComputeStage, mParticleCounterBuffer.get());
	computeBindings << uniformInfo.bindings;
	mComputeBindings[0]->setBindings(computeBindings.begin(), computeBindings.end());
	mComputeBindings[0]->create();
	computeBindings[0] = QRhiShaderResourceBinding::bufferLoad(0, QRhiShaderResourceBinding::ComputeStage, mParticlesBuffer[1].get());
	computeBindings[1] = QRhiShaderResourceBinding::bufferStore(1, QRhiShaderResourceBinding::ComputeStage, mParticlesBuffer[0].get());
	mComputeBindings[1]->setBindings(computeBindings.begin(), computeBindings.end());
	mComputeBindings[1]->create();
	mComputePipeline->setShaderResourceBindings(mComputeBindings[mInputIndex].get());

	mComputePipeline->setShaderStage({ QRhiShaderStage::Compute,computeUpdater });
	mComputePipeline->create();

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
	QShader matrixCompute = QRenderSystem::createShaderFromCode(QShader::ComputeStage, R"(
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
		}
	)");
	mMatrixComputePipline->setShaderStage({ QRhiShaderStage::Compute,matrixCompute });
	mMatrixComputePipline->create();
}

void QParticleSystemComponent::uploadResource(QRhiResourceUpdateBatch* batch) {
	if (!mStaticMesh || mStaticMesh->getVertices().isEmpty() || !mMaterial || !mParticleSystem || !mParticleSystem->getUpdater() || !mParticleSystem->getEmitter())
		return;
	batch->uploadStaticBuffer(mVertexBuffer.get(), mStaticMesh->getVertices().constData());
	batch->uploadStaticBuffer(mIndexBuffer.get(), mStaticMesh->getIndices().constData());
}

void QParticleSystemComponent::updatePrePass(QRhiCommandBuffer* cmdBuffer) {
	if (!mComputePipeline)
		return;

	float currentSecond = QTime::currentTime().msecsSinceStartOfDay() / 1000.0;
	mDuration = currentSecond - mLastSecond;
	mLastSecond = currentSecond;

	QVector<Asset::ParticleSystem::Particle> particles = mParticleSystem->getEmitter()->update();
	QRhiResourceUpdateBatch* u = RHI->nextResourceUpdateBatch();
	if (!particles.isEmpty()) {
		int incrememt = qMin((uint)particles.size(), Asset::ParticleSystem::PARTICLE_MAX_SIZE - mCtx.inputCounter);
		u->uploadStaticBuffer(mParticlesBuffer[mInputIndex].get(), sizeof(Asset::ParticleSystem::Particle) * mCtx.inputCounter, sizeof(Asset::ParticleSystem::Particle) * incrememt, particles.data());
		mCtx.inputCounter += incrememt;
	}

	mCtx.lifetime = mParticleSystem->getEmitter()->getLifetime();
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

void QParticleSystemComponent::updateResourcePrePass(QRhiResourceUpdateBatch* batch) {
	if (!mStaticMesh || mStaticMesh->getVertices().isEmpty() || !mMaterial || !mParticleSystem || !mParticleSystem->getUpdater() || !mParticleSystem->getEmitter())
		return;
	mMaterial->updateResource(batch);
	mParticleSystem->getUpdater()->updateResource(batch);

	UniformMatrix Matrix;
	Matrix.MVP = mEntity->calculateMatrixMVP().toGenericMatrix<4, 4>();
	Matrix.M = mEntity->calculateMatrixModel().toGenericMatrix<4, 4>();
	batch->updateDynamicBuffer(mUniformBuffer.get(), 0, sizeof(UniformMatrix), &Matrix);
}

void QParticleSystemComponent::renderInPass(QRhiCommandBuffer* cmdBuffer, const QRhiViewport& viewport) {
	if (!mPipeline)
		return;
	cmdBuffer->setGraphicsPipeline(mPipeline.get());
	cmdBuffer->setViewport(viewport);
	cmdBuffer->setShaderResources();
	QRhiCommandBuffer::VertexInput VertexInputs[] = {
		{mVertexBuffer.get(), 0},
		{mMatrixBuffer.get(),0}
	};

	cmdBuffer->setVertexInput(0, 2, VertexInputs, mIndexBuffer.get(), 0, QRhiCommandBuffer::IndexUInt32);
	cmdBuffer->drawIndexed(mStaticMesh->getIndices().size(),mCtx.inputCounter);
}

bool QParticleSystemComponent::isDefer() {
	return true;
}

