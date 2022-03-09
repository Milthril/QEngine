#include "QScene.h"

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

void QScene::insertPrimitive(uint32_t index, std::shared_ptr<QPrimitiveComponent> component)
{
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

void QScene::addPrimitive(std::shared_ptr<QPrimitiveComponent> component)
{
	insertPrimitive(mPrimitiveList.size(), component);
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