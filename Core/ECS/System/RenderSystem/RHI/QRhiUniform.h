#ifndef QRhiUniform_h__
#define QRhiUniform_h__

#include "QRhiDefine.h"
#include "Script\QLuaScript.h"
#include <QVariant>
#include "ExtType\QColor4D.h"

class IRenderable;

class QRhiUniform {
	friend class IRenderer;
public:
	struct ParamDescBase {
		QString name;
		enum Type {
			Float = 0,
			Vec2,
			Vec3,
			Vec4,
			Color4,
			Sampler2D,
			Mat4,
		}type;
		QString getTypeName();
		QRhiSignal bNeedUpdate;
		virtual ~ParamDescBase() {}
	};

	struct DataDesc: public ParamDescBase {
		QVariant var;
		uint32_t offsetInByte;
		uint32_t sizeInByte;
		uint32_t sizeInByteAligned;
	};

	struct TextureDesc :public ParamDescBase {
		QImage image;
		QRhiSPtr<QRhiTexture> texture;
	};

	QRhiUniform();

	void setDataFloat(QString name, float var);
	void setDataVec2(QString name, QVector2D var);
	void setDataVec3(QString name, QVector3D var);
	void setDataVec4(QString name, QVector4D var);
	void setDataColor4(QString name, QColor4D var);

	template <typename _Ty>
	void setData(QString name,_Ty var) {
		for (auto& dataDesc : mDataList) {
			if (dataDesc->name == name) {
				dataDesc->var = var;
				dataDesc->bNeedUpdate.active();
				return;
			}
		}
	}

	template <typename _Ty>
	_Ty getData(QString name) {
		for (auto& dataDesc : mDataList) {
			if (dataDesc->name == name) {
				_Ty var = dataDesc->var.value<_Ty>();
				return var;
			}
		}
		return _Ty();
	}

	void setTexture2D(const QString& name, const QImage& image);

	std::shared_ptr<QLuaScript> getScript() const { return mScript; }

	bool renameParma(const QString& src, const QString& dst);
	void removeParam(const QString& name);

	void addRef(IRenderable* comp);
	void removeRef(IRenderable* comp);

	void serialize(QDataStream& out);
	void deserialize(QDataStream& in);

	struct UniformInfo {
		QVector<QRhiShaderResourceBinding> bindings;
		QByteArray uniformDefineCode;
		uint8_t bindingOffset;
	};

	UniformInfo getUniformInfo(uint8_t bindingOffset = 0, QString blockName = "UBO", QRhiShaderResourceBinding::StageFlag stage = QRhiShaderResourceBinding::FragmentStage);
	void recreateResource();
	virtual void updateResource(QRhiResourceUpdateBatch* batch);

	QList<std::shared_ptr<ParamDescBase>> getParamList();

protected:
	Q_DISABLE_COPY(QRhiUniform)
	std::shared_ptr<ParamDescBase> getOrCreateParam(QString name, ParamDescBase::Type type);
	QString getVaildName(QString name);
	void updateLayout();
protected:
	QList<std::shared_ptr<DataDesc>> mDataList;
	QList<std::shared_ptr<TextureDesc>> mTextureList;
	QHash<QString, std::shared_ptr<ParamDescBase>> mParamNameMap;
	uint32_t mDataSize;
	QVector<IRenderable*> mRef;

	std::shared_ptr<QLuaScript> mScript;

	QRhiSignal bNeedRecreate;
	QRhiSPtr<QRhiBuffer> mUniformBlock;
	QRhiSPtr<QRhiSampler> mSampler;
};
#endif // QRhiUniform_h_