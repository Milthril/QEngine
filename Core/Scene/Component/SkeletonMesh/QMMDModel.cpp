#include "QMMDModel.h"
#include "..\AssimpToolkit\MMDVmdParser.h"

QVMDAnimation::QVMDAnimation(QSkeleton* skeleton)
	:QSkeletonAnimation(skeleton)
{
}

void QVMDAnimation::loadFromFile(const QString& path)
{
	std::unique_ptr<vmd::VmdMotion> motion = vmd::VmdMotion::LoadFromFile(path.toLocal8Bit().data());
	for (uint i = 0; i < motion->bone_frames.size(); i++) {
		const auto& boneFrame = motion->bone_frames[i];
		QSkeletonAnimation::Node& skNode = mAnimNode[boneFrame.name];
		skNode.translation[boneFrame.frame] = boneFrame.position;
		skNode.rotation[boneFrame.frame] = boneFrame.orientation;
		skNode.scaling[boneFrame.frame] = QVector3D(1, 1, 1);
	}
}

void QMMDModel::attachVMDFile(const QString& filePath)
{
	std::shared_ptr<QVMDAnimation> vmd = std::make_shared<QVMDAnimation>(mSkeleton.get());
	vmd->loadFromFile(filePath);
	mSkeleton->addAnimation(vmd);
}