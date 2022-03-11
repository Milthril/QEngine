#include "QRhiUniformMgr.h"

QRhiUniformMgr* QRhiUniformMgr::instance()
{
	static QRhiUniformMgr ins;
	return &ins;
}

void QRhiUniformMgr::update(QRhiResourceUpdateBatch* batch)
{
	for (auto& uniform : mUniformMap) {
		const auto& proxy = uniform->getProxy();
		proxy->updateResource(batch);
	}
}

void QRhiUniformMgr::AddUniform(QRhiUniform* material)
{
	mUniformMap << material;
}

void QRhiUniformMgr::RemoveUniform(QRhiUniform* material)
{
	mUniformMap.removeOne(material);
}