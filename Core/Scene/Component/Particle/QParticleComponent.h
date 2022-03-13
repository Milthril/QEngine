#ifndef QParticleComponent_h__
#define QParticleComponent_h__

#include "Scene\Component\QPrimitiveComponent.h"
#include "QParticleUpdater.h"

class QParticleComponent :public QPrimitiveComponent {
	Q_OBJECT
		Q_PROPERTY(float lifetime READ getLifetime WRITE setLifetime)
public:
	QParticleComponent();

	inline static const int PARTICLE_MAX_SIZE = 1000000;
	QSceneComponent::ProxyType type() override {
		return QSceneComponent::Particle;
	}

	struct Particle {
		QVector4D position;
		QVector4D rotation;
		QVector4D scaling = QVector3D(1.0f, 1.0f, 1.0f).toVector4D();
		QVector3D velocity;
		float life = 0.0;
	};

	struct ParticleBuffer {
		Particle particles[PARTICLE_MAX_SIZE];
	};

	QParticleUpdater& updater() { return mUpdater; }
	void setUpdater(QParticleUpdater val) { mUpdater = val; }

	float getLifetime() const { return mLifetime; }
	void setLifetime(float val) { mLifetime = val; }

	std::shared_ptr<QPrimitiveComponent> getStaticMesh() const { return mStaticMesh; }
	void setStaticMesh(std::shared_ptr<QPrimitiveComponent> val) { mStaticMesh = val; }

	void createPartilces(const QVector<Particle>& particles);
	QVector<Particle> takeParticles();

	virtual void setPosition(const QVector3D& newPosition) override;
	virtual void setRotation(const QVector3D& newRotation) override;
	virtual void setScale(const QVector3D& newScale) override;

	static QString getParticleDefine();
private:
	QParticleUpdater mUpdater;
	QVector<Particle> mParticlesPool;
	std::shared_ptr<QPrimitiveComponent> mStaticMesh;
	float mLifetime = 2;
};
#endif // QParticleComponent_h__
