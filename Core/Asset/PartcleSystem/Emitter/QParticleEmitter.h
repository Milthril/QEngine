#ifndef QParticleEmitter_h__
#define QParticleEmitter_h__

#include "PositionGenerator\IPositionGenerator.h"

class QParticleEmitter :public QObject {
	Q_OBJECT
		Q_PROPERTY(float Lifetime READ getLifetime WRITE setLifetime)
		Q_PROPERTY(int NumOfTick READ getNumOfTick WRITE setNumOfTick)
		Q_PROPERTY(QVector3D Scaling READ getScaling WRITE setScaling)
		Q_PROPERTY(IPositionGenerator* PositionGenerator READ getPositionGenerator WRITE setPositionGenerator)
public:
	QParticleEmitter();

	using Particle = Asset::ParticleSystem::Particle;

	virtual QVector<Particle> update();

	float getLifetime() const { return mLifetime; }
	void setLifetime(float val) { mLifetime = val; }

	int getNumOfTick() const { return mNumOfTick; }
	void setNumOfTick(int val) { mNumOfTick = val; }

	IPositionGenerator* getPositionGenerator() const { return mPositionGenerator.get(); }
	void setPositionGenerator(IPositionGenerator* val) { mPositionGenerator = val; }

	QVector3D getScaling() const { return mScaling; }
	void setScaling(QVector3D val) { mScaling = val; }

protected:
	float mLifetime = 2;
	int mNumOfTick = 100;
	QSubClass<IPositionGenerator> mPositionGenerator;
	QVector3D mScaling = QVector3D(0.1f, 0.1f, 0.1f);
};

#endif // QParticleEmitter_h__