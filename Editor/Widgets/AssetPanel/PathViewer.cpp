#include "PathViewer.h"
#include "QPushButton"
#include "QMenu"
#include "Widgets\Buttons\ImageButton.h"

PathViewer::PathViewer(QString rootDir)
	: hLayout(new QHBoxLayout(this))
	, rootDir_(rootDir)
{
	hLayout->setContentsMargins(0, 0, 0, 0);
	hLayout->setSpacing(3);
	hLayout->setAlignment(Qt::AlignLeft | Qt::AlignVCenter);
	setFixedHeight(25);
}

void PathViewer::setCurrentDir(QString dir)
{
	currentDir_ = dir;
	QStringList partonList = rootDir_.relativeFilePath(dir).split("/");

	QLayoutItem* item;
	while ((item = hLayout->takeAt(0)) != NULL) {
		delete item->widget();
		delete item;
	}
	if (partonList.size() == 1 && partonList.first() == ".") {
		return;
	}

	QString path = rootDir_.path();
	for (auto& parton : partonList) {
		path = QDir(path).filePath(parton);
		QPushButton* btText = new QPushButton(parton);
		btText->setFixedHeight(20);
		hLayout->addWidget(btText);
		connect(btText, &QPushButton::clicked, this, [this, path]() {
			Q_EMIT pathClicked(path);
		});
		if (QDir(path).entryInfoList(QDir::Filter::Dirs | QDir::Filter::NoDotAndDotDot).isEmpty())
			return;
		ImageButton* btImage = new ImageButton(":/Resources/Icons/right_arrow.png");
		btImage->setFixedSize(15, 15);
		hLayout->addWidget(btImage);
		connect(btImage, &QPushButton::clicked, this, [this, path, btImage]() {
			QMenu menu;
			QSvgIcon icon(":/Resources/Icons/24gf-folder.png");
			for (auto& subDir : QDir(path).entryInfoList(QDir::Filter::Dirs | QDir::Filter::NoDotAndDotDot)) {
				menu.addAction(icon.getIcon(), subDir.fileName(), [this, subDir]() {
					Q_EMIT pathClicked(subDir.filePath());
				});
			}
			menu.exec(btImage->mapToGlobal(btImage->rect().bottomLeft()));
		});
	}
}