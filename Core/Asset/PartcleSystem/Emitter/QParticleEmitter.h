#ifndef QParticleEmitter_h__
#define QParticleEmitter_h__

#include "PositionGenerator\IPositionGenerator.h"
#include "VelocityGenerator\IVelocityGenerator.h"

class QParticleEmitter :public QObject {
	Q_OBJECT
		Q_PROPERTY(float Lifetime READ getLifetime WRITE setLifetime)
		Q_PROPERTY(int NumOfTick READ getNumOfTick WRITE setNumOfTick)
		Q_PROPERTY(QVector3D Scaling READ getScaling WRITE setScaling)
		Q_PROPERTY(QSubClass<IPositionGenerator> PositionGenerator READ getPositionGenerator WRITE setPositionGenerator)
		Q_PROPERTY(QSubClass<IVelocityGenerator> VelocityGenerator READ getVelocityGenerator WRITE setVelocityGenerator)
public:
	QParticleEmitter();

	using Particle = Asset::ParticleSystem::Particle;

	virtual QVector<Particle> update();

	float getLifetime() const { return mLifetime; }
	void setLifetime(float val) { mLifetime = val; }

	int getNumOfTick() const { return mNumOfTick; }
	void setNumOfTick(int val) { mNumOfTick = val; }

	QSubClass<IPositionGenerator> getPositionGenerator() const { return mPositionGenerator; }
	void setPositionGenerator(QSubClass<IPositionGenerator>  val) { mPositionGenerator = val; }

	QSubClass<IVelocityGenerator> getVelocityGenerator() const { return mVelocityGenerator; }
	void setVelocityGenerator(QSubClass<IVelocityGenerator> val) { mVelocityGenerator = val; }

	QVector3D getScaling() const { return mScaling; }
	void setScaling(QVector3D val) { mScaling = val; }

protected:
	float mLifetime = 2;
	int mNumOfTick = 100;
	QSubClass<IPositionGenerator> mPositionGenerator;
	QSubClass<IVelocityGenerator> mVelocityGenerator;
	QVector3D mScaling = QVector3D(0.01f, 0.01f, 0.01f);
};

#endif // QParticleEmitter_h__