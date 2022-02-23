#include "QCameraComponent.h"

QMatrix4x4 QCameraComponent::getViewMatrix()
{
	return mViewMatrix;
}

const float& QCameraComponent::getYaw()
{
	return getRotation().y();
}

const float& QCameraComponent::getPitch()
{
	return getRotation().x();
}

const float& QCameraComponent::getRoll()
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
	calculateViewMatrix();
}

void QCameraComponent::calculateViewMatrix()
{
	QVector3D cameraDirection;
	cameraDirection.setX(cos(getYaw()) * cos(getPitch()));
	cameraDirection.setY(sin(getPitch()));
	cameraDirection.setZ(sin(getYaw()) * cos(getPitch()));
	mViewMatrix.setToIdentity();
	mViewMatrix.lookAt(position(), position() + cameraDirection, cameraDirection);
}