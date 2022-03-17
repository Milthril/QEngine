#include "MaterialButton.h"
#include "Scene/Material/QMaterial.h"
#include <QPushButton>
#include <QComboBox>
#include <QHBoxLayout>
#include "Window/MaterialEditor/QMaterialEditor.h"

MaterialButton::MaterialButton(std::shared_ptr<QMaterial> material)
	: mMaterial(material)
	, btEdit(new QPushButton("Edit"))
	, cbMaterialList(new QComboBox)
{
	QHBoxLayout* h = new QHBoxLayout(this);
	h->addWidget(cbMaterialList);
	h->addWidget(btEdit);

	connect(btEdit, &QPushButton::clicked, this, [this]() {
		QMaterialEditor::instance()->edit(mMaterial);
	});
}

QVariant MaterialButton::getValue()
{
	return 0;
}

void MaterialButton::setValue(QVariant var)
{
}