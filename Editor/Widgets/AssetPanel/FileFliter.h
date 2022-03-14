#ifndef FileFliter_h__
#define FileFliter_h__

#include "QMenu"
#include "QFileInfo"

class FileFilterMenu :public QMenu {
	Q_OBJECT
public:
	FileFilterMenu();
	enum FileFliterFlag {
		Base = 1,
		Animation = 1 << 1,
		Texture = 1 << 2,
		Matrial = 1 << 3
	};
	Q_ENUM(FileFliterFlag);
	Q_DECLARE_FLAGS(FileFliterFlags, FileFliterFlag);

	FileFliterFlags exec(QPoint pos);
	static bool isValidFile(const QFileInfo& fileInfo, FileFliterFlags flags);
private:
	void addFlag(FileFliterFlag flag);
	void addFlagGroup(const QString& name, QList<FileFliterFlag> flagList);
};

#endif // FileFliter_h__
