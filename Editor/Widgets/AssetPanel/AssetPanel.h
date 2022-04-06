#ifndef AssetPanel_h__
#define AssetPanel_h__

#include "QLineEdit"
#include "PathViewer.h"
#include "DirTreeWidget.h"
#include "FileListWidget.h"
#include "QPushButton"
#include "FileFliter.h"
#include "QStatusBar"
#include "Toolkit\QSvgIcon.h"

class AssetPanel : public QWidget {
	Q_OBJECT
public:
	AssetPanel(QString rootDir);
private:
	void createUI();
	void connectUI();
private:
	PathViewer pathViewer_;
	DirTreeWidget directoryWidget_;
	QLineEdit directorySeacher_;
	FileListWidget fileWidget_;
	QLineEdit fileSearcher_;
	QSvgIcon mFileFileterIcon;
	QPushButton btFileFilter_;
	FileFilterMenu fileFliterMenu_;
	QStatusBar fileStatusBar_;
	QDir rootDir_;
};

#endif // AssetPanel_h__
