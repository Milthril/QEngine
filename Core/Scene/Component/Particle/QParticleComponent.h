#ifndef QParticleComponent_h__
#define QParticleComponent_h__

#include "QParticleSystem.h"
#include "ExtType\QSubClass.h"
#include "..\StaticMesh\QStaticMeshComponent.h"

class QParticleComponent :public QPrimitiveComponent {
	Q_OBJECT
		Q_PROPERTY(std::shared_ptr<QParticleSystem> ParticleSystem READ getParticleSystem WRITE setParticleSystem)
		Q_PROPERTY(QSubClass<QStaticMeshComponent>StaticMesh READ getStaticMesh WRITE setStaticMesh)
public:
	QParticleComponent();

	QSceneComponent::ProxyType type() override {
		return QSceneComponent::Particle;
	}

	const QSubClass<QStaticMeshComponent>& getStaticMesh() const { return mStaticMesh; }
	void setStaticMesh(QSubClass<QStaticMeshComponent> val);

	std::shared_ptr<QParticleSystem> getParticleSystem() const { return mParticleSystem; }
	void setParticleSystem(std::shared_ptr<QParticleSystem> val) { mParticleSystem = val; }

	virtual void setPosition(const QVector3D& newPosition) override;
	virtual void setRotation(const QVector3D& newRotation) override;
	virtual void setScale(const QVector3D& newScale) override;
private:
	QSubClass<QStaticMeshComponent> mStaticMesh;
	std::shared_ptr<QParticleSystem> mParticleSystem = std::make_shared<QParticleSystem>();
};
#endif // QParticleComponent_h__
