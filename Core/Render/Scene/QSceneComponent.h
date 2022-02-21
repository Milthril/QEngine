#ifndef QSceneComponent_h__
#define QSceneComponent_h__

#include "QObject"
#include "qvectornd.h"
#include "private/qrhi_p.h"

class QSceneComponent :public QObject {
public:
	QSceneComponent() {}

	enum class Type {
		None,
		Primitive,
		StaticMesh,
		SkeletonMesh,
		Text2D,
		//Text3D,
		Light,
		Particle,
		SkyBox,
	};

	enum Flag {
		Translucent = 1 << 0
	};
	Q_DECLARE_FLAGS(Flags, Flag);
protected:
	virtual QSceneComponent::Type ComponentType() = 0;
private:
	QVector3D mPosition;
	QVector3D mRotation;
	QVector3D mScale;
	uint32_t mId = 0;
	inline static uint32_t IdCounter = 0;
};

class QVisibleComponent :public QSceneComponent {
};

class QLightComponent;
class QCameraComponent;
class QSkyBoxComponent;

#endif // QSceneComponent_h__