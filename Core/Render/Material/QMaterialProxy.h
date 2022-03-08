#ifndef QMaterialProxy_h__
#define QMaterialProxy_h__

#include "private\qrhi_p.h"
#include "Render\Renderer\QSceneRenderer.h"

class QMaterial;

class QMaterialProxy {
public:
	struct MaterialShaderInfo {
		QVector<QRhiShaderResourceBinding> bindings;
		QByteArray uniformCode;
		QByteArray shadingCode;
	};
	QMaterialProxy(QMaterial* material, std::shared_ptr<QRhi> rhi);
	void recreateResource();
	void uploadResource();
	void updateResource();
	MaterialShaderInfo getMaterialShaderInfo(uint8_t bindingOffset = 0);
protected:
	QMaterial* mMaterial;
	std::shared_ptr<QRhi> mRhi;
	QRhiSPtr<QRhiBuffer> mUniformBlock;
	QMap<QString, QRhiSPtr<QRhiTexture>> mTextureMap;
};

#endif // QMaterialProxy_h__
