#ifndef FromComponent_h__
#define FromComponent_h__

#include "IPositionGenerator.h"
#include "Script\QLua.h"
#include "ExtType\QCombo.h"

class IComponent;

namespace QPositionGenerator {

class FromComponent : public IPositionGenerator {
	Q_OBJECT
		Q_PROPERTY(QCombo Component READ getComponent WRITE setComponent);
		REGISTER_SUBCLASS(IPositionGenerator, FromComponent)
public:
	Q_INVOKABLE FromComponent() {};
	QCombo getComponent();
	void setComponent(QCombo val);
	void setComponent(IComponent* component);
	void generate(QVector<Asset::ParticleSystem::Particle>& particles) override;
private:
	std::function<void(QVector<Asset::ParticleSystem::Particle>&)> mGenFunctor;
	QMap<QString, IComponent*> mCompCache;
};
}


#endif // FromComponent_h__
