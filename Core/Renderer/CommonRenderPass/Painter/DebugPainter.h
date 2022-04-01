#ifndef DebugPainter_h__
#define DebugPainter_h__

#include "QObject"
#include "RHI\QRhiWindow.h"
#include "ImGuiPainter.h"
#include "ImGuizmo.h"

class DebugPainter :public ImGuiPainter {
	Q_OBJECT
public:
	DebugPainter();
	void paint() override;
	void setCurrentCompInternal(QSceneComponent* comp);
protected:
	bool eventFilter(QObject* watched, QEvent* event) override;
Q_SIGNALS:
	void currentCompChanged(QSceneComponent* comp);
private:
	QSceneComponent* mCurrentComp = nullptr;
	ImGuizmo::OPERATION mOpt = ImGuizmo::OPERATION::TRANSLATE;
};

#endif // DebugPainter_h__