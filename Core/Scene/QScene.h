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
		friend class ISceneRenderer;
public:
	void addSceneComponent(QString name, std::shared_ptr<QSceneComponent> comp);
	void removeSceneComponent(std::shared_ptr<QSceneComponent> comp);

	std::shared_ptr<QCameraComponent> getCamera() const;
	void setCamera(std::shared_ptr<QCameraComponent> val);
	std::shared_ptr<QSkyBoxComponent> getSkyBox() const;
	void setSkyBox(std::shared_ptr<QSkyBoxComponent> val);
	std::shared_ptr<QSceneComponent> searchCompById(QSceneComponent::ComponentId id);
	QMatrix4x4 getMatrixVP();

	const QList<std::shared_ptr<QPrimitiveComponent>>& geyPrimitiveList() const { return mPrimitiveList; }
private:
	QList<std::shared_ptr<QSceneComponent>> mSceneCompList;

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