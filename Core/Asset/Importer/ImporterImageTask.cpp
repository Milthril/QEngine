#include "ImporterImageTask.h"
#include "QImage"
#include "Asset\SkyBox.h"
#include "Utils\FileUtils.h"

void ImporterImageTask::executable() {
	QImage image(mFilePath);
	if (image.isNull())
		return;
	image = image.convertToFormat(QImage::Format::Format_RGBA8888);
	QSize mCubeFaceSize;
	if (image.width() == image.height())
		mCubeFaceSize = image.size();
	else if (image.width() * 3 == 4 * image.height())
		mCubeFaceSize = QSize(image.width() / 4, image.width() / 4);
	else if (image.width() * 4 == 3 * image.height())
		mCubeFaceSize = QSize(image.width() / 3, image.width() / 3);
	else
		return;
	std::shared_ptr<Asset::SkyBox> skyBox = std::make_shared<Asset::SkyBox>();
	std::array<QImage, 6> imageList;
	// 依次为左右上下前后
	imageList[0] = image.copy(QRect(QPoint(2 * mCubeFaceSize.width(), mCubeFaceSize.width()), mCubeFaceSize));
	imageList[1] = image.copy(QRect(QPoint(0, mCubeFaceSize.width()), mCubeFaceSize));

	imageList[2] = image.copy(QRect(QPoint(mCubeFaceSize.width(), 0), mCubeFaceSize));
	imageList[3] = image.copy(QRect(QPoint(mCubeFaceSize.width(), mCubeFaceSize.width() * 2), mCubeFaceSize));

	imageList[4] = image.copy(QRect(QPoint(mCubeFaceSize.width(), mCubeFaceSize.width()), mCubeFaceSize));
	imageList[5] = image.copy(QRect(QPoint(3 * mCubeFaceSize.width(), mCubeFaceSize.width()), mCubeFaceSize));

	skyBox->setImageList(imageList);
	QString vaildPath = FileUtils::getVaildPath(mDestDir.filePath(QString("%1.%2").arg(QFileInfo(mFilePath).baseName()).arg(skyBox->getExtName())));
	skyBox->setRefPath(vaildPath);
	skyBox->save(vaildPath);
}
