#include "QPropertyItemFactory.h"
#include "QPropertyAdjusterItem.h"
#include "QPropertySubClassItem.h"
#include "QPropertyVectorItem.h"

#include "Adjuster\BoolBox.h"
#include "Adjuster\ByteArrayLoader.h"
#include "Adjuster\Color\ColorButton.hpp"
#include "Adjuster\Color\ColorDialog.hpp"
#include "Adjuster\Color\ColorsButton.hpp"
#include "Adjuster\Color\ColorsDialog.hpp"
#include "Adjuster\Color\QColor4DButton.hpp"
#include "Adjuster\ComboBox.h"
#include "Adjuster\DoubleBox.h"
#include "Adjuster\DoubleBox.h"
#include "Adjuster\DoubleSlider.h"
#include "Adjuster\ImageLoader.h"
#include "Adjuster\IntBox.h"
#include "Adjuster\IntSlider.h"
#include "Adjuster\LineEdit.h"
#include "Adjuster\MaterialButton.h"
#include "Adjuster\ParticleSystemButton.h"
#include "Adjuster\RangeSlider.h"
#include "Adjuster\Vec2Box.h"
#include "Adjuster\Vec3Box.h"
#include "Adjuster\Vec4Box.h"
#include "Adjuster\AssetBox.h"

#include "ExtType\QBoundedDouble.h"
#include "ExtType\QBoundedInt.h"
#include "ExtType\QColor4D.h"
#include "ExtType\QColors.h"

#include "Asset\Material.h"
#include "Asset\SkyBox.h"
#include "Asset\StaticMesh.h"
#include "Asset\PartcleSystem\ParticleSystem.h"
#include "Asset\SkeletonModel\SkeletonModel.h"


#define REGISTER_ADJUSTER_ITEM(Type,AdjusterType)\
		mCreatorMap[QMetaTypeId2<Type>::qt_metatype_id()] = [](QString name, Getter getter, Setter setter) { \
			return new QPropertyAdjusterItem(name, getter, setter, new AdjusterType(getter().value<Type>()));\
		};

