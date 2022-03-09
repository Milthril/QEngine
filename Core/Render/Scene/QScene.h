#ifndef QScene_h__
#define QScene_h__

#include <QObject>
#include "QSceneComponent.h"

class QLightComponent;
class QCameraComponent;
class QSkyBoxComponent;
class QPrimitiveComponent;

class QScene :public QObject {
	Q_OBJECT
		friend class QSceneRenderer;
public:
	void insertLight(uint32_t index, std::shared_ptr<QLightComponent> light);
	void removeLight(std::shared_ptr<QLightComponent> light);

	void insertPrimitive(uint32_t index, std::shared_ptr<QPrimitiveComponent> component);
	void removePrimitive(std::shared_ptr<QPrimitiveComponent> component);

	void addLight(std::shared_ptr<QLightComponent> light);
	void addPrimitive(std::shared_ptr<QPrimitiveComponent> component);

	std::shared_ptr<QCameraComponent> getCamera() const;
	void setCamera(std::shared_ptr<QCameraComponent> val);

	std::shared_ptr<QSkyBoxComponent> getSkyBox() const;
	void setSkyBox(std::shared_ptr<QSkyBoxComponent> val);
private:
	QList<std::shared_ptr<QPrimitiveComponent>> mPrimitiveList;
	QList<std::shared_ptr<QLightComponent>> mLightList;
	std::shared_ptr<QSkyBoxComponent> mSkyBox;
	std::shared_ptr<QCameraComponent> mCamera;

Q_SIGNALS:
	void lightChanged();
	void skyBoxChanged();
	void primitiveInserted(int index, std::shared_ptr<QPrimitiveComponent>);
	void primitiveRemoved(std::shared_ptr<QPrimitiveComponent>);
};

#endif // QScene_h__