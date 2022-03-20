#ifndef QDebugPainter_h__
#define QDebugPainter_h__

#include "QObject"
#include "RHI\QRhiWindow.h"
#include "QImguiPainter.h"

class QDebugPainter :public QImguiPainter {
	Q_OBJECT
public:
	enum Mode {
		Translate,
		Rotate,
		Scale
	};
	QDebugPainter();
	void paint() override;
protected:
	bool eventFilter(QObject* watched, QEvent* event) override;
Q_SIGNALS:
	void currentCompChanged(std::shared_ptr<QSceneComponent>);
private:
	std::shared_ptr<QSceneComponent> mCurrentComp;
};


#endif // QDebugPainter_h__