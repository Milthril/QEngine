#ifndef FileFliter_h__
#define FileFliter_h__

#include "QMenu"
#include "QFileInfo"

class FileFilterMenu :public QMenu {
	Q_OBJECT
public:
	FileFilterMenu();
	void addItem(QString name);
};

#endif // FileFliter_h__
