#include "UniformPanel.h"
#include "QVBoxLayout"
#include "QTreeWidget"
#include "RHI/QRhiUniform.h"
#include "Adjuster/DoubleBox.h"
#include "Adjuster/Vec2Box.h"
#include "Adjuster/Vec3Box.h"
#include "Adjuster/Vec4Box.h"

UniformPanel::UniformPanel(std::shared_ptr<QRhiUniform> uniform)
	:mUniform(uniform)
	,mParamsPanel(new QTreeWidget)
{
	QVBoxLayout* v = new QVBoxLayout(this);
	v->addWidget(mParamsPanel);
	mParamsPanel->setColumnCount(2);
	mParamsPanel->setIndentation(8);
	mParamsPanel->setHeaderHidden(true);
	mParamsPanel->setContextMenuPolicy(Qt::CustomContextMenu);
	mParamsPanel->setColumnWidth(0, 120);
	mParamsPanel->setSelectionMode(QAbstractItemView::SingleSelection);
	mParamsPanel->setFrameStyle(QFrame::NoFrame);

	updateParamPanel();
}

void UniformPanel::updateParamPanel() {
	mParamsPanel->clear();
	auto params = mUniform->getParams();
	for (auto param : params) {
		QTreeWidgetItem* item = new QTreeWidgetItem({ param.name });
		Adjuster* adjuster;
		switch (param.type)
		{
		case QRhiUniform::QParamDesc::Type::Float: {
			adjuster = new DoubleBox(mUniform->getParam<float>(param.name));
			connect(adjuster, &Adjuster::valueChanged, this, [this, param](QVariant var) {
				mUniform->setParam<float>(param.name, var.toDouble());
			});
			break;
		}
		case QRhiUniform::QParamDesc::Type::Vec2: {
			adjuster = new Vec2Box(mUniform->getParam<QVector2D>(param.name));
			connect(adjuster, &Adjuster::valueChanged, this, [this, param](QVariant var) {
				mUniform->setParam<QVector2D>(param.name, var.value<QVector2D>());
			});
			break;
		}
		case QRhiUniform::QParamDesc::Type::Vec3: {
			adjuster = new Vec3Box(mUniform->getParam<QVector3D>(param.name));
			connect(adjuster, &Adjuster::valueChanged, this, [this, param](QVariant var) {
				mUniform->setParam<QVector3D>(param.name, var.value<QVector3D>());
			});
			break;
		}
		case QRhiUniform::QParamDesc::Type::Vec4: {
			adjuster = new Vec4Box(mUniform->getParam<QVector4D>(param.name));
			connect(adjuster, &Adjuster::valueChanged, this, [this, param](QVariant var) {
				mUniform->setParam<QVector4D>(param.name, var.value<QVector4D>());
			});
			break;
		}
		case QRhiUniform::QParamDesc::Type::Mat4:
			break;
		default:
			break;
		}
		mParamsPanel->addTopLevelItem(item);
		mParamsPanel->setItemWidget(item, 1, adjuster);
	}
}