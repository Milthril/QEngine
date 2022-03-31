#include "QScene.h"
#include "Component\QPrimitiveComponent.h"
#include "Component\Camera\QCameraComponent.h"
#include "QQueue"

void QScene::insertLight(uint32_t index, std::shared_ptr<QLightComponent> light)
{
	mLightList.insert(index, light);
	Q_EMIT lightChanged();
}

void QScene::removeLight(std::shared_ptr<QLightComponent> light)
{
	if (mLightList.removeOne(light)) {
		Q_EMIT lightChanged();
	}
}

void QScene::insertPrimitive(uint32_t index, const QString& name, std::shared_ptr<QPrimitiveComponent> component)
{
	component->setObjectName(name);
	mPrimitiveList.insert(index, component);
	Q_EMIT primitiveInserted(index, component);
}

void QScene::removePrimitive(std::shared_ptr<QPrimitiveComponent> component)
{
	if (mPrimitiveList.removeOne(component)) {
		Q_EMIT primitiveRemoved(component);
	}
}

void QScene::addLight(std::shared_ptr<QLightComponent> light)
{
	insertLight(mLightList.size(), light);
}

void QScene::addPrimitive(const QString& name, std::shared_ptr<QPrimitiveComponent> component)
{
	insertPrimitive(mPrimitiveList.size(), name, component);
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