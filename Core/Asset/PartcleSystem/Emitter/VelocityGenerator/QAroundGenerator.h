#ifndef QAroundGenerator_h__
#define QAroundGenerator_h__

#include "IVelocityGenerator.h"

class QAroundGenerator : public IVelocityGenerator {
	Q_OBJECT
		Q_ENABLE_LUA
		Q_PROPERTY(QVector3D Width READ getStrength WRITE setStrength)
		REGISTER_SUBCLASS(IVelocityGenerator, QAroundGenerator);
public:
	Q_INVOKABLE QAroundGenerator() {};
	void generate(QVector<Asset::ParticleSystem::Particle>& particles) override;
	QVector3D getStrength() const { return mStrength; }
	void setStrength(QVector3D val) { mStrength = val; }
private:
	QVector3D mStrength = QVector3D(1, 1, 1);
};

#endif // QAroundGenerator_h__
