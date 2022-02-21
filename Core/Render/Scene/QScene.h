#ifndef QScene_h__
#define QScene_h__

#include <QList>
#include <QObject>
#include <QVector3D>
#include "QSceneComponent.h"

class QScene {
public:

private:
	QList<std::shared_ptr<QVisibleComponent>> mVisibleList;
	QList<std::shared_ptr<QLightComponent>> mLightList;
	std::shared_ptr<QSkyBoxComponent> mSkyBox;
	std::shared_ptr<QCameraComponent> mCamera;
};

#endif // QScene_h__