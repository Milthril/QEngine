#include "QSceneComponent.h"
#include "imgui.h"
#include "ImGuizmo.h"
#include "Scene/QScene.h"

QScene* QSceneComponent::getScene() const
{
	return mScene;
}

void QSceneComponent::setScene(QScene* val)
{
	mScene = val;
}

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

QMatrix4x4 QSceneComponent::calculateModelMatrix()
{
	return calculateParentMatrix() * calculateLocalMatrix();
}

QMatrix4x4 QSceneComponent::calculateMVP()
{
	return mScene->getMatrixVP() * calculateModelMatrix();
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
	bNeedAddChild.active();
}

void QSceneComponent::removeChild(std::shared_ptr<QSceneComponent> child)
{
	child->mParent = nullptr;
	mChildren.removeOne(child);
	bNeedRemove.active();
}

void QSceneComponent::clear()
{
	for (auto& child : mChildren)
		child->mParent = nullptr;
	mChildren.clear();
}