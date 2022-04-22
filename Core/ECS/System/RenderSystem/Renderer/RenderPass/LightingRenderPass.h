#ifndef LightingRenderPass_h__
#define LightingRenderPass_h__

#include "BlurRenderPass.h"
#include "IRenderPassBase.h"

class ILightComponent;

class LightingRenderPass:public IRenderPassBase {
public:
	LightingRenderPass();

	void setupBaseColorTexutre(QRhiTexture* texture);
	void setupPositionTexutre(QRhiTexture* texture);
	void setupNormalMetalnessTexutre(QRhiTexture* texture);
	void setupTangentRoughnessTexutre(QRhiTexture* texture);
	
	void addLightItem(ILightComponent* item);
	void removeLightItem(ILightComponent* item);


	virtual void compile() override;

	void rebuildLight();

	virtual void execute(QRhiCommandBuffer* cmdBuffer) override;

	enum OutputTextureSlot {
		LightingResult
	};
	virtual QRhiTexture* getOutputTexture(int slot = 0) {
		switch ((OutputTextureSlot)slot) {
		case LightingResult: return mRT.colorAttachment.get();
		}
		return nullptr;
	}


private:
	struct RTResource {
		QRhiSPtr<QRhiTexture> colorAttachment;
		QRhiSPtr<QRhiTextureRenderTarget> renderTarget;
		QRhiSPtr<QRhiRenderPassDescriptor> renderPassDesc;
	};
	struct UniformBlock {
		QVector3D eyePosition;
	};

	RTResource mRT;
	QRhiSPtr<QRhiGraphicsPipeline> mPipeline;
	QRhiSPtr<QRhiBuffer> mUniformBuffer;
	QRhiSPtr<QRhiSampler> mSampler;
	QRhiSPtr<QRhiShaderResourceBindings> mBindings;

	QRhiTexture* mBaseColorTexture;
	QRhiTexture* mPositionTexture;
	QRhiTexture* mNormal_MetalnessTexture;
	QRhiTexture* mTangent_RoughnessTexture;

	QList<ILightComponent*> mLightItemList;
};

#endif // LightingRenderPass_h__
