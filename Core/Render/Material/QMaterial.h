#ifndef QMaterial_h__
#define QMaterial_h__
#include "private\qrhi_p.h"

class QMaterial {
	friend class QMaterialProxy;
public:
	QByteArray getShadingCode() const { return mShadingCode; }
	void setShadingCode(QByteArray val) { mShadingCode = val; }
protected:
	struct QParamDesc {
		QString name;
		uint32_t offsetInByte;
		uint32_t sizeInByte;
		uint32_t sizeInByteAligned;
		int typeId;
		bool needUpdate = false;
	};

	struct QTextureDesc {
		QString name;
		QImage image;
		bool needUpdate = false;
	};

	template<typename _Ty>
	void setParam(const QString& name, const _Ty& data) {
		auto paramDescIter = getParamDesc(name);
		if (paramDescIter != mParams.end()) {
			Q_ASSERT(sizeof(data) == paramDescIter->sizeInByte);
			memcpy(mData.data() + paramDescIter->offsetInByte, &data, sizeof(data));
			paramDescIter->needUpdate = true;
		}
	}
	template<typename _Ty>
	const _Ty& getParam(const QString& name) {
		auto paramDescIter = getParamDesc(name);
		_Ty param;
		if (paramDescIter != mParams.end()) {
			memcpy(&param, mData.data() + paramDescIter->offsetInByte, sizeof(_Ty));
		}
		return param;
	}

	void addParamFloat(const QString& name, float var);
	void addParamVec2(const QString& name, QVector2D vec2);
	void addParamVec3(const QString& name, QVector3D vec3);
	void addParamVec4(const QString& name, QVector4D vec4);
	void addParamMat4(const QString& name, QMatrix4x4 mat4);
	void removeParam(const QString& name);

	void setTextureSampler(const QString& name, const QImage& image);
	void addTextureSampler(const QString& name, const QImage& image);
	void removeTextureSampler(const QString& name);
protected:
	void addParam(const QString& name, void* data, uint16_t size, int typeId);
	QVector<QParamDesc>::iterator getParamDesc(const QString& name);
	QVector<QTextureDesc>::iterator getTextureDesc(const QString& name);
protected:
	QVector<QParamDesc> mParams;
	QVector<uint8_t> mData;
	QVector<QTextureDesc> mTexture;
	QByteArray mShadingCode;
};

#endif // QMaterial_h__
