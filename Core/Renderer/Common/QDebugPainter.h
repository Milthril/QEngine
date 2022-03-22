#ifndef QDebugPainter_h__
#define QDebugPainter_h__

#include "QObject"
#include "RHI\QRhiWindow.h"
#include "QImguiPainter.h"
#include "ImGuizmo.h"

class QDebugPainter :public QImguiPainter {
	Q_OBJECT
public:
	QDebugPainter();
	void paint() override;
protected:
	bool eventFilter(QObject* watched, QEvent* event) override;
Q_SIGNALS:
	void currentCompChanged(std::shared_ptr<QSceneComponent>);
private:
	std::shared_ptr<QSceneComponent> mCurrentComp;
	ImGuizmo::OPERATION mOpt = ImGuizmo::OPERATION::TRANSLATE;
};

#endif // QDebugPainter_h__