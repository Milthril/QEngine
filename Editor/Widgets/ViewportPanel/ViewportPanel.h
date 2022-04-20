#ifndef ViewportPanel_h__
#define ViewportPanel_h__

#include <QWidget>
#include "Toolkit\DropInterface.h"

class ViewportPanel: public QWidget, public DropInterface{
public:
	ViewportPanel();

private:
	QWidget* mViewportContainter = nullptr;
protected:
	virtual bool isVaild() override;
	virtual bool verifyCanDrop(const QMimeData* data) override;
	virtual void updateDropState() override;
	virtual void paintEvent(QPaintEvent* event) override;
	bool eventFilter(QObject* watched, QEvent* event) override;
};

#endif // ViewportPanel_h__
