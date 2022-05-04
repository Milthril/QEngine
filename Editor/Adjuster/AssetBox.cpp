#include <QComboBox>
#include <QHBoxLayout>
#include <QDragEnterEvent>
#include <QMimeData>
#include <QFileDialog>
#include <QPainter>

#include "AssetBox.h"
#include "Widgets/Inspector/QPropertyAdjusterItem.h"
#include "Window/MaterialEditor/QMaterialEditor.h"
#include "Asset/Material.h"
#include "Window/ParticlesEditor/QParticlesEditor.h"
#include "Asset/GAssetMgr.h"
#include "QEngineCoreApplication.h"

AssetBox::AssetBox(std::shared_ptr<IAsset> asset, IAsset::Type type, QWidget* parent /*= nullptr*/)
	: mDefaultAsset(asset)
	, mCurrentAsset(asset)
	, btOpenFile(":/Resources/Icons/24gf-folderOpen.png")
	, btReset(":/Resources/Icons/reset.png")
	, mType(type)
{
	QHBoxLayout* h = new QHBoxLayout(this);
	h->setContentsMargins(0, 0, 0, 0);
	h->addWidget(&mName);
	h->addWidget(&btOpenFile);
	h->addWidget(&btReset);
	setAcceptDrops(true);
	btOpenFile.setFixedSize(15, 15);
	btReset.setFixedSize(15, 15);
	mName.setEnabled(false);
	if (asset) {
		mName.setText(asset->getName());
	}
	connect(&btOpenFile, &QPushButton::clicked, this, [this]() {
		auto filePath = QFileDialog::getOpenFileName(nullptr, QString(), TheAssetMgr->assetDir().path(), QString("*.%1").arg(IAsset::AssetExtName[mType]));
		if (filePath.isEmpty() || !QFile::exists(filePath))
			return;
		setValue(QVariant::fromValue(TheAssetMgr->load(filePath)));
	});

	connect(&btReset, &QPushButton::clicked, this, [this]() {
		setValue(QVariant::fromValue(mDefaultAsset));
	});
}

QVariant AssetBox::getValue()
{
	return QVariant::fromValue(mCurrentAsset);
}

void AssetBox::setValue(QVariant var){
	auto it = var.value<std::shared_ptr<IAsset>>();
	if (it != mCurrentAsset) {
		mCurrentAsset = it;
		mName.setText(mCurrentAsset?mCurrentAsset->getName():"");
		Q_EMIT valueChanged(QVariant::fromValue(mCurrentAsset));
		if ((mType == IAsset::StaticMesh || mType == IAsset::ParticleSystem) && mItem) {
			mItem->updatePanel();
		}
		if ( mType == IAsset::SkeletonModel && mItem) {
			mItem->recreatePanel();
		}
	}
}

void AssetBox::dragEnterEvent(QDragEnterEvent* event) {
	if (verifyCanDrop(event->mimeData())) {
		event->acceptProposedAction();
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
			auto asset = TheAssetMgr->load(path);
			setValue(QVariant::fromValue(asset));
		}
	}
}

void AssetBox::mouseDoubleClickEvent(QMouseEvent* event) {
	if (mCurrentAsset && mType == IAsset::Material) {
		QMaterialEditor::instance()->edit(std::dynamic_pointer_cast<Asset::Material>(mCurrentAsset));
	}
	else if (mCurrentAsset && mType == IAsset::ParticleSystem) {
		QParticlesEditor::instance()->edit(std::dynamic_pointer_cast<Asset::ParticleSystem>(mCurrentAsset));
	}
}

bool AssetBox::isVaild() {
	return isVisible();
}

bool AssetBox::verifyCanDrop(const QMimeData* data) {
	if (data->hasFormat("application/x-qabstractitemmodeldatalist")) {
		QByteArray itemData = data->data("application/x-qabstractitemmodeldatalist");
		QDataStream stream(&itemData, QIODevice::ReadOnly);
		QMap<int, QVariant> roleDataMap;
		int  row, col;
		stream >> row >> col >> roleDataMap;
		if (roleDataMap.contains(Qt::ToolTipRole)) {
			QString path = roleDataMap[Qt::ToolTipRole].toString();
			if (path.endsWith(IAsset::AssetExtName[mType])) {
				return true;
			}
		}
	}
	return false;
}

void AssetBox::updateDropState() {
	update();
}

void AssetBox::paintEvent(QPaintEvent* event) {
	if (canDrop()) {
		QPainter painter(this);
		painter.setRenderHint(QPainter::Antialiasing);
		painter.setPen(Qt::NoPen);
		painter.setBrush(QColor(100, 200, 100));
		painter.drawRoundedRect(rect(), 2, 2);
	}
	Adjuster::paintEvent(event);
}

