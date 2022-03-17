#ifndef QPointEmitter_h__
#define QPointEmitter_h__

#include "IParticleEmitter.h"

class QPointEmitter : public IParticleEmitter {
public:
	virtual QVector<Particle> update() override;
	QVector3D getPostion() const { return mPostion; }
	void setPostion(QVector3D val) { mPostion = val; }
private:
	QVector3D mPostion;
};

#endif // QPointEmitter_h__