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
protected:
	bool eventFilter(QObject* watched, QEvent* event) override;
Q_SIGNALS:
	void currentCompChanged(std::shared_ptr<QSceneComponent>);
private:
	std::shared_ptr<QSceneComponent> mCurrentComp;
	ImGuizmo::OPERATION mOpt = ImGuizmo::OPERATION::TRANSLATE;
};

#endif // DebugDrawPass_h__