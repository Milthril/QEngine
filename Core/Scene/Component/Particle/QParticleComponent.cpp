#include "QParticleComponent.h"
#include "Scene/Component/StaticMesh/QCube.h"

QParticleComponent::QParticleComponent()
{
	setStaticMesh(std::make_shared<QCube>());
	mParticleSystem->getUpdater()->addRef(this);
}

void QParticleComponent::setStaticMesh(QSubClass<QStaticMeshComponent> val)
{
	if (!val)
		return;
	if (mStaticMesh) {
		val->setMaterial(mStaticMesh->getMaterial());		// 转移材质
		val->getMaterial()->bNeedRecreate.active();
		mStaticMesh->getMaterial()->removeRef(this);
	}
	mStaticMesh = val;
	mStaticMesh->getMaterial()->addRef(this);
	mStaticMesh->bNeedRecreateResource.active();
	mStaticMesh->bNeedRecreatePipeline.active();
	bNeedRecreateResource.active();
	bNeedRecreatePipeline.active();
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