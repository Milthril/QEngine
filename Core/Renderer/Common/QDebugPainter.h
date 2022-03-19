#ifndef QDebugPainter_h__
#define QDebugPainter_h__

#include "QObject"
#include "RHI\QRhiWindow.h"

class QDebugPainter :public QObject{
	Q_OBJECT
public:
	QDebugPainter();
protected:
	bool eventFilter(QObject* watched, QEvent* event) override;
Q_SIGNALS:
	void currentCompChanged(std::shared_ptr<QSceneComponent>);
private:
	std::shared_ptr<QSceneComponent> mCurrentComp;
};


#endif // QDebugPainter_h__