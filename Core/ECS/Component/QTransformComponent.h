#ifndef QTransformComponent_h__
#define QTransformComponent_h__

#include "qvectornd.h"
#include "IComponent.h"

class QTransformComponent:public IComponent {
	Q_OBJECT
public:
	QVector3D getPosition() const { return mPosition; }
	void setPosition(QVector3D val) { mPosition = val; }

	QVector3D getRotation() const { return mRotation; }
	void setRotation(QVector3D val) { mRotation = val; }

	QVector3D getScale() const { return mScale; }
	void setScale(QVector3D val) { mScale = val; }

private:
	QVector3D mPosition = QVector3D(0.0f, 0.0f, 0.0f);
	QVector3D mRotation = QVector3D(0.0f, 0.0f, 0.0f);
	QVector3D mScale = QVector3D(1.0f, 1.0f, 1.0f);
};

#endif // QTransformComponent_h__
