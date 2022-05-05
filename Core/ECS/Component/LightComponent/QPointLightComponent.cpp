#include "QPointLightComponent.h"
#include "ECS/QEntity.h"

QPointLightComponent::QPointLightComponent() {
	setDataVec3("Position", QVector3D(0, 0, 0));
	setDataVec3("Radiance", QVector3D(100, 100, 100));
	setDistance(1000);
	if (bNeedRecreate.receive()) {
		recreateResource();
	}
}

QVector3D QPointLightComponent::getRadiance() {
	return getData<QVector3D>("Radiance");
}

void QPointLightComponent::setRadiance(QVector3D vec) {
	setData<QVector3D>("Radiance", vec);
}

float QPointLightComponent::getDistance() {
	return mDistance;
}

void QPointLightComponent::setDistance(float var) {
	mDistance = var;
	if (mDistance < 7) {
		setDataFloat("Constant", 1.0);
		setDataFloat("Linear", 0.7);
		setDataFloat("Quadratic", 1.8);
	}
	else if (mDistance < 13) {
		setDataFloat("Constant", 1.0);
		setDataFloat("Linear", 0.35);
		setDataFloat("Quadratic", 0.44);
	}
	else if (mDistance < 20) {
		setDataFloat("Constant", 1.0);
		setDataFloat("Linear", 0.22);
		setDataFloat("Quadratic", 0.2);
	}
	else if (mDistance < 32) {
		setDataFloat("Constant", 1.0);
		setDataFloat("Linear", 0.14);
		setDataFloat("Quadratic", 0.07);
	}
	else if (mDistance < 50) {
		setDataFloat("Constant", 1.0);
		setDataFloat("Linear", 0.09);
		setDataFloat("Quadratic", 0.032);
	}
	else if (mDistance < 65) {
		setDataFloat("Constant", 1.0);
		setDataFloat("Linear", 0.07);
		setDataFloat("Quadratic", 0.017);
	}
	else if (mDistance < 100) {
		setDataFloat("Constant", 1.0);
		setDataFloat("Linear", 0.045);
		setDataFloat("Quadratic", 0.0075);
	}
	else if (mDistance < 160) {
		setDataFloat("Constant", 1.0);
		setDataFloat("Linear", 0.027);
		setDataFloat("Quadratic", 0.0028);
	}
	else if (mDistance < 200) {
		setDataFloat("Constant", 1.0);
		setDataFloat("Linear", 0.022);
		setDataFloat("Quadratic", 0.0019);
	}
	else if (mDistance < 325) {
		setDataFloat("Constant", 1.0);
		setDataFloat("Linear", 0.014);
		setDataFloat("Quadratic", 0.0007);
	}
	else if (mDistance < 600) {
		setDataFloat("Constant", 1.0);
		setDataFloat("Linear", 0.007);
		setDataFloat("Quadratic", 0.0002);
	}
	else{
		setDataFloat("Constant", 1.0);
		setDataFloat("Linear", 0.0014);
		setDataFloat("Quadratic", 0.000007);
	}
}

void QPointLightComponent::updateResource(QRhiResourceUpdateBatch* batch) {
	setDataVec3("Position", mEntity->getTransformComponent()->getPosition());
	QRhiUniform::updateResource(batch);
}

QByteArray QPointLightComponent::getLightingCode(QString lightBlockName) {
	return QString(R"({	

		vec3 Li = - normalize(position - %1.Position);
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

		float distance = length(position - %1.Position);
		float attenuation = 1.0 / (%1.Constant + %1.Linear * distance +  %1.Quadratic * (distance * distance));

		directLighting += (diffuseBRDF + specularBRDF) * Lradiance * cosLi * attenuation;
	})").arg(lightBlockName).toLocal8Bit();
}
