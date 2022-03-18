#ifndef IParticleEmitter_h__
#define IParticleEmitter_h__

#include "..\QParticleSystem.h"

class IParticleEmitter :public QObject {
	Q_OBJECT
		Q_PROPERTY(float Lifetime READ getLifetime WRITE setLifetime)
		Q_PROPERTY(int NumOfTick READ getNumOfTick WRITE setNumOfTick)
		Q_PROPERTY(QVector3D Scaling READ getScaling WRITE setScaling)
public:
	IParticleEmitter() {}
	using Particle = QParticleSystem::Particle;

	virtual QVector<Particle> update();
	virtual void handParticle(Particle& particle) = 0;

	float getLifetime() const { return mLifetime; }
	void setLifetime(float val) { mLifetime = val; }

	QVector3D getScaling() const { return mScaling; }
	void setScaling(QVector3D val) { mScaling = val; }

	int getNumOfTick() const { return mNumOfTick; }
	void setNumOfTick(int val) { mNumOfTick = val; }
protected:
	float mLifetime = 2;
	int mNumOfTick = (100);
	QVector3D mScaling = QVector3D(1, 1, 1);
};

class SubClass {
};

#endif // IParticleEmitter_h__
