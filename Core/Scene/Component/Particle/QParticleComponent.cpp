#include "QParticleComponent.h"
#include "Scene/Component/StaticMesh/QCube.h"

QString QParticleComponent::getParticleDefine() {
	return  R"(
#extension GL_ARB_separate_shader_objects : enable
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
)";
}

QParticleComponent::QParticleComponent()
{
	mStaticMesh = std::make_shared<QCube>();
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
	if (mStaticMesh)
		mStaticMesh->setPosition(newPosition);
}

void QParticleComponent::setRotation(const QVector3D& newRotation)
{
	QPrimitiveComponent::setRotation(newRotation);
	if (mStaticMesh)
		mStaticMesh->setRotation(newRotation);
}

void QParticleComponent::setScale(const QVector3D& newScale)
{
	QPrimitiveComponent::setScale(newScale);
	if (mStaticMesh)
		mStaticMesh->setScale(newScale);
}