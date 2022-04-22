#ifndef QRhiUniformProxy_h__
#define QRhiUniformProxy_h__

#include "private\qrhi_p.h"
#include "QRhiDefine.h"

class QRhiUniform;

class QRhiUniformProxy {
public:
	struct UniformInfo {
		QVector<QRhiShaderResourceBinding> bindings;
		QByteArray uniformDefineCode;
		uint8_t bindingOffset;
	};
	QRhiUniformProxy(QRhiUniform* material);
	void recreateResource();
	void updateResource(QRhiResourceUpdateBatch* batch);
	UniformInfo getUniformInfo(uint8_t bindingOffset = 0, QString blockName = "UBO", QRhiShaderResourceBinding::StageFlag stage = QRhiShaderResourceBinding::FragmentStage);
protected:
	QRhiUniform* mMaterial;
	QRhiSPtr<QRhiBuffer> mUniformBlock;
	QRhiSPtr<QRhiSampler> mSampler;
	QMap<QString, QRhiSPtr<QRhiTexture>> mTextureMap;
};

#endif // QRhiUniformProxy_h__
