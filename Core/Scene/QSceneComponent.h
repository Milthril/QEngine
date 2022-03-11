#ifndef QSceneComponent_h__
#define QSceneComponent_h__

#include "QObject"
#include "qvectornd.h"
#include "QMatrix4x4"
#include "RHI\QRhiSignal.h"

class QSceneComponent :public QObject {
	friend class QSceneRenderer;
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

	enum ProxyType {
		None,
		Camera,
		StaticMesh,
		SkeletonMesh,
		Light,
		Particle,
		SkyBox,
	};

	QRhiSignal bNeedResetProxy;

	QMatrix4x4 calculateModelMatrix();
	QMatrix4x4 calculateWorldMatrix();

	const QVector3D& getPosition() const;
	virtual void setPosition(const QVector3D& newPosition);
	const QVector3D& getRotation() const;
	virtual void setRotation(const QVector3D& newRotation);
	const QVector3D& getScale() const;
	virtual void setScale(const QVector3D& newScale);
	virtual QSceneComponent::ProxyType type() = 0;
protected:
	void addChild(std::shared_ptr<QSceneComponent> child);
	void removeChild(std::shared_ptr<QSceneComponent> child);
	void clear();
private:
	QVector3D mPosition = QVector3D(0.0f, 0.0f, 0.0f);
	QVector3D mRotation = QVector3D(0.0f, 0.0f, 0.0f);
	QVector3D mScale = QVector3D(1.0f, 1.0f, 1.0f);
	ComponentId mId = 0;
	inline static ComponentId IdCounter = 0;
	QSceneComponent* mParent = nullptr;
	QList<std::shared_ptr<QSceneComponent>> mChildren;
};

#endif // QSceneComponent_h__
