#ifndef QScenePrimitiveGenerator_h__
#define QScenePrimitiveGenerator_h__

#include <functional>
#include "IPositionGenerator.h"
#include "ExtType\QCombo.h"

class QSceneComponent;

class QScenePrimitiveGenerator : public IPositionGenerator {
	Q_OBJECT;
	Q_ENABLE_LUA;
	REGISTER_SUBCLASS(IPositionGenerator, QScenePrimitiveGenerator);
	Q_PROPERTY(QCombo CurrentPrimitive READ getCurrentPrimitive WRITE setCurrentPrimitive);
public:
	Q_INVOKABLE QScenePrimitiveGenerator() {};
	QCombo getCurrentPrimitive();
	void setCurrentPrimitive(QCombo val);
	void generate(QVector<QParticleSystem::Particle>& particles) override;
private:
	std::function<void(QVector<QParticleSystem::Particle>&)> mGenFunctor;
	QMap<QString, std::shared_ptr<QSceneComponent>> mCompCache;
};

#endif // QScenePrimitiveGenerator_h__