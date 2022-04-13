#include "QPropertyItemFactory.h"
#include "QPropertyAdjusterItem.h"
#include "QPropertySubClassItem.h"

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
#include "ExtType/QBoundedDouble.h"
#include "ExtType/QBoundedInt.h"
#include "ExtType/QColor4D.h"
#include "ExtType/QColors.h"

#define REGISTER_ADJUSTER_ITEM(Type,AdjusterType)\
		mCreatorMap[QMetaTypeId2<Type>::qt_metatype_id()] = [](QString name, Getter getter, Setter setter) { \
			return new QPropertyAdjusterItem(name, getter, setter, new AdjusterType(getter().value<Type>()));\
		};

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
	//REGISTER_ADJUSTER_ITEM(std::shared_ptr<QMaterial>, MaterialButton);
	//REGISTER_ADJUSTER_ITEM(std::shared_ptr<QParticleSystem>, ParticleSystemButton);

	//mCreatorMap[QMetaTypeId2<QSubClass<IPositionGenerator>>::qt_metatype_id()] = [](QString name, Getter getter, Setter setter) {
	//	return new QPropertySubClassItem<QSubClass<IPositionGenerator>>(name, getter, setter);
	//};

	//mCreatorMap[QMetaTypeId2<QSubClass<QStaticMeshComponent>>::qt_metatype_id()] = [](QString name, Getter getter, Setter setter) {
	//	return new QPropertySubClassItem<QSubClass<QStaticMeshComponent>>(name, getter, setter);
	//};
}

QPropertyItem* QPropertyItemFactory::createItem(TypeId id, QString name, Getter getter, Setter setter)
{
	auto creator = mCreatorMap.find(id);
	if (creator != mCreatorMap.end()) {
		return (*creator)(name, getter, setter);
	}
	return nullptr;
}