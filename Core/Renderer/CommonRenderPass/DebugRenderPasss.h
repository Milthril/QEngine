#ifndef DebugRenderPasss_h__
#define DebugRenderPasss_h__
#include "..\IRenderPassBase.h"

#include "Painter\ImGuiPainter.h"
#include "ImGuizmo.h"

class DebugPainter :public ImGuiPainter {
	Q_OBJECT
public:
	DebugPainter();
	virtual void paintImgui() override;
	void setCurrentCompInternal(QSceneComponent* comp);
protected:
	bool eventFilter(QObject* watched, QEvent* event) override;
Q_SIGNALS:
	void currentCompChanged(QSceneComponent* comp);
private:
	QSceneComponent* mCurrentComp = nullptr;
	ImGuizmo::OPERATION mOpt = ImGuizmo::OPERATION::TRANSLATE;
};


class DebugRenderPasss :public IRenderPassBase {
public:
	DebugRenderPasss();
	void setupWindow(QRhiWindow* window);

	virtual void compile() override;
	virtual void execute() override;
protected:
	struct RTResource {
		QRhiSPtr<QRhiTexture> colorAttachment;
		QRhiSPtr<QRhiTextureRenderTarget> renderTarget;
	};
	RTResource mRT;
	QRhiWindow* mWindow;
	QSize mWindowFrameSize;
	DebugPainter mDebugPainter;
};



#endif // DebugRenderPasss_h__
