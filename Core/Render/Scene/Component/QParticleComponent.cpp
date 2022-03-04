#include "QParticleComponent.h"

const char* UpdateShaderHeader = R"(
#version 450
#extension GL_ARB_separate_shader_objects : enable
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

layout(std140,binding = 0)  buffer InputParticle{
    int inputCounter;
    Particle intputParticles[PARTICLE_MAX_SIZE];
};

layout(std140,binding = 1) buffer OutputParticle{
    int outputCounter;
    Particle outputParticles[PARTICLE_MAX_SIZE];
};

void main(){
    const uint srcIndex = gl_GlobalInvocationID.x ;      //根据工作单元的位置换算出内存上的索引
    const int key0 = 1 - int(step(inputCounter,srcIndex));
    const int key1 = 1 - int(step(5.0,intputParticles[srcIndex].life));
                         //顶点计数
    const uint dstIndex = atomicAdd(outputCounter,min(key0,key1));
    outputParticles[dstIndex].life = intputParticles[srcIndex].life + 0.01;     //填充到新的顶点索引
    outputParticles[dstIndex].position = intputParticles[srcIndex].position + intputParticles[srcIndex].velocity;
    outputParticles[dstIndex].velocity = intputParticles[srcIndex].velocity + vec3(0,0.00003,0) ;
}
)";

QParticleComponent::QParticleComponent()
{
	setUpdater(UpdateShaderHeader);
	auto it = std::make_shared<QCube>();
	it->setDefaultBaseColor(QVector4D(1, 0, 0, 1));
	mShape = it;
}

QByteArray QParticleComponent::getUpdater() const
{
	return mUpdater;
}

void QParticleComponent::setUpdater(QByteArray val)
{
	mUpdater = val;
	bNeedRecreatePipeline = true;
}

void QParticleComponent::createPartilces(const QVector<Particle>& particles)
{
	mParticlesPool = particles;
}

QVector<QParticleComponent::Particle> QParticleComponent::takeParticles()
{
	QVector<QParticleComponent::Particle> p;
	p << QParticleComponent::Particle();
	return p;
}