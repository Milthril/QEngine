#include "QParticleComponent.h"
#include "Scene/Component/StaticMesh/QCube.h"

QParticleComponent::QParticleComponent()
{
	mStaticMesh = std::make_shared<QCube>();
	mParticleSystem->getUpdater()->addRef(this);
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