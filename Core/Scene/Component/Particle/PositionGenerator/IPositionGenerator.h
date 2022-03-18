#ifndef IPositionGenerator_h__
#define IPositionGenerator_h__

#include "QObject"
#include "qvectornd.h"
#include "ExtType\QSubClass.h"
#include <QRandomGenerator>
#include "Scene\Component\Particle\QParticleSystem.h"

class IPositionGenerator :public QObject {
	Q_OBJECT
public:
	virtual void generate(QVector<QParticleSystem::Particle>& particle) = 0;
	inline static QRandomGenerator mRandom;
};

Q_DECLARE_METATYPE(QSubClass<IPositionGenerator>)

#endif // IPositionGenerator_h__
