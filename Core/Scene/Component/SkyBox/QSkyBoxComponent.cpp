#include "QSkyBoxComponent.h"

QSkyBoxComponent::QSkyBoxComponent(QImage texture /*= QImage()*/)
{
	setScale(QVector3D(1000.0f, 1000.0f, 1000.0f));
}

const QImage& QSkyBoxComponent::getSkyBoxImage() const
{
	return mSkyBoxImage;
}

void QSkyBoxComponent::setSkyBoxImage(QImage val)
{
	mSkyBoxImage = val.convertedTo(QImage::Format_RGBA8888);
	if (mSkyBoxImage.width() == mSkyBoxImage.height())
		mCubeFaceSize = mSkyBoxImage.size();
	else if (mSkyBoxImage.width() * 3 == 4 * mSkyBoxImage.height())
		mCubeFaceSize = QSize(mSkyBoxImage.width() / 4, mSkyBoxImage.width() / 4);
	else if (mSkyBoxImage.width() * 4 == 3 * mSkyBoxImage.height())
		mCubeFaceSize = QSize(mSkyBoxImage.width() / 3, mSkyBoxImage.width() / 3);
}

QSize QSkyBoxComponent::getImageSize() const
{
	return mSkyBoxImage.size();
}

QSize QSkyBoxComponent::getCubeFaceSize() const
{
	return mCubeFaceSize;
}

std::array<QImage, 6> QSkyBoxComponent::getSubImageArray()
{
	auto it = mSkyBoxImage.format();
	std::array<QImage, 6> imageList;

	// 依次为左右上下前后
	imageList[0] = mSkyBoxImage.copy(QRect(QPoint(2 * mCubeFaceSize.width(), mCubeFaceSize.width()), mCubeFaceSize));
	imageList[1] = mSkyBoxImage.copy(QRect(QPoint(0, mCubeFaceSize.width()), mCubeFaceSize));

	imageList[2] = mSkyBoxImage.copy(QRect(QPoint(mCubeFaceSize.width(), 0), mCubeFaceSize));
	imageList[3] = mSkyBoxImage.copy(QRect(QPoint(mCubeFaceSize.width(), mCubeFaceSize.width() * 2), mCubeFaceSize));

	imageList[4] = mSkyBoxImage.copy(QRect(QPoint(mCubeFaceSize.width(), mCubeFaceSize.width()), mCubeFaceSize));
	imageList[5] = mSkyBoxImage.copy(QRect(QPoint(3 * mCubeFaceSize.width(), mCubeFaceSize.width()), mCubeFaceSize));
	return imageList;
}