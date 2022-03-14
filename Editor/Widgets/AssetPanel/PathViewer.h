#ifndef PathViewer_h__
#define PathViewer_h__

#include <QWidget>
#include "QBoxLayout"
#include "QDir"

class PathViewer : public QWidget {
	Q_OBJECT
public:
	PathViewer(QString rootDir);
	void setCurrentDir(QString dir);
private:
	QHBoxLayout* hLayout;
	QDir currentDir_;
	QDir rootDir_;
Q_SIGNALS:
	void pathClicked(QString path);
};

#endif // PathViewer_h__
