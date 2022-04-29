#ifndef LightingRenderPass_h__
#define LightingRenderPass_h__

#include "BlurRenderPass.h"
#include "IRenderPassBase.h"

class ILightComponent;

class LightingRenderPass:public IRenderPassBase {
public:
	LightingRenderPass();

	void addLightItem(ILightComponent* item);
	void removeLightItem(ILightComponent* item);

	virtual void compile() override;

	void rebuildLight();

	virtual void execute(QRhiCommandBuffer* cmdBuffer) override;

	enum InputTextureSlot {
		Color = 0,
		Position,
		Normal_MetalnessTexture,
		Tangent_RoughnessTexture
	};

	enum OutputTextureSlot {
		LightingResult
	};
private:
	void buildIBL(QRhiCommandBuffer* cmdBuffer);
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

	QList<ILightComponent*> mLightItemList;

	QRhiTexture* mSkyCube;

	QRhiSPtr<QRhiComputePipeline> spmapPipeline;
	QRhiSPtr<QRhiShaderResourceBindings> spmapShaderBindings;
	QRhiSPtr<QRhiBuffer> spmapUniform;
	QRhiSPtr<QRhiTexture> spmapTexture;

	QRhiSPtr<QRhiComputePipeline> irmapPipeline;
	QRhiSPtr<QRhiShaderResourceBindings> irmapShaderBindings;
	QRhiSPtr<QRhiTexture> irmapTexture;

	QRhiSPtr<QRhiComputePipeline> BRDFPipeline;
	QRhiSPtr<QRhiShaderResourceBindings> BRDFShaderBindings;
	QRhiSPtr<QRhiTexture> BRDFTexture;
};

#endif // LightingRenderPass_h__
