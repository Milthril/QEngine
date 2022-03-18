#include "QPropertyItem.h"
#include <QBoxLayout>
#include <QHBoxLayout>
#include <QLabel>
#include <QLineEdit>
#include "AdjusterFactory.h"
#include "Adjuster/Adjuster.h"

QPropertyItem::QPropertyItem(QObject* object, QMetaProperty property)
	: object_(object)
	, property_(property)
	, itemWidget_(new QWidget)
	, name_(new QLabel)
{
	itemWidget_->setAttribute(Qt::WA_TranslucentBackground, true);
	layout_ = new QHBoxLayout(itemWidget_);
	layout_->setAlignment(Qt::AlignCenter);
	layout_->setContentsMargins(10, 2, 10, 2);
	layout_->addWidget(name_, 100, Qt::AlignLeft);
	name_->setText(QString(property.name()).replace("_", " "));
	name_->setStyleSheet("background-color:rgba(0,0,0,0)");
}

QPropertyItem::~QPropertyItem()
{
	if (itemWidget_)
		itemWidget_->deleteLater();
}

QWidget* QPropertyItem::createWidget()
{
	if (object_ == nullptr)
		return nullptr;
	return AdjusterFactory::create(object_, property_);
}

void QPropertyItem::setUp(QTreeWidgetItem* tree)
{
	QWidget* widget = createWidget();
	if (widget != nullptr) {
		layout_->addWidget(widget, 0, Qt::AlignRight);
	}
	tree->addChild(this);
	treeWidget()->setItemWidget(this, 0, itemWidget_);
}

void QPropertyItem::setUp(QTreeWidget* tree)
{
	QWidget* widget = createWidget();
	if (widget != nullptr) {
		layout_->addWidget(widget, 0, Qt::AlignRight);
	}
	tree->addTopLevelItem(this);
	treeWidget()->setItemWidget(this, 0, itemWidget_);
}

void QPropertyItem::rename()
{
	QLineEdit* lineEdit = new QLineEdit;
	//treeWidget()->setItemWidget(this,0,lineEdit);
	//lineEdit->setText(text(0));
	//lineEdit->selectAll();
	//lineEdit->setFocus();
	//connect(lineEdit,&QLineEdit::editingFinished,this,[this,lineEdit](){
	//    QVariant var=getVar();
	//    ikObject_->setProperty(ikVar_.name(),QVariant());    //remove
	//    ikObject_->setProperty(lineEdit->text().toLocal8Bit().data(),var);
	//    int newIndex=ikObject_->metaObject()->indexOfProperty(lineEdit->text().toLocal8Bit().data());
	//    ikVar_=ikObject_->metaObject()->property(newIndex);
	//    treeWidget()->removeItemWidget(this,0);
	//    lineEdit->deleteLater();
	//});
}