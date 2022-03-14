#ifndef QMMDModel_h__
#define QMMDModel_h__

#include "QSkeletonAnimation.h"
#include "QSkeletonMeshComponent.h"

class QVMDAnimation :public QSkeletonAnimation {
public:
	QVMDAnimation(QSkeleton* skeleton);
	void loadFromFile(const QString& path);
};

class QMMDModel :public QSkeletonModelComponent {
public:
	void attachVMDFile(const QString& filePath);
};

#endif // QMMDModel_h__
