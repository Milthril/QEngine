#ifndef QSkeletonAnimation_h__
#define QSkeletonAnimation_h__

#include "QMap"
#include "qvectornd.h"
#include "QQuaternion"
#include "assimp\anim.h"
#include "QMatrix4x4"
#include "QSkeleton.h"

class QSkeletonAnimation {
public:
	QSkeletonAnimation(QSkeleton* skeleton);
	void show(const double& timeMs);
	void loadFromAssimp(aiAnimation* anim);
	double getTicksPerSecond() const { return mTicksPerSecond; }
	double getDuration() const { return mDuration; }
	struct Node {
		QMap<double, QVector3D> translation;
		QMap<double, QQuaternion> rotation;
		QMap<double, QVector3D> scaling;
		QMatrix4x4 getMatrix(const double& timeMs);
	};
private:
	QVector<QSkeleton::Mat4> calcBoneMatrix(const double& timeMs);
protected:
	QMap<QString, Node> mAnimNode;
	QSkeleton* mSkeleton;
	double mDuration;
	double mTicksPerSecond;
};

#endif // QSkeletonAnimation_h__