#define REGISTER_ASSET_ITEM(AssetType)\
		mCreatorMap[QMetaTypeId2<std::shared_ptr<AssetType>>::qt_metatype_id()] = [](QString name, Getter getter, Setter setter) { \
			Getter newGetter = [getter]() {\
				std::shared_ptr<AssetType> iasset = getter().value<std::shared_ptr<AssetType> >();\
				return QVariant::fromValue(std::dynamic_pointer_cast<IAsset>(iasset));\
			};\
			Setter newSetter = [setter](QVariant var) {\
				std::shared_ptr<IAsset> iasset = var.value<std::shared_ptr<IAsset> >();\
				setter(QVariant::fromValue(std::dynamic_pointer_cast<AssetType>(iasset)));\
			};\
			return new QPropertyAdjusterItem(name, newGetter, newSetter, new AssetBox(getter().value<std::shared_ptr<AssetType>>(),I##AssetType));\
		}



QPropertyItemFactory* QPropertyItemFactory::instance()
{
	static QPropertyItemFactory ins;
	return &ins;
}


QPropertyItemFactory::QPropertyItemFactory()
{
	REGISTER_ADJUSTER_ITEM(double, DoubleBox);
	REGISTER_ADJUSTER_ITEM(float, DoubleBox);
	REGISTER_ADJUSTER_ITEM(int, IntBox);
	REGISTER_ADJUSTER_ITEM(bool, BoolBox);
	REGISTER_ADJUSTER_ITEM(QString, LineEdit);
	REGISTER_ADJUSTER_ITEM(QBoundedDouble, DoubleSlider);
	REGISTER_ADJUSTER_ITEM(QBoundedInt, IntSlider);
	REGISTER_ADJUSTER_ITEM(QColor, ColorButton);
	REGISTER_ADJUSTER_ITEM(QColor4D, QColor4DButton);
	REGISTER_ADJUSTER_ITEM(QColors, ColorsButton);
	REGISTER_ADJUSTER_ITEM(QVector2D, Vec2Box);
	REGISTER_ADJUSTER_ITEM(QVector3D, Vec3Box);
	REGISTER_ADJUSTER_ITEM(QVector4D, Vec4Box);
	REGISTER_ADJUSTER_ITEM(QCombo, ComboBox);
	REGISTER_ADJUSTER_ITEM(QRange, RangeSlider);
	REGISTER_ADJUSTER_ITEM(QByteArray, ByteArrayLoader);
	REGISTER_ADJUSTER_ITEM(QImage, ImageLoader);

	REGISTER_ASSET_ITEM(Asset::Material);
	REGISTER_ASSET_ITEM(Asset::SkyBox);
	REGISTER_ASSET_ITEM(Asset::StaticMesh);
	REGISTER_ASSET_ITEM(Asset::ParticleSystem);
	REGISTER_ASSET_ITEM(Asset::SkeletonModel);


	//REGISTER_ADJUSTER_ITEM(std::shared_ptr<QMaterial>, MaterialButton);
	//REGISTER_ADJUSTER_ITEM(std::shared_ptr<QParticleSystem>, ParticleSystemButton);

	//mCreatorMap[QMetaTypeId2<QSubClass<IPositionGenerator>>::qt_metatype_id()] = [](QString name, Getter getter, Setter setter) {
	//	return new QPropertySubClassItem<QSubClass<IPositionGenerator>>(name, getter, setter);
	//};

	//mCreatorMap[QMetaTypeId2<QSubClass<QStaticMeshComponent>>::qt_metatype_id()] = [](QString name, Getter getter, Setter setter) {
	//	return new QPropertySubClassItem<QSubClass<QStaticMeshComponent>>(name, getter, setter);
	//};

	mCreatorMap[QMetaTypeId2<QVector<std::shared_ptr<Asset::Material>>>::qt_metatype_id()] = [](QString name, Getter getter, Setter setter) {
		return new QPropertyVectorItem<QVector<std::shared_ptr<Asset::Material>>>(name, getter, setter);
	};
}

QPropertyItem* QPropertyItemFactory::createItem(QMetaType metaType, QString name, Getter getter, Setter setter) {
	auto creator = mCreatorMap.find(metaType.id());

	if (creator != mCreatorMap.end()) {
		return (*creator)(name, getter, setter);
	}

	if (QMetaType::canConvert(metaType, QMetaType::fromType<QObject*>())) {		//处理Object*属性
		QObject* obj = getter().value<QObject*>();
		if (obj != nullptr) {
			return nullptr;
		}
	}

	if (QMetaType::canConvert(metaType, QMetaType::fromType<QVariantList>())) {	//处理数组类型
		QTreeWidgetItem* item = new QTreeWidgetItem;
		QVariantList varList = getter().toList();
		return nullptr;
	}
	return nullptr;
}

//if (QMetaType::canConvert(property.metaType(), QMetaType::fromType<QObject*>())) {		//处理Object*属性
//	QObject* obj = property.read(mObject).value<QObject*>();
//	if (obj != nullptr) {
//		QTreeWidgetItem* item = new QTreeWidgetItem;
//		item->setText(0, property.name());
//		addTopLevelItem(item);
//		setupObjectToItem(item, obj);
//	}
//}
//if (QMetaType::canConvert(property.metaType(), QMetaType::fromType<QVariantList>())) {	//处理数组类型
//	QTreeWidgetItem* item = new QTreeWidgetItem;
//	item->setText(0, property.name());
//	addTopLevelItem(item);
//	QVariantList varList = property.read(mObject).toList();
//	for (int i = 0; i < varList.size(); i++) {
//		QPropertyItem* listItem = QPropertyItemFactory::instance()->createItem(varList[i].typeId(),
//																			   QString::number(i),
//																			   [this, property, i]() {
//			return property.read(mObject).toList()[i];
//		},
//																			   [this, property, i](QVariant var) {
//			QVariantList varList = property.read(mObject).toList();
//			varList[i] = var;
//			property.write(mObject, varList);
//		});
//		if (listItem) {
//			item->addChild(listItem);
//			listItem->createWidgetOrSubItem();
//		}
//	}
//}