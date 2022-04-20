#ifndef QCameraComponent_h__
#define QCameraComponent_h__

#include <QWindow>
#include "IComponent.h"
#include "QQuaternion"
#include "QMatrix4x4"

class QCameraComponent :public IComponent {
	Q_OBJECT
		Q_COMPONENT(QCameraComponent)
public:
	QCameraComponent();
	virtual QMatrix4x4 getViewMatrix();
	float getYaw();
	float getPitch();
	float getRoll();

	void setPosition(const QVector3D& newPosition);
	void setRotation(const QVector3D& newRotation);

	void setAspectRatio(float val);

	QMatrix4x4 getMatrixVPWithCorr();
	QMatrix4x4 getMatrixClip();
	QMatrix4x4 getMatrixView();

	void setupWindow(QWindow* window);

private:
	void calculateViewMatrix();
	void calculateClipMatrix();
	void calculateCameraDirection();
	bool eventFilter(QObject* watched, QEvent* event) override;
public:
private:
	QMatrix4x4 mViewMatrix;
	QMatrix4x4 mClipMatrix;
	QWindow* mWindow;

	float mFov = 45.0f;
	float mAspectRatio = 1.0;
	float mNearPlane = 0.1f;
	float mFarPlane = 2000.0f;

	QVector3D mCameraDirection;
	QVector3D mCameraUp;
	QVector3D mCameraRight;

	QSet<int> mKeySet;					     //记录当前被按下按键的集合
	int64_t mDeltaTimeMs = 0;						 //当前帧与上一帧的时间差
	int64_t mLastFrameTimeMs = 0;					 //上一帧的时间
	float mRotationSpeed = 0.003f;			 //鼠标灵敏度
	float mMoveSpeed = 0.1f;				 //控制移动速度
};

#endif // QCameraComponent_h__
