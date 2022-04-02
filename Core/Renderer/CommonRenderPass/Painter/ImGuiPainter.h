#ifndef ImGuiPainter_h__
#define ImGuiPainter_h__

#include "QObject"
#include "RHI\QRhiWindow.h"
#include "imgui_internal.h"
#include "IPainter.h"

class ImGuiPainter :public QObject ,public IPainter{
	Q_OBJECT
public:
	ImGuiPainter();
	void setupWindow(QRhiWindow* window);
	void setupRenderTarget(QRhiRenderTarget* renderTarget);

	virtual void compile() override;
	virtual void paint(QRhiCommandBuffer* cmdBuffer) override;

	virtual void paintImgui() = 0;
protected:
	virtual bool eventFilter(QObject* watched, QEvent* event) override;
Q_SIGNALS:
	void currentCompChanged(std::shared_ptr<QSceneComponent>);
protected:
	std::shared_ptr<QSceneComponent> mCurrentComp;
	QRhiSPtr<QRhiGraphicsPipeline> mPipeline;
	QRhiSPtr<QRhiBuffer> mVertexBuffer;
	QRhiSPtr<QRhiBuffer> mIndexBuffer;
	QRhiSPtr<QRhiBuffer> mUniformBuffer;
	QRhiSPtr<QRhiShaderResourceBindings> mBindings;
	QRhiSPtr<QRhiTexture> mFontTexture;
	QRhiSPtr<QRhiSampler> mSampler;
	QRhiWindow* mWindow;
	QImage mFontImage;
	QRhiRenderTarget* mRenderTarget;
	ImGuiContext* mImGuiContext = nullptr;
	double       mTime = 0.0f;
	bool         mMousePressed[3] = { false, false, false };
	float        mMouseWheel = 0;
	float        mMouseWheelH = 0;
};

#endif // ImGuiPainter_h__