#ifndef DirTreeWidget_h__
#define DirTreeWidget_h__

#include "QTreeWidget"
#include "QDir"

class DirTreeWidget : public QTreeWidget {
public:
	DirTreeWidget(QString rootDir);
	void setCurrentDir(QString dir);
	void searchDir(QString keyword);
protected:
	void intiDirectories();
	void processDir(QDir dir, QTreeWidgetItem* item);
private:
	QHash<QString, QTreeWidgetItem*> itemMap_;
	QDir rootDir_;
};

#endif // DirTreeWidget_h__