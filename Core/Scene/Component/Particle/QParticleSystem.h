#ifndef QParticleSystem_h__
#define QParticleSystem_h__
#include <QObject>
#include "qvectornd.h"
#include "QParticleUpdater.h"

class IParticleEmitter;

inline const char* getParticleDefine() {
	return R"(
#extension GL_ARB_separate_shader_objects : enable
#define LOCAL_SIZE 256
#define PARTICLE_MAX_SIZE 1000000
layout (local_size_x = LOCAL_SIZE) in;
struct Particle {
	vec3 position;
	vec3 rotation;
	vec3 scaling;
	vec3 velocity;
	float life;
};
)";
}

class QParticleSystem {
	Q_GADGET

public:
	QParticleSystem();
	inline static const int PARTICLE_MAX_SIZE = 1000000;
	struct Particle {
		QVector4D position;
		QVector4D rotation;
		QVector4D scaling = QVector3D(1.0f, 1.0f, 1.0f).toVector4D();
		QVector3D velocity;
		float life = 0.0;
	};
	struct ParticleBuffer {
		uint32_t counter = 0;
		uint32_t padding[3];
		Particle particles[PARTICLE_MAX_SIZE];
	};
	std::shared_ptr<QParticleUpdater> getUpdater() const { return mUpdater; }
	std::shared_ptr<IParticleEmitter> getEmitter() const { return mEmitter; }
private:
	std::shared_ptr<IParticleEmitter> mEmitter;
	std::shared_ptr<QParticleUpdater> mUpdater = std::make_shared<QParticleUpdater>();
};

Q_DECLARE_METATYPE(std::shared_ptr<QParticleSystem>)

#endif // QParticleSystem_h__
