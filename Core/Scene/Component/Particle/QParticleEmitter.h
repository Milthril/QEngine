#ifndef QParticleEmitter_h__
#define QParticleEmitter_h__

#include "QParticleSystem.h"
#include "PositionGenerator\IPositionGenerator.h"

class QParticleEmitter :public QObject {
	Q_OBJECT
		Q_PROPERTY(float Lifetime READ getLifetime WRITE setLifetime)
		Q_PROPERTY(int NumOfTick READ getNumOfTick WRITE setNumOfTick)
		Q_PROPERTY(QVector3D Scaling READ getScaling WRITE setScaling)
		Q_PROPERTY(QSubClass<IPositionGenerator> PositionGenerator READ getPositionGenerator WRITE setPositionGenerator)
public:
	QParticleEmitter();
	using Particle = QParticleSystem::Particle;

	virtual QVector<Particle> update();

	float getLifetime() const { return mLifetime; }
	void setLifetime(float val) { mLifetime = val; }

	int getNumOfTick() const { return mNumOfTick; }
	void setNumOfTick(int val) { mNumOfTick = val; }

	QSubClass<IPositionGenerator> getPositionGenerator() const { return mPositionGenerator; }
	void setPositionGenerator(QSubClass<IPositionGenerator> val) { mPositionGenerator = val; }

	QVector3D getScaling() const { return mScaling; }
	void setScaling(QVector3D val) { mScaling = val; }

protected:
	float mLifetime = 2;
	int mNumOfTick = 100;
	QSubClass<IPositionGenerator> mPositionGenerator;
	QVector3D mScaling = QVector3D(1, 1, 1);
};

#endif // QParticleEmitter_h__