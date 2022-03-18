#ifndef QCubeEmitter_h__
#define QCubeEmitter_h__

#include "IParticleEmitter.h"
#include <QRandomGenerator>
class QCubeEmitter : public IParticleEmitter {
	Q_OBJECT
		Q_PROPERTY(float Width READ getWidth WRITE setWidth)
		Q_PROPERTY(float Height READ getHeight WRITE setHeight)
		Q_PROPERTY(float Depth READ getDepth WRITE setDepth)
public:
	float getWidth() const { return mWidth; }
	void setWidth(float val) { mWidth = val; }
	float getHeight() const { return mHeight; }
	void setHeight(float val) { mHeight = val; }
	float getDepth() const { return mDepth; }
	void setDepth(float val) { mDepth = val; }
	void handParticle(Particle& particle) override;
private:
	float mWidth = 1;
	float mHeight = 1;
	float mDepth = 1;
	QRandomGenerator mRandom;
};

#endif // QCubeEmitter_h__
