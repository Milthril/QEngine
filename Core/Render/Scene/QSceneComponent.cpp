#include "QSceneComponent.h"

QMatrix4x4 QSceneComponent::calculateModelMatrix()
{
	QMatrix4x4 modelMatrix;
	modelMatrix.translate(mPosition);
	modelMatrix.rotate(mRotation.x(), 1, 0, 0);
	modelMatrix.rotate(mRotation.y(), 0, 1, 0);
	modelMatrix.rotate(mRotation.z(), 0, 0, 1);
	modelMatrix.scale(mScale);
	return modelMatrix;
}

const QVector3D& QSceneComponent::getPosition() const
{
	return mPosition;
}

void QSceneComponent::setPosition(const QVector3D& newPosition)
{
	if (mPosition == newPosition)
		return;
	mPosition = newPosition;
}

const QVector3D& QSceneComponent::getRotation() const
{
	return mRotation;
}

void QSceneComponent::setRotation(const QVector3D& newRotation)
{
	if (mRotation == newRotation)
		return;
	mRotation = newRotation;
}

const QVector3D& QSceneComponent::getScale() const
{
	return mScale;
}

void QSceneComponent::setScale(const QVector3D& newScale)
{
	if (mScale == newScale)
		return;
	mScale = newScale;
}