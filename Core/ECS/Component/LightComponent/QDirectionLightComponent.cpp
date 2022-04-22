#include "QDirectionLightComponent.h"

QDirectionLightComponent::QDirectionLightComponent() {
	setDataVec3("Direction", QVector3D(1, 0, 1));
	setDataVec3("Radiance", QVector3D(1, 1, 1));
	if (bNeedRecreate.receive()) {
		recreateResource();
	}
}

QVector3D QDirectionLightComponent::getDirection() {
	return getData<QVector3D>("Direction");
}

void QDirectionLightComponent::setDirection(QVector3D vec) {
	setData<QVector3D>("Direction", vec);
}

QVector3D QDirectionLightComponent::getRadiance() {
	return getData<QVector3D>("Radiance");
}

void QDirectionLightComponent::setRadiance(QVector3D vec) {
	setData<QVector3D>("Radiance", vec);
}

QByteArray QDirectionLightComponent::getLightingCode(QString lightBlockName) {
	return QString(R"({	
		vec3 Li = - %1.Direction;
		vec3 Lradiance = %1.Radiance;
		vec3 Lh = normalize(Li + Lo);

		float cosLi = max(0.0, dot(N, Li));
		float cosLh = max(0.0, dot(N, Lh));

		vec3 F  = fresnelSchlick(F0, max(0.0, dot(Lh, Lo)));
		float D = ndfGGX(cosLh, roughness);
		float G = gaSchlickGGX(cosLi, cosLo, roughness);

		vec3 kd = mix(vec3(1.0) - F, vec3(0.0), metalness);

		vec3 diffuseBRDF = kd * baseColor;

		vec3 specularBRDF = (F * D * G) / max(Epsilon, 4.0 * cosLi * cosLo);

		directLighting += (diffuseBRDF + specularBRDF) * Lradiance * cosLi;
	})").arg(lightBlockName).toLocal8Bit();
}
