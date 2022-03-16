#ifndef QRhiUniform_h__
#define QRhiUniform_h__

#include "private\qrhi_p.h"
#include "QRhiUniformProxy.h"

class QRhiUniform {
	friend class QRhiUniformProxy;
public:
	struct QParamDesc {
		QString name;
		uint32_t offsetInByte;
		uint32_t sizeInByte;
		uint32_t sizeInByteAligned;
		bool needUpdate = false;
		enum Type {
			Float = 0,
			Vec2,
			Vec3,
			Vec4,
			Mat4,
		}type;
		QString getTypeName();
	};
	struct QTextureDesc {
		QString name;
		QImage image;
		bool needUpdate = false;
		enum Type {
			Sampler2D = 0,
			CubeMap,
		};
	};

	QRhiUniform();
	~QRhiUniform();
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
	_Ty getParam(const QString& name) {
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
	const QVector<QRhiUniform::QParamDesc>& getParams() const { return mParams; }
	QRhiSignal bNeedRecreate;
	std::shared_ptr<QRhiUniformProxy> getProxy() const { return mProxy; }
protected:
	void addParam(const QString& name, void* data, uint16_t size, QParamDesc::Type type);
	QVector<QParamDesc>::iterator getParamDesc(const QString& name);
	QVector<QTextureDesc>::iterator getTextureDesc(const QString& name);
protected:
	QVector<QParamDesc> mParams;

	QVector<int8_t> mData;
	QVector<QTextureDesc> mTexture;
	std::shared_ptr<QRhiUniformProxy> mProxy;
};

#endif // QRhiUniform_h__
