#ifndef QCubeGenerator_h__
#define QCubeGenerator_h__

#include "IPositionGenerator.h"
#include "Script\QLua.h"

class QCubeGenerator : public IPositionGenerator {
	Q_OBJECT
		Q_ENABLE_LUA
		Q_PROPERTY(float Width READ getWidth WRITE setWidth)
		Q_PROPERTY(float Height READ getHeight WRITE setHeight)
		Q_PROPERTY(float Depth READ getDepth WRITE setDepth)
		REGISTER_SUBCLASS(IPositionGenerator, QCubeGenerator);
public:
	Q_INVOKABLE QCubeGenerator() {};
	float getWidth() const { return mWidth; }
	void setWidth(float val) { mWidth = val; }
	float getHeight() const { return mHeight; }
	void setHeight(float val) { mHeight = val; }
	float getDepth() const { return mDepth; }
	void setDepth(float val) { mDepth = val; }
	void generate(QVector<QParticleSystem::Particle>& particles) override;
private:
	float mWidth = 10;
	float mHeight = 10;
	float mDepth = 10;
};

#endif // QCubeGenerator_h__
