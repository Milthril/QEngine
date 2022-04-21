#include "ViewportPanel.h"
#include "ECS/System/RenderSystem/QRenderSystem.h"
#include <QHBoxLayout>
#include <QPainter>
#include <QDragEnterEvent>
#include <QMimeData>
#include "Asset/IAsset.h"
#include "Asset/GAssetMgr.h"
#include "QEngineCoreApplication.h"
#include "ECS/Component/RenderableComponent/QStaticMeshComponent.h"
#include "ECS/Component/RenderableComponent/QSkeletonModelComponent.h"
#include "ECS/Component/RenderableComponent/QParticleSystemComponent.h"

ViewportPanel::ViewportPanel() {
	mViewportContainter = QWidget::createWindowContainer(TheRenderSystem->window());
	mViewportContainter->setMinimumSize(400, 300);
	QHBoxLayout* h = new QHBoxLayout(this);
	h->addWidget(mViewportContainter);
	h->setContentsMargins(2, 2, 2, 2);
	setAcceptDrops(true);
	TheRenderSystem->window()->installEventFilter(this);
}


bool ViewportPanel::isVaild() {
	return isVisible();
}

bool ViewportPanel::verifyCanDrop(const QMimeData* data) {
	if (data->hasFormat("application/x-qabstractitemmodeldatalist")) {
		QByteArray itemData = data->data("application/x-qabstractitemmodeldatalist");
		QDataStream stream(&itemData, QIODevice::ReadOnly);
		QMap<int, QVariant> roleDataMap;
		int  row, col;
		stream >> row >> col >> roleDataMap;
		if (roleDataMap.contains(Qt::ToolTipRole)) {
			QString path = roleDataMap[Qt::ToolTipRole].toString();
			if (path.endsWith(IAsset::AssetExtName[IAsset::StaticMesh])
				|| path.endsWith(IAsset::AssetExtName[IAsset::SkeletonModel])
				|| path.endsWith(IAsset::AssetExtName[IAsset::ParticleSystem])
				) {
				return true;
			}
		}
	}
	return false;
}

void ViewportPanel::updateDropState() {
	update();
}

void ViewportPanel::paintEvent(QPaintEvent* event) {
	QWidget::paintEvent(event);
	if (canDrop()) {
		QPainter painter(this);
		painter.setPen(QPen(QColor(100, 200, 100), 6, Qt::PenStyle::SolidLine, Qt::PenCapStyle::RoundCap));
		painter.drawRoundedRect(rect(), 5, 5);
	}
}

bool ViewportPanel::eventFilter(QObject* watched, QEvent* event) {
	if (event->type() == QEvent::Drop) {
		QDropEvent* dropEvent = static_cast<QDropEvent*>(event);
		if (dropEvent && dropEvent->mimeData()->hasFormat("application/x-qabstractitemmodeldatalist")) {
			QByteArray itemData = dropEvent->mimeData()->data("application/x-qabstractitemmodeldatalist");
			QDataStream stream(&itemData, QIODevice::ReadOnly);
			QMap<int, QVariant> roleDataMap;
			int  row, col;
			while (!stream.atEnd()) {
				stream >> row >> col >> roleDataMap;
				if (roleDataMap.contains(Qt::ToolTipRole)) {
					QString path = roleDataMap[Qt::ToolTipRole].toString();
					std::shared_ptr<IAsset> asset = TheAssetMgr->load(path);
					QEntity* entity = TheEngine->world()->createEntity(QFileInfo(path).baseName());
					if (asset->type() == IAsset::StaticMesh) {
						QStaticMeshComponent* comp = entity->addComponent<QStaticMeshComponent>();
						comp->setStaticMesh(std::dynamic_pointer_cast<Asset::StaticMesh>(asset));
					}
					else if (asset->type() == IAsset::SkeletonModel) {
						QSkeletonModelComponent* comp = entity->addComponent<QSkeletonModelComponent>();
						comp->setSkeletonModel(std::dynamic_pointer_cast<Asset::SkeletonModel>(asset));
					}
					else if (asset->type() == IAsset::ParticleSystem) {
						QParticleSystemComponent* comp = entity->addComponent<QParticleSystemComponent>();
						comp->setParticleSystem(std::dynamic_pointer_cast<Asset::ParticleSystem>(asset));
					}
				}
				TheEngine->processEvents();
			}
		}
	}
	return QObject::eventFilter(watched, event);
}

