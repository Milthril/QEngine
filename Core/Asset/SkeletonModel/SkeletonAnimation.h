#ifndef SkeletonAnimation_h__
#define SkeletonAnimation_h__

#include "Asset\IAsset.h"
#include <QMap>
#include <qvectornd.h>
#include <QQuaternion>
#include <QMatrix4x4>
#include "Skeleton.h"

namespace Asset {

class SkeletonAnimation : public IAsset {
	friend class ImporterModelTask;
public:
	struct Node {
		QMap<double, QVector3D> translation;
		QMap<double, QQuaternion> rotation;
		QMap<double, QVector3D> scaling;
		
		QMatrix4x4 getMatrix(const double& timeMs);
	};
	double getTicksPerSecond() const { return mTicksPerSecond; }
	double getDuration() const { return mDuration; }
	QByteArray getSkeletonSHA() const { return mSkeletonSHA; }
	const QMap<QString, Asset::SkeletonAnimation::Node>& getAnimNode() const { return mAnimNode; }

	virtual void serialize(QDataStream& out) override;
	virtual void deserialize(QDataStream& in) override;

	virtual IAsset::Type type() override {
		return IAsset::SkeletonAnimation;
	}
	void play(Asset::Skeleton* skeleton, double timeMs);
private:
	QByteArray mSkeletonSHA;
	QMap<QString, Node> mAnimNode;
	double mDuration;
	double mTicksPerSecond;
};

}

Q_ENGINE_DECLARE_ASSET_TYPE(Asset::SkeletonAnimation, IAsset::SkeletonAnimation);

#endif // SkeletonAnimation_h__
