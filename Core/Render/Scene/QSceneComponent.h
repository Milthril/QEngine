#ifndef QSceneComponent_h__
#define QSceneComponent_h__

#include "QObject"
#include "qvectornd.h"
#include "private/qrhi_p.h"

class QSceneComponent :public QObject {
	Q_OBJECT
		Q_PROPERTY(QVector3D position READ position WRITE setPosition)
		Q_PROPERTY(QVector3D rotation READ rotation WRITE setRotation)
		Q_PROPERTY(QVector3D scale READ scale WRITE setScale)
public:
	QSceneComponent() {}

	enum Type {
		None,
		Shape,
		StaticMesh,
		SkeletonMesh,
		Text2D,
		//Text3D,
		Light,
		Particle,
		SkyBox,
	};
	enum Flag {
		Visiable = 1 << 0,				//可见的
		Translucent = 1 << 1,			//半透明物体
		NeedRecreateProxy = 1 << 2,		//重建代理
	};
	Q_DECLARE_FLAGS(Flags, Flag);

	const QVector3D& position() const;
	void setPosition(const QVector3D& newPosition);
	const QVector3D& rotation() const;
	void setRotation(const QVector3D& newRotation);
	const QVector3D& scale() const;
	void setScale(const QVector3D& newScale);

	virtual QSceneComponent::Type type() = 0;
private:
	QVector3D mPosition;
	QVector3D mRotation;
	QVector3D mScale;
	Flags mFlags;
	uint32_t mId = 0;
	inline static uint32_t IdCounter = 0;
};

#endif // QSceneComponent_h__
