#include "QParticleComponent.h"
#include "CustomShape\QCube.h"

const char* UpdateShaderHeader = R"(
#version 450
#extension GL_ARB_separate_shader_objects : enable
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

layout(std140,binding = 0)  buffer InputParticle{
    Particle intputParticles[PARTICLE_MAX_SIZE];
};

layout(std140,binding = 1) buffer OutputParticle{
    Particle outputParticles[PARTICLE_MAX_SIZE];
};

void main(){
    const uint index = gl_GlobalInvocationID.x ;								//根据工作单元的位置换算出内存上的索引
    outputParticles[index].life = intputParticles[index].life + 0.01;
    outputParticles[index].position = intputParticles[index].position + intputParticles[index].velocity;
    outputParticles[index].velocity = intputParticles[index].velocity ;
    outputParticles[index].scaling = intputParticles[index].scaling;
    outputParticles[index].rotation = intputParticles[index].rotation;
}
)";

QParticleComponent::QParticleComponent()
{
	setUpdater(UpdateShaderHeader);
	auto it = std::make_shared<QCube>();
	mShape = it;
}

QByteArray QParticleComponent::getUpdater() const
{
	return mUpdater;
}

void QParticleComponent::setUpdater(QByteArray val)
{
	mUpdater = val;
}

void QParticleComponent::createPartilces(const QVector<Particle>& particles)
{
	mParticlesPool = particles;
}

QVector<QParticleComponent::Particle> QParticleComponent::takeParticles()
{
	return mParticlesPool;
}

void QParticleComponent::setPosition(const QVector3D& newPosition)
{
	QPrimitiveComponent::setPosition(newPosition);
	if (mShape)
		mShape->setPosition(newPosition);
}

void QParticleComponent::setRotation(const QVector3D& newRotation)
{
	QPrimitiveComponent::setRotation(newRotation);
	if (mShape)
		mShape->setRotation(newRotation);
}

void QParticleComponent::setScale(const QVector3D& newScale)
{
	QPrimitiveComponent::setScale(newScale);
	if (mShape)
		mShape->setScale(newScale);
}