#include "QDirectionLightComponent.h"

QDirectionLightComponent::QDirectionLightComponent() {
	addDataVec3("Direction", QVector3D(1, 0, 1));
}

QVector3D QDirectionLightComponent::getDirection() {
	return getData<QVector3D>("Direction");
}

void QDirectionLightComponent::setDirection(QVector3D vec) {
	setData<QVector3D>("Direction", vec);
}

QByteArray QDirectionLightComponent::getLightingCode() {
	throw std::logic_error("The method or operation is not implemented.");
}
