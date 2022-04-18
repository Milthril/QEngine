#ifndef SkeletonAnimation_h__
#define SkeletonAnimation_h__

#include "Asset\IAsset.h"

namespace Asset {

class SkletonAnimation : public IAsset {
public:
	struct Node {
		QMap<double, QVector3D> translation;
		QMap<double, QQuaternion> rotation;
		QMap<double, QVector3D> scaling;
	};
private:
	QByteArray mSkeletonSHA;
	QMap<QString, Node> mAnimNode;
	double mDuration;
	double mTicksPerSecond;
};
}

#endif // SkeletonAnimation_h__
