#ifndef QSceneComponent_h__
#define QSceneComponent_h__

#include "QObject"
#include "qvectornd.h"
#include "private/qrhi_p.h"

class QSceneComponent :public QObject {
	Q_OBJECT
		Q_PROPERTY(QVector3D Position READ getPosition WRITE setPosition)
		Q_PROPERTY(QVector3D Rotation READ getRotation WRITE setRotation)
		Q_PROPERTY(QVector3D Scale READ getScale WRITE setScale)
public:
	QSceneComponent() {
		mId = IdCounter++;
	}
	using ComponentId = uint32_t;
	QSceneComponent::ComponentId componentId() const { return mId; }

	enum Type {
		None,
		Camera,
		Shape,
		StaticMesh,
		SkeletonMesh,
		Light,
		Particle,
		SkyBox,
	};

	uint8_t bVisiable : 1 = 0;
	uint8_t bTranslucent : 1 = 0;
	uint8_t bNeedResetProxy : 1 = 0;

	QMatrix4x4 calculateModelMatrix();

	const QVector3D& getPosition() const;
	virtual void setPosition(const QVector3D& newPosition);
	const QVector3D& getRotation() const;
	virtual void setRotation(const QVector3D& newRotation);
	const QVector3D& getScale() const;
	virtual void setScale(const QVector3D& newScale);
	virtual QSceneComponent::Type type() = 0;
private:
	QVector3D mPosition = QVector3D(0.0f, 0.0f, 0.0f);
	QVector3D mRotation = QVector3D(0.0f, 0.0f, 0.0f);
	QVector3D mScale = QVector3D(1.0f, 1.0f, 1.0f);
	ComponentId mId = 0;
	inline static ComponentId IdCounter = 0;
};

#endif // QSceneComponent_h__
