#include "QSceneComponent.h"
#include "imgui.h"
#include "ImGuizmo.h"

QMatrix4x4 QSceneComponent::calculateLocalMatrix()
{
	QMatrix4x4 modelMatrix;
	ImGuizmo::RecomposeMatrixFromComponents((float*)&mPosition, (float*)&mRotation, (float*)&mScale, modelMatrix.data());
	return modelMatrix;
}

QMatrix4x4 QSceneComponent::calculateParentMatrix()
{
	QMatrix4x4 matrix;
	QSceneComponent* parent = mParent;
	while (parent != nullptr) {
		matrix = parent->calculateLocalMatrix() * matrix;
		parent = parent->mParent;
	}
	return matrix;
}

QMatrix4x4 QSceneComponent::calculateWorldMatrix()
{
	return calculateParentMatrix() * calculateLocalMatrix();
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