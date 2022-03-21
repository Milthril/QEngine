#ifndef QImguiPainter_h__
#define QImguiPainter_h__

#include "QObject"
#include "RHI\QRhiWindow.h"
#include "imgui_internal.h"

class QImguiPainter :public QObject {
	Q_OBJECT
public:
	QImguiPainter();
	virtual void paint() = 0;
	void updatePrePass(QRhiResourceUpdateBatch* batch, QRhiRenderTarget* outputTarget);
	void drawInPass(QRhiCommandBuffer* cmdBuffer, QRhiRenderTarget* outputTarget);
	void setupWindow(QRhiWindow* window);
protected:
	void initRhiResource(QRhiResourceUpdateBatch* batch, QRhiRenderTarget* outputTarget);
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
	ImGuiContext* mImGuiContext = nullptr;
	double       mTime = 0.0f;
	bool         mMousePressed[3] = { false, false, false };
	float        mMouseWheel = 0;
	float        mMouseWheelH = 0;
};

#endif // QImguiPainter_h__