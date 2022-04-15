#include "QTransformComponent.h"
#include "imgui.h"
#include "ImGuizmo.h"

void QTransformComponent::setMatrix(QMatrix4x4 matrix) {
	ImGuizmo::DecomposeMatrixToComponents(matrix.data(),(float*)&mPosition, (float*)&mRotation, (float*)&mScale);
}

QMatrix4x4 QTransformComponent::calculateMatrix() {
	QMatrix4x4 modelMatrix;
	ImGuizmo::RecomposeMatrixFromComponents((float*)&mPosition, (float*)&mRotation, (float*)&mScale, modelMatrix.data());
	return modelMatrix;
}

