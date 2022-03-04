#ifndef QParticleComponent_h__
#define QParticleComponent_h__

#include "QPrimitiveComponent.h"
#include "CustomShape\QCube.h"

class  QParticleComponent :public QPrimitiveComponent {
	Q_OBJECT
		Q_PROPERTY(QByteArray updater READ getUpdater WRITE setUpdater)
public:
	QParticleComponent();
	inline static const int PARTICLE_MAX_SIZE = 100;
	QSceneComponent::Type type() override {
		return QSceneComponent::Particle;
	}
	struct Particle {
		QVector3D position;
		uint32_t padding0;

		QVector3D rotation;
		uint32_t padding1;

		QVector3D scaling;
		uint32_t padding2;

		QVector3D velocity;
		float life;
	};
	struct ParticleBuffer {
		int counter = 0;
		int32_t padding[3];
		Particle particles[PARTICLE_MAX_SIZE];
	};

	QByteArray getUpdater() const;
	void setUpdater(QByteArray val);

	std::shared_ptr<QPrimitiveComponent> getShape() const { return mShape; }
	void setShape(std::shared_ptr<QPrimitiveComponent> val) { mShape = val; }

	void createPartilces(const QVector<Particle>& particles);
	QVector<Particle> takeParticles();

	uint8_t bNeedRecreatePipeline : 1 = 0;
private:
	QByteArray mUpdater;
	QVector<Particle> mParticlesPool;
	std::shared_ptr<QPrimitiveComponent> mShape;
};
#endif // QParticleComponent_h__
