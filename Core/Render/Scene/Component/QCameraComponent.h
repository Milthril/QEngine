#ifndef QCameraComponent_h__
#define QCameraComponent_h__

#include "Render/Scene/QSceneComponent.h"

class QCameraComponent :public QSceneComponent {
public:
	virtual QMatrix4x4 getViewMatrix();
	const float& getYaw();
	const float& getPitch();
	const float& getRoll();
	void setPosition(const QVector3D& newPosition) override;
	void setRotation(const QVector3D& newRotation) override;
private:
	void calculateViewMatrix();
public:
	QSceneComponent::Type type() override {
		return QSceneComponent::Type::Camera;
	}
private:
	QMatrix4x4 mViewMatrix;
};

#endif // QCameraComponent_h__
