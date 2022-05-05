#include "QSubClass.h"

QSubClassMgr* QSubClassMgr::instance()
{
	static QSubClassMgr ins;
	return &ins;
}

void QSubClassMgr::registerInfo(const QMetaObject* parent, const QMetaObject* child)
{
	if (parent && child) {
		mInfo[parent->className()].parent = parent;
		mInfo[parent->className()].children << child;
	}
}

QSubClassMgr::Info QSubClassMgr::getInfo(const QString& parentName)
{
	return mInfo.value(parentName);
}

QStringList QSubClassMgr::getSubList(const QString& parentName)
{
	const QSubClassMgr::Info& info = mInfo[parentName];
	QStringList subList;
	for (auto& meta : info.children) {
		QString name = meta->className();
		name = name.split("::").back();
		subList << name;
	}
	return subList;
}
