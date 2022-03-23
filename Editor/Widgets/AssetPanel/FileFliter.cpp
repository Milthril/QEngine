#include "FileFliter.h"
#include "QWidgetAction"
#include "QBoxLayout"
#include "Adjuster\BoolBox.h"
#include <QLabel>
#include <QMetaEnum>
#include "QApplication"

class FilterCheckAction :public QWidgetAction {
public:
	FilterCheckAction(FileFilterMenu::FileFliterFlag flag)
		: QWidgetAction(nullptr)
		, flag_(flag) {
		QWidget* widget = new QWidget;
		QHBoxLayout* h = new QHBoxLayout(widget);
		h->setContentsMargins(0, 0, 0, 0);
		h->addWidget(&boolBox);
		h->addWidget(new QLabel(metaFlag.valueToKey((int)flag)));
		setDefaultWidget(widget);
	}
	FileFilterMenu::FileFliterFlag getFlag() {
		return flag_;
	}
private:
	FileFilterMenu::FileFliterFlag flag_;
	BoolBox boolBox;
	inline static QMetaEnum metaFlag = QMetaEnum::fromType<FileFilterMenu::FileFliterFlag>();;
};

FileFilterMenu::FileFilterMenu()
{
	addFlag(FileFliterFlag::Base);
	addFlag(FileFliterFlag::Matrial);
	addFlag(FileFliterFlag::Texture);
	addFlag(FileFliterFlag::Animation);
	//addFlagGroup("Animation", { FileFliterFlag::Texture ,FileFliterFlag::Texture ,FileFliterFlag::Texture });
}

void FileFilterMenu::addFlag(FileFliterFlag flag)
{
	QWidgetAction* action = new FilterCheckAction(flag);
	addAction(action);
}

void FileFilterMenu::addFlagGroup(const QString& name, QList<FileFliterFlag> flagList)
{
	QMenu* menu = addMenu(name);
	for (auto& flag : flagList) {
		QWidgetAction* action = new FilterCheckAction(flag);
		menu->addAction(action);
	}
}

FileFilterMenu::FileFliterFlags FileFilterMenu::exec(QPoint pos)
{
	QMenu::exec(pos);
	return FileFilterMenu::FileFliterFlags(0);
}

bool FileFilterMenu::isValidFile(const QFileInfo& fileInfo, FileFliterFlags flags)
{
	if (flags & FileFliterFlag::Base) {
	}
	else if (FileFliterFlag::Animation) {
	}
	else if (FileFliterFlag::Matrial) {
	}
	else if (FileFliterFlag::Texture) {
	}
	return false;
}