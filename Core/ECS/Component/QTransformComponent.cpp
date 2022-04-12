#include "QTransformComponent.h"
#include "imgui.h"
#include "ImGuizmo.h"

QMatrix4x4 QTransformComponent::calculateMatrix() {
	QMatrix4x4 modelMatrix;
	ImGuizmo::RecomposeMatrixFromComponents((float*)&mPosition, (float*)&mRotation, (float*)&mScale, modelMatrix.data());
	return modelMatrix;
}

