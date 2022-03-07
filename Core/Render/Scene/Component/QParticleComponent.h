#ifndef QParticleComponent_h__
#define QParticleComponent_h__

#include "QPrimitiveComponent.h"
#include "CustomShape\QCube.h"

class  QParticleComponent :public QPrimitiveComponent {
	Q_OBJECT
		Q_PROPERTY(QByteArray updater READ getUpdater WRITE setUpdater)
		Q_PROPERTY(float lifetime READ getLifetime WRITE setLifetime)
public:
	QParticleComponent();

	inline static const int PARTICLE_MAX_SIZE = 100000;
	QSceneComponent::Type type() override {
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

	QByteArray getUpdater() const;
	void setUpdater(QByteArray val);

	float getLifetime() const { return mLifetime; }
	void setLifetime(float val) { mLifetime = val; }

	std::shared_ptr<QPrimitiveComponent> getShape() const { return mShape; }
	void setShape(std::shared_ptr<QPrimitiveComponent> val) { mShape = val; }

	void createPartilces(const QVector<Particle>& particles);
	QVector<Particle> takeParticles();

	virtual void setPosition(const QVector3D& newPosition) override;
	virtual void setRotation(const QVector3D& newRotation) override;
	virtual void setScale(const QVector3D& newScale) override;

private:
	QByteArray mUpdater;
	QVector<Particle> mParticlesPool;
	std::shared_ptr<QPrimitiveComponent> mShape;
	float mLifetime = 2;
};
#endif // QParticleComponent_h__
