#ifndef ParticleSystem_h__
#define ParticleSystem_h__
#include <QObject>
#include "qvectornd.h"
#include "QParticleUpdater.h"
#include "Asset\IAsset.h"

class QParticleEmitter;

namespace Asset {
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

class ParticleSystem: public IAsset {
public:
	ParticleSystem();
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
	std::shared_ptr<QParticleEmitter> getEmitter() const { return mEmitter; }

	QString getStaticMeshPath() const { return mStaticMeshPath; }
	void setStaticMeshPath(QString val) { mStaticMeshPath = val; }

	virtual IAsset::Type type() override {
		return IAsset::ParticleSystem;
	}

	friend QDataStream& operator<<(QDataStream& out, const ParticleSystem& var);
	friend QDataStream& operator>>(QDataStream& in, ParticleSystem& var);
private:
	QString mStaticMeshPath;
	std::shared_ptr<QParticleEmitter> mEmitter;
	std::shared_ptr<QParticleUpdater> mUpdater;
};
}
Q_DECLARE_BUILTIN_METATYPE(Asset::ParticleSystem, 100003, Asset::ParticleSystem);

#endif // ParticleSystem_h__
