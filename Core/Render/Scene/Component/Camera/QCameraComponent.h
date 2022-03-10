#ifndef QCameraComponent_h__
#define QCameraComponent_h__

#include "Render/Scene/QSceneComponent.h"
#include <QWindow>

class QCameraComponent :public QSceneComponent {
public:
	virtual QMatrix4x4 getViewMatrix();
	float getYaw();
	float getPitch();
	float getRoll();

	virtual void setPosition(const QVector3D& newPosition) override;
	virtual void setRotation(const QVector3D& newRotation) override;

	void setAspectRatio(float val);

	QMatrix4x4 getMatrixVP();
	QMatrix4x4 getMatrixClip();
	QMatrix4x4 getMatrixView();

	void setupWindow(QWindow* window);

private:
	void calculateViewMatrix();
	void calculateClipMatrix();
	void calculateCameraDirection();
	bool eventFilter(QObject* watched, QEvent* event) override;
public:
	QSceneComponent::ProxyType type() override { return QSceneComponent::ProxyType::Camera; }
private:
	QMatrix4x4 mViewMatrix;
	QMatrix4x4 mClipMatrix;
	QWindow* mWindow;

	float mFov = 45.0f;
	float mAspectRatio = 1.0;
	float mNearPlane = 0.01f;
	float mFarPlane = 10000.0f;

	QVector3D mCameraDirection;
	QVector3D mCameraUp;
	QVector3D mCameraRight;

	QSet<int> mKeySet;			 //记录当前被按下按键的集合
	float mDeltaTimeMs;			 //当前帧与上一帧的时间差
	float mLastFrameTimeMs;			 //上一帧的时间
	float mRotationSpeed = 0.005f;						//鼠标灵敏度
	float mMoveSpeed = 0.05f;									//控制移动速度
};

#endif // QCameraComponent_h__
