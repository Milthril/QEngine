//#include "ParticleSystemButton.h"
//#include <QPushButton>
//#include <QComboBox>
//#include <QHBoxLayout>
//#include "Scene/Component/Particle/QParticleSystem.h"
//#include "Window/ParticlesEditor/QParticlesEditor.h"
//
//ParticleSystemButton::ParticleSystemButton(std::shared_ptr<QParticleSystem> system)
//	: mSystem(system)
//	, btEdit(new QPushButton("Edit"))
//	, cbList(new QComboBox)
//{
//	QHBoxLayout* h = new QHBoxLayout(this);
//	h->addWidget(cbList);
//	h->addWidget(btEdit);
//	connect(btEdit, &QPushButton::clicked, this, [this]() {
//		QParticlesEditor::instance()->edit(mSystem);
//	});
//}
//
//QVariant ParticleSystemButton::getValue()
//{
//	return 0;
//}
//
//void ParticleSystemButton::setValue(QVariant var)
//{
//}