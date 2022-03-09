#include "QSkyBoxComponent.h"

QSkyBoxComponent::QSkyBoxComponent(QImage texture /*= QImage()*/)
{
	setScale(QVector3D(1000.0f, 1000.0f, 1000.0f));
}

QImage QSkyBoxComponent::getSkyBoxImage() const
{
	return mSkyBoxImage;
}

void QSkyBoxComponent::setSkyBoxImage(QImage val)
{
	mSkyBoxImage = val;
}
