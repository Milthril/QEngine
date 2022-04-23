#ifndef IVelocityGenerator_h__
#define IVelocityGenerator_h__

#include "QObject"
#include "qvectornd.h"
#include "ExtType\QSubClass.h"
#include <QRandomGenerator>
#include "Asset\PartcleSystem\ParticleSystem.h"

class IVelocityGenerator :public QObject {
	Q_OBJECT
public:
	virtual void generate(QVector<Asset::ParticleSystem::Particle>& particle) = 0;
	inline static QRandomGenerator mRandom;
};

Q_DECLARE_METATYPE(QSubClass<IVelocityGenerator>)

#endif // IVelocityGenerator_h__
