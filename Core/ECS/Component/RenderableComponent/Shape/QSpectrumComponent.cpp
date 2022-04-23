#include "QSpectrumComponent.h"

QSpectrumComponent::QSpectrumComponent() {
	auto material = std::make_shared<Asset::Material>();
	material->generateDefualtShadingCode();
	setMaterial(material);
}

void QSpectrumComponent::updatePrePass(QRhiCommandBuffer*) {
	QVector<SimpleVertex> vertices;
	const auto& bars = getBars();
	float width = bars.size() * 2 + mSpacing * (bars.size() - 1);
	float offset = -width / 2;
	for (int i = 0; i < bars.size(); i++) {
		float height = mMaxHeight * bars[i].amp;
		QVector<SimpleVertex> cube = {
			//position					normal		tangent		bitangent   texCoord			baseColor
			  SimpleVertex{QVector3D(-1.0f,-1.0f,-1.0f), QVector2D(0.0f, 1.0f)},
			  SimpleVertex{QVector3D(-1.0f,-1.0f, 1.0f) ,QVector2D(1.0f, 1.0f)},
			  SimpleVertex{QVector3D(-1.0f, height, 1.0f),QVector2D(1.0f, 0.0f)},
			  SimpleVertex{QVector3D(-1.0f, height, 1.0f),QVector2D(1.0f, 0.0f)},
			  SimpleVertex{QVector3D(-1.0f, height,-1.0f),QVector2D(0.0f, 0.0f)},
			  SimpleVertex{QVector3D(-1.0f,-1.0f,-1.0f),QVector2D(0.0f, 1.0f)},

			  SimpleVertex{QVector3D(-1.0f,-1.0f,-1.0f),QVector2D(1.0f, 1.0f)},
			  SimpleVertex{QVector3D(1.0f, height,-1.0f), QVector2D(0.0f, 0.0f)},
			  SimpleVertex{QVector3D(1.0f,-1.0f,-1.0f), QVector2D(0.0f, 1.0f)},
			  SimpleVertex{QVector3D(-1.0f,-1.0f,-1.0f),QVector2D(1.0f, 1.0f)},
			  SimpleVertex{QVector3D(-1.0f, height,-1.0f),QVector2D(1.0f, 0.0f)},
			  SimpleVertex{QVector3D(1.0f, height,-1.0f), QVector2D(0.0f, 0.0f)},

			  SimpleVertex{QVector3D(-1.0f,-1.0f,-1.0f),QVector2D(1.0f, 0.0f)},
			  SimpleVertex{QVector3D(1.0f,-1.0f,-1.0f), QVector2D(1.0f, 1.0f)},
			  SimpleVertex{QVector3D(1.0f,-1.0f, 1.0f), QVector2D(0.0f, 1.0f)},
			  SimpleVertex{QVector3D(-1.0f,-1.0f,-1.0f),QVector2D(1.0f, 0.0f)},
			  SimpleVertex{QVector3D(1.0f,-1.0f, 1.0f), QVector2D(0.0f, 1.0f)},
			  SimpleVertex{QVector3D(-1.0f,-1.0f, 1.0f),QVector2D(0.0f, 0.0f)},

			  SimpleVertex{QVector3D(-1.0f, height,-1.0f),QVector2D(1.0f, 0.0f)},
			  SimpleVertex{QVector3D(-1.0f, height, 1.0f),QVector2D(0.0f, 0.0f)},
			  SimpleVertex{QVector3D(1.0f,  height, 1.0f), QVector2D(0.0f, 1.0f)},
			  SimpleVertex{QVector3D(-1.0f, height,-1.0f),QVector2D(1.0f, 0.0f)},
			  SimpleVertex{QVector3D(1.0f,  height, 1.0f), QVector2D(0.0f, 1.0f)},
			  SimpleVertex{QVector3D(1.0f,  height,-1.0f), QVector2D(1.0f, 1.0f)},

			  SimpleVertex{QVector3D(1.0f, height,-1.0f),QVector2D(1.0f, 0.0f)},
			  SimpleVertex{QVector3D(1.0f, height, 1.0f),QVector2D(0.0f, 0.0f)},
			  SimpleVertex{QVector3D(1.0f,-1.0f, 1.0f),QVector2D(0.0f, 1.0f)},
			  SimpleVertex{QVector3D(1.0f,-1.0f, 1.0f),QVector2D(0.0f, 1.0f)},
			  SimpleVertex{QVector3D(1.0f,-1.0f,-1.0f),QVector2D(1.0f, 1.0f)},
			  SimpleVertex{QVector3D(1.0f, height,-1.0f),QVector2D(1.0f, 0.0f)},

			  SimpleVertex{QVector3D(-1.0f, height, 1.0f),QVector2D(0.0f, 0.0f)},
			  SimpleVertex{QVector3D(-1.0f,-1.0f, 1.0f),QVector2D(0.0f, 1.0f)},
			  SimpleVertex{QVector3D(1.0f,  height, 1.0f), QVector2D(1.0f, 0.0f)},
			  SimpleVertex{QVector3D(-1.0f,-1.0f, 1.0f),QVector2D(0.0f, 1.0f)},
			  SimpleVertex{QVector3D(1.0f, -1.0f, 1.0f), QVector2D(1.0f, 1.0f)},
			  SimpleVertex{QVector3D(1.0f,  height, 1.0f), QVector2D(1.0f, 0.0f)},
		};
		for (auto& vertex : cube) {
			vertex.position += QVector3D(offset, 0, 0);
		}
		offset += 2 + mSpacing;
		vertices << cube;
	}
	setSimpleVertex(vertices);
}

QBoundedInt QSpectrumComponent::getChannelIndexLimit() {
	QBoundedInt BInt(getChannelIndex(), 0, 1);
	return BInt;
}

void QSpectrumComponent::setChannelIndexLimit(QBoundedInt var) {
	setChannelIndex(var.number());
}

QBoundedInt QSpectrumComponent::getBarCountLimit() {
	QBoundedInt BInt(getBarSize(), 4, 1000);
	return BInt;
}

void QSpectrumComponent::setBarCountLimit(QBoundedInt var) {
	setBarSize(var.number());
}

QRange QSpectrumComponent::getFrequencyRangeLimit() {
	QRange range(getLowFreq(), getHighFreq(), 0, 48000);
	return range;
}

void QSpectrumComponent::setFrequencyRangeLimit(QRange var) {
	setLowFreq(var.getLower());
	setHighFreq(var.getUpper());
}

QBoundedDouble QSpectrumComponent::getSmoothRangeFactorLimit() {
	return QBoundedDouble(getSmoothFactorRange(), 0, 1);
}

void QSpectrumComponent::setSmoothRangeFactorLimit(QBoundedDouble var) {
	setSmoothFactorRange(var.number());
}

QBoundedDouble QSpectrumComponent::getSmoothFactorRiseLimit() {
	return QBoundedDouble(getSmoothFactorRise(), 0, 1);
}

void QSpectrumComponent::setSmoothFactorRiseLimit(QBoundedDouble var) {
	setSmoothFactorRise(var.number());
}

QBoundedDouble QSpectrumComponent::getSmoothFactorFallLimit() {
	return QBoundedDouble(getSmoothFactorFall(), 0, 1);
}

void QSpectrumComponent::setSmoothFactorFallLimit(QBoundedDouble var) {
	setSmoothFactorFall(var.number());
}

