/********************************************************************************
** Form generated from reading UI file 'color_dialog.ui'
**
** Created by: Qt User Interface Compiler version 6.2.0
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_COLOR_DIALOG_H
#define UI_COLOR_DIALOG_H

#include <QtCore/QVariant>
#include <QtGui/QIcon>
#include <QtWidgets/QAbstractButton>
#include <QtWidgets/QApplication>
#include <QtWidgets/QDialog>
#include <QtWidgets/QDialogButtonBox>
#include <QtWidgets/QFrame>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QLabel>
#include <QtWidgets/QSpinBox>
#include <QtWidgets/QVBoxLayout>
#include "Component/GradientSlider.hpp"
#include "Component/HueSlider.hpp"
#include "Component/ColorLineEdit.hpp"
#include "Component/ColorPreview.hpp"
#include "Component/ColorWheel.hpp"
#include "Component/GradientBar.hpp"

QT_BEGIN_NAMESPACE

class Ui_ColorsDialog
{
public:
	QVBoxLayout* layout;
	QHBoxLayout* layout_main;
	QVBoxLayout* verticalLayout;
	QWidgetEx::ColorWheel* wheel;
	QWidgetEx::GradientBar* gradient_bar;
	QGridLayout* gridLayout;
	QWidgetEx::GradientSlider* slide_value;
	QLabel* label_7;
	QLabel* label_6;
	QWidgetEx::GradientSlider* slide_saturation;
	QLabel* label_8;
	QLabel* label_3;
	QWidgetEx::GradientSlider* slide_alpha;
	QWidgetEx::GradientSlider* slide_red;
	QWidgetEx::GradientSlider* slide_green;
	QLabel* label_5;
	QLabel* label_2;
	QLabel* label_alpha;
	QLabel* label;
	QWidgetEx::GradientSlider* slide_blue;
	QSpinBox* spin_hue;
	QSpinBox* spin_saturation;
	QSpinBox* spin_value;
	QSpinBox* spin_red;
	QSpinBox* spin_green;
	QSpinBox* spin_blue;
	QSpinBox* spin_alpha;
	QFrame* line_alpha;
	QFrame* line;
	QFrame* line_3;
	QWidgetEx::HueSlider* slide_hue;
	QWidgetEx::ColorLineEdit* edit_hex;

	void setupUi(QDialog* GradientDialog)
	{
		if (GradientDialog->objectName().isEmpty())
			GradientDialog->setObjectName(QString::fromUtf8("GradientDialog"));
		GradientDialog->resize(491, 380);
		layout = new QVBoxLayout(GradientDialog);
		layout->setSpacing(6);
		layout->setContentsMargins(11, 11, 11, 11);
		layout->setObjectName(QString::fromUtf8("layout"));
		layout_main = new QHBoxLayout();
		layout_main->setSpacing(6);
		layout_main->setObjectName(QString::fromUtf8("layout_main"));
		verticalLayout = new QVBoxLayout();
		verticalLayout->setSpacing(6);
		verticalLayout->setObjectName(QString::fromUtf8("verticalLayout"));
		wheel = new QWidgetEx::ColorWheel(GradientDialog);
		wheel->setObjectName(QString::fromUtf8("wheel"));
		QSizePolicy sizePolicy(QSizePolicy::MinimumExpanding, QSizePolicy::MinimumExpanding);
		sizePolicy.setHorizontalStretch(0);
		sizePolicy.setVerticalStretch(0);
		sizePolicy.setHeightForWidth(wheel->sizePolicy().hasHeightForWidth());
		wheel->setSizePolicy(sizePolicy);

		verticalLayout->addWidget(wheel);

		gradient_bar = new QWidgetEx::GradientBar(GradientDialog);
		gradient_bar->setObjectName(QString::fromUtf8("preview"));
		verticalLayout->addWidget(gradient_bar);

		layout_main->addLayout(verticalLayout);

		gridLayout = new QGridLayout();
		gridLayout->setSpacing(6);
		gridLayout->setObjectName(QString::fromUtf8("gridLayout"));
		slide_value = new QWidgetEx::GradientSlider(GradientDialog);
		slide_value->setObjectName(QString::fromUtf8("slide_value"));
		slide_value->setMaximum(255);
		slide_value->setOrientation(Qt::Horizontal);

		gridLayout->addWidget(slide_value, 2, 1, 1, 1);

		label_7 = new QLabel(GradientDialog);
		label_7->setObjectName(QString::fromUtf8("label_7"));

		gridLayout->addWidget(label_7, 1, 0, 1, 1);

		label_6 = new QLabel(GradientDialog);
		label_6->setObjectName(QString::fromUtf8("label_6"));

		gridLayout->addWidget(label_6, 0, 0, 1, 1);

		slide_saturation = new QWidgetEx::GradientSlider(GradientDialog);
		slide_saturation->setObjectName(QString::fromUtf8("slide_saturation"));
		slide_saturation->setMaximum(255);
		slide_saturation->setOrientation(Qt::Horizontal);

		gridLayout->addWidget(slide_saturation, 1, 1, 1, 1);

		label_8 = new QLabel(GradientDialog);
		label_8->setObjectName(QString::fromUtf8("label_8"));

		gridLayout->addWidget(label_8, 10, 0, 1, 1);

		label_3 = new QLabel(GradientDialog);
		label_3->setObjectName(QString::fromUtf8("label_3"));

		gridLayout->addWidget(label_3, 6, 0, 1, 1);

		slide_alpha = new QWidgetEx::GradientSlider(GradientDialog);
		slide_alpha->setObjectName(QString::fromUtf8("slide_alpha"));
		slide_alpha->setMaximum(255);
		slide_alpha->setOrientation(Qt::Horizontal);

		gridLayout->addWidget(slide_alpha, 8, 1, 1, 1);

		slide_red = new QWidgetEx::GradientSlider(GradientDialog);
		slide_red->setObjectName(QString::fromUtf8("slide_red"));
		slide_red->setMaximum(255);
		slide_red->setOrientation(Qt::Horizontal);

		gridLayout->addWidget(slide_red, 4, 1, 1, 1);

		slide_green = new QWidgetEx::GradientSlider(GradientDialog);
		slide_green->setObjectName(QString::fromUtf8("slide_green"));
		slide_green->setMaximum(255);
		slide_green->setOrientation(Qt::Horizontal);

		gridLayout->addWidget(slide_green, 5, 1, 1, 1);

		label_5 = new QLabel(GradientDialog);
		label_5->setObjectName(QString::fromUtf8("label_5"));

		gridLayout->addWidget(label_5, 2, 0, 1, 1);

		label_2 = new QLabel(GradientDialog);
		label_2->setObjectName(QString::fromUtf8("label_2"));

		gridLayout->addWidget(label_2, 5, 0, 1, 1);

		label_alpha = new QLabel(GradientDialog);
		label_alpha->setObjectName(QString::fromUtf8("label_alpha"));

		gridLayout->addWidget(label_alpha, 8, 0, 1, 1);

		label = new QLabel(GradientDialog);
		label->setObjectName(QString::fromUtf8("label"));

		gridLayout->addWidget(label, 4, 0, 1, 1);

		slide_blue = new QWidgetEx::GradientSlider(GradientDialog);
		slide_blue->setObjectName(QString::fromUtf8("slide_blue"));
		slide_blue->setMaximum(255);
		slide_blue->setOrientation(Qt::Horizontal);

		gridLayout->addWidget(slide_blue, 6, 1, 1, 1);

		spin_hue = new QSpinBox(GradientDialog);
		spin_hue->setObjectName(QString::fromUtf8("spin_hue"));
		spin_hue->setWrapping(true);
		spin_hue->setMaximum(359);

		gridLayout->addWidget(spin_hue, 0, 2, 1, 1);

		spin_saturation = new QSpinBox(GradientDialog);
		spin_saturation->setObjectName(QString::fromUtf8("spin_saturation"));
		spin_saturation->setMaximum(255);

		gridLayout->addWidget(spin_saturation, 1, 2, 1, 1);

		spin_value = new QSpinBox(GradientDialog);
		spin_value->setObjectName(QString::fromUtf8("spin_value"));
		spin_value->setMaximum(255);

		gridLayout->addWidget(spin_value, 2, 2, 1, 1);

		spin_red = new QSpinBox(GradientDialog);
		spin_red->setObjectName(QString::fromUtf8("spin_red"));
		spin_red->setMaximum(255);

		gridLayout->addWidget(spin_red, 4, 2, 1, 1);

		spin_green = new QSpinBox(GradientDialog);
		spin_green->setObjectName(QString::fromUtf8("spin_green"));
		spin_green->setMaximum(255);

		gridLayout->addWidget(spin_green, 5, 2, 1, 1);

		spin_blue = new QSpinBox(GradientDialog);
		spin_blue->setObjectName(QString::fromUtf8("spin_blue"));
		spin_blue->setMaximum(255);

		gridLayout->addWidget(spin_blue, 6, 2, 1, 1);

		spin_alpha = new QSpinBox(GradientDialog);
		spin_alpha->setObjectName(QString::fromUtf8("spin_alpha"));
		spin_alpha->setMaximum(255);

		gridLayout->addWidget(spin_alpha, 8, 2, 1, 1);

		line_alpha = new QFrame(GradientDialog);
		line_alpha->setObjectName(QString::fromUtf8("line_alpha"));
		line_alpha->setFrameShape(QFrame::HLine);
		line_alpha->setFrameShadow(QFrame::Sunken);

		gridLayout->addWidget(line_alpha, 7, 0, 1, 3);

		line = new QFrame(GradientDialog);
		line->setObjectName(QString::fromUtf8("line"));
		line->setFrameShape(QFrame::HLine);
		line->setFrameShadow(QFrame::Sunken);

		gridLayout->addWidget(line, 3, 0, 1, 3);

		line_3 = new QFrame(GradientDialog);
		line_3->setObjectName(QString::fromUtf8("line_3"));
		line_3->setFrameShape(QFrame::HLine);
		line_3->setFrameShadow(QFrame::Sunken);

		gridLayout->addWidget(line_3, 9, 0, 1, 3);

		slide_hue = new QWidgetEx::HueSlider(GradientDialog);
		slide_hue->setObjectName(QString::fromUtf8("slide_hue"));
		slide_hue->setMinimum(0);
		slide_hue->setMaximum(359);

		gridLayout->addWidget(slide_hue, 0, 1, 1, 1);

		edit_hex = new QWidgetEx::ColorLineEdit(GradientDialog);
		edit_hex->setObjectName(QString::fromUtf8("edit_hex"));
		QFont font;
		font.setFamilies({ QString::fromUtf8("Monospace") });
		edit_hex->setFont(font);
		edit_hex->setProperty("showAlpha", QVariant(true));

		gridLayout->addWidget(edit_hex, 10, 1, 1, 2);

		layout_main->addLayout(gridLayout);

		layout->addLayout(layout_main);

		retranslateUi(GradientDialog);
		QObject::connect(slide_saturation, SIGNAL(valueChanged(int)), GradientDialog, SLOT(set_hsv()));
		QObject::connect(slide_value, SIGNAL(valueChanged(int)), GradientDialog, SLOT(set_hsv()));
		QObject::connect(slide_red, SIGNAL(valueChanged(int)), GradientDialog, SLOT(set_rgb()));
		QObject::connect(slide_green, SIGNAL(valueChanged(int)), GradientDialog, SLOT(set_rgb()));
		QObject::connect(slide_blue, SIGNAL(valueChanged(int)), GradientDialog, SLOT(set_rgb()));
		QObject::connect(slide_alpha, SIGNAL(valueChanged(int)), GradientDialog, SLOT(set_alpha()));
		QObject::connect(wheel, SIGNAL(colorSelected(QColor)), GradientDialog, SLOT(setColorInternal(QColor)));
		QObject::connect(slide_saturation, SIGNAL(valueChanged(int)), spin_saturation, SLOT(setValue(int)));
		QObject::connect(spin_saturation, SIGNAL(valueChanged(int)), slide_saturation, SLOT(setValue(int)));
		QObject::connect(slide_value, SIGNAL(valueChanged(int)), spin_value, SLOT(setValue(int)));
		QObject::connect(spin_value, SIGNAL(valueChanged(int)), slide_value, SLOT(setValue(int)));
		QObject::connect(slide_red, SIGNAL(valueChanged(int)), spin_red, SLOT(setValue(int)));
		QObject::connect(spin_red, SIGNAL(valueChanged(int)), slide_red, SLOT(setValue(int)));
		QObject::connect(slide_green, SIGNAL(valueChanged(int)), spin_green, SLOT(setValue(int)));
		QObject::connect(spin_green, SIGNAL(valueChanged(int)), slide_green, SLOT(setValue(int)));
		QObject::connect(slide_alpha, SIGNAL(valueChanged(int)), spin_alpha, SLOT(setValue(int)));
		QObject::connect(spin_alpha, SIGNAL(valueChanged(int)), slide_alpha, SLOT(setValue(int)));
		QObject::connect(slide_blue, SIGNAL(valueChanged(int)), spin_blue, SLOT(setValue(int)));
		QObject::connect(spin_blue, SIGNAL(valueChanged(int)), slide_blue, SLOT(setValue(int)));
		QObject::connect(slide_hue, SIGNAL(valueChanged(int)), spin_hue, SLOT(setValue(int)));
		QObject::connect(spin_hue, SIGNAL(valueChanged(int)), slide_hue, SLOT(setValue(int)));
		QObject::connect(slide_hue, SIGNAL(valueChanged(int)), GradientDialog, SLOT(set_hsv()));

		QMetaObject::connectSlotsByName(GradientDialog);
	} // setupUi

	void retranslateUi(QDialog* GradientDialog)
	{
		GradientDialog->setWindowTitle(QCoreApplication::translate("GradientDialog", "Select Color", nullptr));
		label_7->setText(QCoreApplication::translate("GradientDialog", "Saturation", nullptr));
		label_6->setText(QCoreApplication::translate("GradientDialog", "Hue", nullptr));
		label_8->setText(QCoreApplication::translate("GradientDialog", "Hex", nullptr));
		label_3->setText(QCoreApplication::translate("GradientDialog", "Blue", nullptr));
		label_5->setText(QCoreApplication::translate("GradientDialog", "Value", nullptr));
		label_2->setText(QCoreApplication::translate("GradientDialog", "Green", nullptr));
		label_alpha->setText(QCoreApplication::translate("GradientDialog", "Alpha", nullptr));
		label->setText(QCoreApplication::translate("GradientDialog", "Red", nullptr));
	} // retranslateUi
};

namespace Ui {
class GradientDialog : public Ui_ColorsDialog {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_COLOR_DIALOG_H