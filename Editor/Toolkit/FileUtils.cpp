#include "FileUtils.h"
#include <QFileInfo>
#include <QDesktopServices>
#include <QDir>
#include <QProcess>
#include <QUrl>

void FileUtils::showInFolder(const QString& path) {
	QFileInfo info(path);
#if defined(Q_OS_WIN)
	QStringList args;
	if (!info.isDir())
		args << "/select,";
	args << QDir::toNativeSeparators(path);
	if (QProcess::startDetached("explorer", args))
		return;
#elif defined(Q_OS_MAC)
	QStringList args;
	args << "-e";
	args << "tell application \"Finder\"";
	args << "-e";
	args << "activate";
	args << "-e";
	args << "select POSIX file \"" + path + "\"";
	args << "-e";
	args << "end tell";
	args << "-e";
	args << "return";
	if (!QProcess::execute("/usr/bin/osascript", args))
		return;
#endif
	QDesktopServices::openUrl(QUrl::fromLocalFile(info.isDir() ? path : info.path()));
}

QString FileUtils::getVaildPath(QString path) {
	if (!QFile::exists(path))
		return path;
	QFileInfo info(path);
	QString baseName = info.baseName();
	QString newPath;
	int index = 1;
	do {
		baseName = info.baseName()+ QString::number(index++);
		newPath = info.dir().filePath(baseName +"." + info.suffix());
	} while (QFile::exists(newPath));
	return newPath;
}
