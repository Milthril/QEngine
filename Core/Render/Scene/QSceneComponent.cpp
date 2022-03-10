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

QMatrix4x4 QSceneComponent::calculateWorldMatrix()
{
	QMatrix4x4 matrix = calculateModelMatrix();
	QSceneComponent* parent = mParent;
	while (parent != nullptr) {
		matrix = parent->calculateModelMatrix() * matrix;
		parent = parent->mParent;
	}
	return matrix;
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

void QSceneComponent::addChild(std::shared_ptr<QSceneComponent> child)
{
	child->mParent = this;
	mChildren << child;
}

void QSceneComponent::removeChild(std::shared_ptr<QSceneComponent> child)
{
	child->mParent = nullptr;
	mChildren.removeOne(child);
}

void QSceneComponent::clear()
{
	for (auto& child : mChildren)
		child->mParent = nullptr;
	mChildren.clear();
}