#ifndef DebugDrawPass_h__
#define DebugDrawPass_h__

#include "QObject"
#include "RHI\QRhiWindow.h"
#include "ImGuiDrawPass.h"
#include "ImGuizmo.h"

class DebugDrawPass :public ImGuiDrawPass {
	Q_OBJECT
public:
	DebugDrawPass();
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

#endif // DebugDrawPass_h__