#include "QScene.h"
#include "Component\QPrimitiveComponent.h"
#include "Component\Camera\QCameraComponent.h"
#include "Component\SkyBox\QSkyBoxComponent.h"
#include "QQueue"

void QScene::addSceneComponent(QString name, std::shared_ptr<QSceneComponent> comp)
{
	switch (comp->type())
	{
	case QSceneComponent::Camera: {
		mCamera = std::dynamic_pointer_cast<QCameraComponent>(comp);
		break;
	}
	case QSceneComponent::StaticMesh: {
		mPrimitiveList << std::dynamic_pointer_cast<QPrimitiveComponent>(comp);
		break;
	}
	case QSceneComponent::SkeletonMesh: {
		mPrimitiveList << std::dynamic_pointer_cast<QPrimitiveComponent>(comp);
		break;
	}
	case QSceneComponent::Light: {
		//mLightList << std::dynamic_pointer_cast<QLightComponent>(comp);
		break;
	}
	case QSceneComponent::Particle: {
		mPrimitiveList << std::dynamic_pointer_cast<QPrimitiveComponent>(comp);;
		break;
	}
	case QSceneComponent::SkyBox: {
		if (mSkyBox == nullptr) {
			mPrimitiveList << std::dynamic_pointer_cast<QPrimitiveComponent>(comp);
			mSkyBox = std::dynamic_pointer_cast<QSkyBoxComponent>(comp);
		}
		else {
			Q_ASSERT(false);
		}
		break;
	}
	default:
		break;;
	}
	mSceneCompList << comp;
	comp->setObjectName(name);
	comp->setScene(this);
	for (auto& child : comp->getChildren()) {
		addSceneComponent(child->objectName(), child);
	}
}

void QScene::removeSceneComponent(std::shared_ptr<QSceneComponent> comp)
{
}

std::shared_ptr<QCameraComponent> QScene::getCamera() const
{
	return mCamera;
}

void QScene::setCamera(std::shared_ptr<QCameraComponent> val)
{
	mCamera = val;
}

std::shared_ptr<QSkyBoxComponent> QScene::getSkyBox() const
{
	return mSkyBox;
}

void QScene::setSkyBox(std::shared_ptr<QSkyBoxComponent> val)
{
	mSkyBox = val;
	Q_EMIT skyBoxChanged();
}

std::shared_ptr<QSceneComponent> QScene::searchCompById(QSceneComponent::ComponentId id)
{
	QQueue<std::shared_ptr<QSceneComponent>> qObject;
	for (auto comp : mPrimitiveList) {
		qObject.push_back(comp);
	}
	while (!qObject.isEmpty()) {
		std::shared_ptr<QSceneComponent> parent = qObject.takeFirst();
		if (parent->componentId() == id) {
			return parent;
		}
		for (auto child : parent->getChildren()) {
			qObject.push_back(child);
		}
	}
	return nullptr;
}

QMatrix4x4 QScene::getMatrixVP()
{
	if (mCamera)
		return mCamera->getMatrixVP();
	return QMatrix4x4();
}