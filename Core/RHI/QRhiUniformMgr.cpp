#include "QRhiUniformMgr.h"

QRhiUniformMgr* QRhiUniformMgr::instance()
{
	static QRhiUniformMgr ins;
	return &ins;
}

void QRhiUniformMgr::update(QRhiResourceUpdateBatch* batch)
{
	for (auto& uniform : mUniformList) {
		const auto& proxy = uniform->getProxy();
		proxy->updateResource(batch);
	}
}

void QRhiUniformMgr::AddUniform(QRhiUniform* material)
{
	mUniformList << material;
}

void QRhiUniformMgr::RemoveUniform(QRhiUniform* material)
{
	mUniformList.removeOne(material);
}