#ifndef QSphereGenerator_h__
#define QSphereGenerator_h__

#include <QRandomGenerator>
#include "IPositionGenerator.h"
#include "Script\LuaRegister.h"

class QSphereGenerator : public IPositionGenerator {
	Q_OBJECT
		Q_ENABLE_LUA
		Q_PROPERTY(float Radius READ getRadius WRITE setRadius)
		Q_PROPERTY(bool Solid READ getSolid WRITE setSolid)
		REGISTER_SUBCLASS(IPositionGenerator, QSphereGenerator);
public:
	Q_INVOKABLE QSphereGenerator();;

	void generate(QVector<QParticleSystem::Particle>& particles) override;

	float getRadius() const { return mRadius; }
	void setRadius(float val) { mRadius = val; }

	bool getSolid() const { return mSolid; }
	void setSolid(bool val) { mSolid = val; }

private:
	float mRadius = 10;
	bool mSolid = true;
};

#endif // QSphereGenerator_h__
