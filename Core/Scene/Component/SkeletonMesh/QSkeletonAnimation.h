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
	void loadFromAssimp(aiAnimation* anim);
	struct Node {
		QMap<double, QVector3D> translation;
		QMap<double, QQuaternion> rotation;
		QMap<double, QVector3D> scaling;
		QMatrix4x4 getMatrix(const double& timeMs);
	};
	QVector<QSkeleton::BoneMatrix> calcBoneMatrix(const double& timeMs);
protected:
	QMap<QString, Node> mAnimNode;
	QSkeleton* mSkeleton;
	double mDuration;
	double mTicksPerSecond;
};

#endif // QSkeletonAnimation_h__
