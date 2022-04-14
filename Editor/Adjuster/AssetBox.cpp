#include "AssetBox.h"
#include <QComboBox>
#include <QHBoxLayout>
#include <QDragEnterEvent>
#include <QMimeData>

AssetBox::AssetBox(std::shared_ptr<IAsset> asset, IAsset::Type type, QWidget* parent /*= nullptr*/)
	: mComboBox(new QComboBox)
	, mDefaultAsset(asset)
	, mCurrentAsset(asset)
	, mType(type)
{
	QHBoxLayout* h = new QHBoxLayout(this);
	h->setContentsMargins(0, 0, 0, 0);
	h->addWidget(mComboBox);
	setAcceptDrops(true);
}

QVariant AssetBox::getValue()
{
	return QVariant::fromValue(mCurrentAsset);
}

void AssetBox::setValue(QVariant var)
{
	mCurrentAsset = var.value<std::shared_ptr<IAsset>>();
}

void AssetBox::dragEnterEvent(QDragEnterEvent* event) {
	if (event->mimeData()->hasFormat("application/x-qabstractitemmodeldatalist")) {
		QByteArray itemData = event->mimeData()->data("application/x-qabstractitemmodeldatalist");
		QDataStream stream(&itemData, QIODevice::ReadOnly);
		QMap<int, QVariant> roleDataMap;
		int  row, col;
		stream >> row >> col >> roleDataMap;
		if (roleDataMap.contains(Qt::ToolTipRole)) {
			QString path = roleDataMap[Qt::ToolTipRole].toString();
			if (path.endsWith(IAsset::mAssetExtName[mType])) {
				event->acceptProposedAction();
			}
		}
	}
}

void AssetBox::dropEvent(QDropEvent* event) {
	if (event->mimeData()->hasFormat("application/x-qabstractitemmodeldatalist")) {
		QByteArray itemData = event->mimeData()->data("application/x-qabstractitemmodeldatalist");
		QDataStream stream(&itemData, QIODevice::ReadOnly);
		QMap<int, QVariant> roleDataMap;
		int  row, col;
		stream >> row >> col >> roleDataMap;
		if (roleDataMap.contains(Qt::ToolTipRole)) {
			QString path = roleDataMap[Qt::ToolTipRole].toString();
			mCurrentAsset = IAsset::CreateAssetFromPath(path);
			Q_EMIT valueChanged(QVariant::fromValue(mCurrentAsset));
		}
	}
}
