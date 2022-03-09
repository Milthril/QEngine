#ifndef QSkyBoxComponent_h__
#define QSkyBoxComponent_h__

#include "QPrimitiveComponent.h"

class QSkyBoxComponent :public QSceneComponent {
	Q_OBJECT
		Q_PROPERTY(QImage SkyBoxImage READ getSkyBoxImage WRITE setSkyBoxImage)

public:
	QSceneComponent::Type type() override { return QSceneComponent::SkyBox; }
public:
	QSkyBoxComponent(QImage texture = QImage());
	const QImage& getSkyBoxImage()const;
	void setSkyBoxImage(QImage val);
	QSize getImageSize() const;
	QSize getCubeFaceSize() const;
	std::array<QImage, 6> getSubImageArray();
private:
	QImage mSkyBoxImage;
	QSize mCubeFaceSize;
};

#endif // QSkyBoxComponent_h__
