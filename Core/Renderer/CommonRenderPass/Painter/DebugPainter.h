#ifndef DebugPainter_h__
#define DebugPainter_h__

#include "ImGuiPainter.h"
#include "ImGuizmo.h"
#include "Scene\QSceneComponent.h"

class DebugPainter :public ImGuiPainter {
	Q_OBJECT
public:
	DebugPainter();
	void setupDebugTexture(QRhiTexture* texture) {
		mDebugTexture = texture;
	}
	void paintImgui() override;
	virtual void resourceUpdate(QRhiResourceUpdateBatch* batch) override;
protected:
	bool eventFilter(QObject* watched, QEvent* event) override;
private:
	QRhiTexture* mDebugTexture = nullptr;

	ImGuizmo::OPERATION mOpt = ImGuizmo::OPERATION::TRANSLATE;
	QRhiReadbackResult mReadReult;
	QRhiReadbackDescription mReadDesc;
	QPoint mReadPoint;

};

#endif // DebugPainter_h__