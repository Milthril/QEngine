#ifndef ImGuiPainter_h__
#define ImGuiPainter_h__

#include "QObject"
#include "imgui_internal.h"
#include "RHI/QRhiDefine.h"
#include "IPainter.h"

class ImGuiPainter :public QObject ,public IPainter{
	Q_OBJECT
public:
	ImGuiPainter();

	void setupWindow(QWindow* window);

	virtual void compile() override;

	virtual void resourceUpdate(QRhiResourceUpdateBatch* batch) override;

	virtual void paint(QRhiCommandBuffer* cmdBuffer, QRhiRenderTarget* renderTarget) override;

	virtual void paintImgui() = 0;

protected:
	virtual bool eventFilter(QObject* watched, QEvent* event) override;

	QRhiSPtr<QRhiGraphicsPipeline> mPipeline;
	QRhiSPtr<QRhiBuffer> mVertexBuffer;
	QRhiSPtr<QRhiBuffer> mIndexBuffer;
	QRhiSPtr<QRhiBuffer> mUniformBuffer;
	QRhiSPtr<QRhiShaderResourceBindings> mBindings;
	QRhiSPtr<QRhiTexture> mFontTexture;
	QRhiSPtr<QRhiSampler> mSampler;
	QWindow* mWindow;
	QImage mFontImage;
	ImGuiContext* mImGuiContext = nullptr;
	double       mTime = 0.0f;
	bool         mMousePressed[3] = { false, false, false };
	float        mMouseWheel = 0;
	float        mMouseWheelH = 0;
};

#endif // ImGuiPainter_h__