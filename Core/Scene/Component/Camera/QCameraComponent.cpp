#include "QCameraComponent.h"
#include "qevent.h"
#include "QDateTime"
#include "QApplication"

QMatrix4x4 QCameraComponent::getViewMatrix()
{
	return mViewMatrix;
}

float QCameraComponent::getYaw()
{
	return getRotation().y();
}

float QCameraComponent::getPitch()
{
	return getRotation().x();
}

float QCameraComponent::getRoll()
{
	return getRotation().z();
}

void QCameraComponent::setPosition(const QVector3D& newPosition)
{
	QSceneComponent::setPosition(newPosition);
	calculateViewMatrix();
}

void QCameraComponent::setRotation(const QVector3D& newRotation)
{
	QSceneComponent::setRotation(newRotation);
	calculateCameraDirection();
	calculateViewMatrix();
}

void QCameraComponent::setAspectRatio(float val)
{
	mAspectRatio = val;
	calculateClipMatrix();
}

QMatrix4x4 QCameraComponent::getMatrixVP()
{
	return mClipMatrix * mViewMatrix;
}

QMatrix4x4 QCameraComponent::getMatrixClip()
{
	return mClipMatrix;
}

QMatrix4x4 QCameraComponent::getMatrixView()
{
	return mViewMatrix;
}

void QCameraComponent::setupWindow(QWindow* window)
{
	mWindow = window;
	if (mWindow) {
		mWindow->installEventFilter(this);
		setAspectRatio(mWindow->width() / (float)mWindow->height());
	}
}

bool QCameraComponent::eventFilter(QObject* watched, QEvent* event)
{
	if (watched != nullptr && watched == mWindow) {
		switch (event->type())
		{
		case QEvent::Paint:
		case QEvent::Resize:
			setAspectRatio(mWindow->width() / (float)mWindow->height());
			break;
		case QEvent::MouseButtonPress:
			mWindow->setCursor(Qt::BlankCursor);             //隐藏鼠标光标
			QCursor::setPos(mWindow->mapToGlobal(QPoint(mWindow->width() / 2, mWindow->height() / 2)));   //将鼠标移动窗口中央
			break;
		case QEvent::MouseButtonRelease:
			mWindow->setCursor(Qt::ArrowCursor);             //显示鼠标光标
			break;

		case QEvent::MouseMove: {
			if (qApp->mouseButtons() & Qt::LeftButton) {
				QRect rect(0, 0, mWindow->width(), mWindow->height());
				QMouseEvent* mouseEvent = static_cast<QMouseEvent*>(event);
				float xoffset = mouseEvent->x() - rect.center().x();
				float yoffset = mouseEvent->y() - rect.center().y(); // 注意这里是相反的，因为y坐标是从底部往顶部依次增大的
				xoffset *= mRotationSpeed;
				yoffset *= mRotationSpeed;
				float yaw = getYaw() - xoffset;
				float pitch = getPitch() - yoffset;

				if (pitch > 1.55f)         //将俯视角限制到[-89°,89°]，89°约等于1.55
					pitch = 1.55f;
				if (pitch < -1.55f)
					pitch = -1.55f;

				QVector3D rotation = getRotation();
				rotation.setY(yaw);
				rotation.setX(pitch);
				setRotation(rotation);
				QCursor::setPos(mWindow->mapToGlobal(QPoint(mWindow->width() / 2, mWindow->height() / 2)));   //将鼠标移动窗口中央
			}
			break;
		}
		case QEvent::KeyPress: {
			QKeyEvent* keyEvent = static_cast<QKeyEvent*>(event);
			mKeySet.insert(keyEvent->key());
			if (keyEvent->key() == Qt::Key_Escape) {
				mWindow->close();
			}
			break;
		}
		case QEvent::KeyRelease: {
			QKeyEvent* keyEvent = static_cast<QKeyEvent*>(event);
			mKeySet.remove(keyEvent->key());
			break;
		}
		case QEvent::UpdateRequest: {
			float time = QTime::currentTime().msecsSinceStartOfDay() / 1000.0;
			mDeltaTimeMs = time - mLastFrameTimeMs;                           //在此处更新时间差
			mLastFrameTimeMs = time;
			if (!mKeySet.isEmpty()) {
				QVector3D position = getPosition();
				if (mKeySet.contains(Qt::Key_W))                           //前
					position += mMoveSpeed * mCameraDirection;
				if (mKeySet.contains(Qt::Key_S))                           //后
					position -= mMoveSpeed * mCameraDirection;
				if (mKeySet.contains(Qt::Key_A))                           //左
					position -= QVector3D::crossProduct(mCameraDirection, mCameraUp) * mMoveSpeed;
				if (mKeySet.contains(Qt::Key_D))                           //右
					position += QVector3D::crossProduct(mCameraDirection, mCameraUp) * mMoveSpeed;
				if (mKeySet.contains(Qt::Key_Space))                       //上浮
					position.setY(position.y() + mMoveSpeed);
				if (mKeySet.contains(Qt::Key_Shift))                       //下沉
					position.setY(position.y() - mMoveSpeed);
				setPosition(position);
			}
			break;
		}
		case QEvent::WindowActivate: {
			break;
		}
		case  QEvent::WindowDeactivate: {
			mKeySet.clear();
			break;
		}

		case  QEvent::Close: {
			mKeySet.clear();
			break;
		}
		default:
			break;
		}
	}
	return QObject::eventFilter(watched, event);
}

void QCameraComponent::calculateViewMatrix()
{
	mViewMatrix.setToIdentity();
	mViewMatrix.lookAt(getPosition(), getPosition() + mCameraDirection, mCameraUp);
}

void QCameraComponent::calculateClipMatrix()
{
	mClipMatrix.setToIdentity();
	mClipMatrix.perspective(mFov, mAspectRatio, mNearPlane, mFarPlane);
}

void QCameraComponent::calculateCameraDirection()
{
	mCameraDirection.setX(cos(getYaw()) * cos(getPitch()));
	mCameraDirection.setY(sin(getPitch()));
	mCameraDirection.setZ(sin(getYaw()) * cos(getPitch()));
	mCameraRight = QVector3D::crossProduct({ 0.0f,1.0f,0.0f }, mCameraDirection);
	mCameraUp = QVector3D::crossProduct(mCameraRight, mCameraDirection);         //摄像机上向量
}