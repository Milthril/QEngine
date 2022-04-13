#include "Adjuster/DoubleBox.h"
#include "Adjuster/ImageLoader.h"
#include "Adjuster/Vec2Box.h"
#include "Adjuster/Vec3Box.h"
#include "Adjuster/Vec4Box.h"
#include "QLineEdit"
#include "QTreeWidget"
#include "QVBoxLayout"
#include "UniformPanel.h"
#include "QPushButton"
#include "QMenu"
#include "QApplication"
#include "QClipboard"
#include "Window/ScriptEditor/QScriptEditor.h"
#include "ECS/System/RenderSystem/RHI/QRhiUniform.h"

UniformPanel::UniformPanel()
	: mParamsPanel(new QTreeWidget)
	, btNewParam(new QPushButton("New Param"))
{
	QHBoxLayout* h = new QHBoxLayout;
	QVBoxLayout* v = new QVBoxLayout(this);
	h->addWidget(btNewParam);
	v->addLayout(h);
	v->addWidget(mParamsPanel);
	mParamsPanel->setColumnCount(1);
	mParamsPanel->setIndentation(8);
	mParamsPanel->setHeaderHidden(true);
	mParamsPanel->setContextMenuPolicy(Qt::CustomContextMenu);
	mParamsPanel->setSelectionMode(QAbstractItemView::SingleSelection);
	mParamsPanel->setFrameStyle(QFrame::NoFrame);

	connect(mParamsPanel, &QTreeWidget::itemDoubleClicked, this, [this](QTreeWidgetItem* item) {
		QGuiApplication::clipboard()->setText("UBO." + item->text(1));
	});

	connect(mParamsPanel, &QTreeWidget::itemPressed, this, [this](QTreeWidgetItem* item, int) {
		if (qApp->mouseButtons() & Qt::RightButton) {
			if (!mUniform)
				return;
			QMenu menu;
			menu.addAction("Rename", [this, item]() {
				QWidget* widget = mParamsPanel->itemWidget(item, 0);
				if (widget != nullptr) {
					QLineEdit* edit = dynamic_cast<QLineEdit*>(widget->layout()->itemAt(0)->widget());
					if (edit) {
						edit->setEnabled(true);
						edit->selectAll();
						edit->setFocus();
					}
				}
			});
			menu.addAction("Remove", [this, item]() {
				mUniform->removeParam(item->text(1));
				updateParamPanel();
			});
			menu.exec(QCursor::pos());
		}
	});
	connect(btNewParam, &QPushButton::clicked, this, [this]() {
		if (!mUniform)
			return;
		QMenu menu;
		menu.addAction("Float", [this]() {
			mUniform->addDataFloat("Float", 0.0f);
			updateParamPanel();
		});
		menu.addAction("Vec2", [this]() {
			mUniform->addDataVec2("Vec2", QVector2D());
			updateParamPanel();
		});
		menu.addAction("Vec3", [this]() {
			mUniform->addDataVec3("Vec3", QVector3D());
			updateParamPanel();
		});
		menu.addAction("Vec4", [this]() {
			mUniform->addDataVec4("Vec4", QVector4D());
			updateParamPanel();
		});
		menu.addAction("Sampler2D", [this]() {
			mUniform->addTextureSampler("Texture", QImage());
			updateParamPanel();
		});
		menu.exec(QCursor::pos());
	});
}

void UniformPanel::setUniform(std::shared_ptr<QRhiUniform> uniform)
{
	mUniform = uniform;
	updateParamPanel();
}

void UniformPanel::updateParamPanel() {
	mParamsPanel->clear();
	auto params = mUniform->getParamsDesc();
	for (auto param : params) {
		QTreeWidgetItem* item = new QTreeWidgetItem();
		item->setText(1, param->name);
		QWidget* itemWidget = new QWidget;
		itemWidget->setAttribute(Qt::WA_TranslucentBackground, true);
		QHBoxLayout* h = new QHBoxLayout(itemWidget);
		QLineEdit* lineEdit = new QLineEdit(param->name);
		lineEdit->setEnabled(false);
		connect(lineEdit, &QLineEdit::editingFinished, this, [lineEdit, item, this]() {
			if (mUniform->renameParma(item->text(1), lineEdit->text())) {
				updateParamPanel();
			}
			lineEdit->setEnabled(false);
		});
		h->addWidget(lineEdit, 0, Qt::AlignLeft);
		Adjuster* adjuster = nullptr;
		switch (param->type)
		{
		case QRhiUniform::ParamDescBase::Type::Float: {
			adjuster = new DoubleBox(mUniform->getData<float>(param->name));
			connect(adjuster, &Adjuster::valueChanged, this, [this, param](QVariant var) {
				mUniform->setData<float>(param->name, var.toDouble());
			});
			break;
		}
		case QRhiUniform::ParamDescBase::Type::Vec2: {
			adjuster = new Vec2Box(mUniform->getData<QVector2D>(param->name));
			connect(adjuster, &Adjuster::valueChanged, this, [this, param](QVariant var) {
				mUniform->setData<QVector2D>(param->name, var.value<QVector2D>());
			});
			break;
		}
		case QRhiUniform::ParamDescBase::Type::Vec3: {
			adjuster = new Vec3Box(mUniform->getData<QVector3D>(param->name));
			connect(adjuster, &Adjuster::valueChanged, this, [this, param](QVariant var) {
				mUniform->setData<QVector3D>(param->name, var.value<QVector3D>());
			});
			break;
		}
		case QRhiUniform::ParamDescBase::Type::Vec4: {
			adjuster = new Vec4Box(mUniform->getData<QVector4D>(param->name));
			connect(adjuster, &Adjuster::valueChanged, this, [this, param](QVariant var) {
				mUniform->setData<QVector4D>(param->name, var.value<QVector4D>());
			});
			break;
		}
		case QRhiUniform::ParamDescBase::Type::Mat4:
			break;
		case QRhiUniform::ParamDescBase::Type::Sampler2D: {
			auto texDesc = std::dynamic_pointer_cast<QRhiUniform::TextureDesc>(mUniform->getParamDesc(param->name));;
			adjuster = new ImageLoader(texDesc->image);
			connect(adjuster, &Adjuster::valueChanged, this, [this, param](QVariant var) {
				mUniform->setTextureSampler(param->name, var.value<QImage>());
			});
			break;
		}

		default:
			break;
		}
		h->addWidget(adjuster, 1);
		mParamsPanel->addTopLevelItem(item);
		mParamsPanel->setItemWidget(item, 0, itemWidget);
	}
}