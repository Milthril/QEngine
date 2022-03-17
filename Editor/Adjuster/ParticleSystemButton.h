#ifndef ParticleSystemButton_h__
#define ParticleSystemButton_h__

#include"Adjuster/Adjuster.h"

class QComboBox;
class QPushButton;
class QParticleSystem;

class ParticleSystemButton :public Adjuster {
public:
	ParticleSystemButton(std::shared_ptr<QParticleSystem> system);
	QVariant getValue() override;
	void setValue(QVariant var) override;
protected:
	QComboBox* cbList;
	QPushButton* btEdit;
	std::shared_ptr<QParticleSystem> mSystem;
};

#endif // ParticleSystemButton_h__
