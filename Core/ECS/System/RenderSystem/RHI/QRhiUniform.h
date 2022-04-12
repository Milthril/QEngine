#ifndef QRhiUniform_h__
#define QRhiUniform_h__

#include "private\qrhi_p.h"
#include "Script\QLuaScript.h"
#include "QRhiUniformProxy.h"

class IRenderable;

class QRhiUniform {
	friend class QRhiUniformProxy;
	friend class IRenderer;
public:
	struct ParamDescBase {
		QString name;
		enum Type {
			Float = 0,
			Vec2,
			Vec3,
			Vec4,
			Mat4,

			Sampler2D
		}type;
		QString getTypeName();
		QRhiSignal needUpdate;
		virtual ~ParamDescBase() {}
	};

	struct DataDesc : public ParamDescBase {
		uint32_t offsetInByte;
		uint32_t sizeInByte;
		uint32_t sizeInByteAligned;
	};
	struct TextureDesc :public ParamDescBase {
		QImage image;
	};

	QRhiUniform();
	~QRhiUniform();
	template<typename _Ty>
	void setData(const QString& name, const _Ty& data) {
		auto paramDescIter = getParamDesc(name);
		std::shared_ptr<QRhiUniform::DataDesc> ptr = std::dynamic_pointer_cast<QRhiUniform::DataDesc>(paramDescIter);
		if (ptr) {
			Q_ASSERT(sizeof(data) == ptr->sizeInByte);
			memcpy(mData.data() + ptr->offsetInByte, &data, sizeof(data));
			ptr->needUpdate.active();
		}
	}

	template<typename _Ty>
	_Ty getData(const QString& name) {
		auto paramDescIter = getParamDesc(name);
		_Ty param = {};
		std::shared_ptr<QRhiUniform::DataDesc> ptr = std::dynamic_pointer_cast<QRhiUniform::DataDesc>(paramDescIter);
		if (ptr) {
			memcpy(&param, mData.data() + ptr->offsetInByte, sizeof(_Ty));
		}
		return param;
	}
	void addDataFloat(const QString& name, float var);
	void addDataVec2(const QString& name, QVector2D vec2);
	void addDataVec3(const QString& name, QVector3D vec3);
	void addDataVec4(const QString& name, QVector4D vec4);
	void addDataMat4(const QString& name, QMatrix4x4 mat4);

	void removeParam(const QString& name);
	bool renameParma(const QString& src, const QString& dst);

	QString getVaildName(QString name);

	void setTextureSampler(const QString& name, const QImage& image);
	void addTextureSampler(const QString& name, const QImage& image);

	std::shared_ptr<QRhiUniformProxy> getProxy() const { return mProxy; }
	const QVector<std::shared_ptr<QRhiUniform::ParamDescBase>>& getParamsDesc() const { return mParams; }
	std::shared_ptr<ParamDescBase> getParamDesc(const QString& name);
	QVector<std::shared_ptr<DataDesc>> getAllDataDesc();
	QVector<std::shared_ptr<TextureDesc>> getAllTextureDesc();

	std::shared_ptr<QLuaScript> getScript() const { return mScript; }

	void addRef(IRenderable* comp);
	void removeRef(IRenderable* comp);
	QRhiSignal bNeedRecreate;
protected:
	void addParam(const QString& name, void* data, uint16_t size, ParamDescBase::Type type);
protected:
	QVector<std::shared_ptr<ParamDescBase>> mParams;
	QVector<int8_t> mData;
	std::shared_ptr<QRhiUniformProxy> mProxy;
	QVector<IRenderable*> mRef;
	std::shared_ptr<QLuaScript> mScript;
};
#endif // QRhiUniform_h_