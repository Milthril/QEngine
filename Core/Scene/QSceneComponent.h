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

	QString getCompIdVec4String() const { 
		int r = (mId & 0x000000FF) >> 0;
		int g = (mId & 0x0000FF00) >> 8;
		int b = (mId & 0x00FF0000) >> 16;
		int a = (mId & 0xFF000000) >> 24;
		return QString("vec4(%1,%2,%3,%4)").arg(r / 255.0f).arg(g / 255.0f).arg(b / 255.0f).arg(a / 255.0f);
	}

	enum ProxyType {
		None,
		Camera,
		StaticMesh,
		SkeletonMesh,
		Light,
		Particle,
		SkyBox,
	};

	QMatrix4x4 calculateModelMatrix();
	QMatrix4x4 calculateWorldMatrix();

	const QVector3D& getPosition() const;
	virtual void setPosition(const QVector3D& newPosition);
	const QVector3D& getRotation() const;
	virtual void setRotation(const QVector3D& newRotation);
	const QVector3D& getScale() const;
	virtual void setScale(const QVector3D& newScale);
	virtual QSceneComponent::ProxyType type() = 0;

	const QList<std::shared_ptr<QSceneComponent>>& getChildren() const { return mChildren; }

	QRhiSignal bNeedRecreateResource;
	QRhiSignal bNeedRecreatePipeline;
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
