#ifndef ColorGradingRenderPass_h__
#define ColorGradingRenderPass_h__

#include "IRenderPassBase.h"
#include "ExtType\QCombo.h"

class ColorGradingRenderPass :public IRenderPassBase {
	Q_OBJECT
		Q_PROPERTY(QCombo LUT READ getLUT WRITE setLUT)
public:
	ColorGradingRenderPass();

	virtual void compile() override;
	virtual void execute(QRhiCommandBuffer* cmdBuffer) override;

	enum InputTextureSlot {
		Color = 0,
	};

	enum OutputTextureSlot {
		Result = 0,
	};

	void recreatePipeline();

	QCombo getLUT() const;
	void setLUT(QCombo val);
private:
	struct RTResource {
		QRhiSPtr<QRhiTexture> colorAttachment;
		QRhiSPtr<QRhiTextureRenderTarget> renderTarget;
		QRhiSPtr<QRhiRenderPassDescriptor> renderPassDesc;
	};
	RTResource mRT;
	QRhiSPtr<QRhiGraphicsPipeline> mPipeline;

	QRhiSPtr<QRhiSampler> mSampler;
	QRhiSPtr<QRhiShaderResourceBindings> mBindings;

	QRhiSPtr<QRhiTexture> mLUTTexture;

	struct Param {
		int LUTBlockNum;
		float LUTBlockSize;
	}mParam;

	QRhiSPtr<QRhiBuffer> mUniformBuffer;
	QImage mLUT;
	QCombo mLutCombo;
	QMap<QString, QString> mNameToPath;
};



#endif // ColorGradingRenderPass_h__
