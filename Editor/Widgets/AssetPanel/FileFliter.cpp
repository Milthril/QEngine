#include "FileFliter.h"
#include "QWidgetAction"
#include "QBoxLayout"
#include "Adjuster\BoolBox.h"
#include <QLabel>
#include "Asset\IAsset.h"

class FilterCheckAction :public QWidgetAction {
public:
	FilterCheckAction(QString name)
		: QWidgetAction(nullptr)
		, mName(name) {
		QWidget* widget = new QWidget;
		QHBoxLayout* h = new QHBoxLayout(widget);
		h->setContentsMargins(0, 0, 0, 0);
		h->addWidget(&boolBox);
		h->addWidget(new QLabel(mName));
		setDefaultWidget(widget);
	}
	bool isChecked() {
		return boolBox.getValue().toBool();
	}
private:
	QString mName;
	BoolBox boolBox;
};

FileFilterMenu::FileFilterMenu()
{
	for (auto& assetType : IAsset::AssetExtName.values()) {
		addItem(assetType);
	}
}

void FileFilterMenu::addItem(QString name)
{
	QWidgetAction* action = new FilterCheckAction(name);
	addAction(action);
}

