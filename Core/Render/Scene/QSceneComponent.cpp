#include "QSceneComponent.h"

const QVector3D& QSceneComponent::position() const
{
	return mPosition;
}

void QSceneComponent::setPosition(const QVector3D& newPosition)
{
	if (mPosition == newPosition)
		return;
	mPosition = newPosition;
}

const QVector3D& QSceneComponent::rotation() const
{
	return mRotation;
}

void QSceneComponent::setRotation(const QVector3D& newRotation)
{
	if (mRotation == newRotation)
		return;
	mRotation = newRotation;
}

const QVector3D& QSceneComponent::scale() const
{
	return mScale;
}

void QSceneComponent::setScale(const QVector3D& newScale)
{
	if (mScale == newScale)
		return;
	mScale = newScale;
}