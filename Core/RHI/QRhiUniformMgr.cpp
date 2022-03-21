#include "QRhiUniformMgr.h"
#include "QEngine.h"

QRhiUniformMgr* QRhiUniformMgr::instance()
{
	static QRhiUniformMgr ins;
	return &ins;
}

void QRhiUniformMgr::update(QRhiResourceUpdateBatch* batch)
{
	if (!mClipMatUniform) {
		mClipMatUniform.reset(RHI->newBuffer(QRhiBuffer::Dynamic, QRhiBuffer::UniformBuffer, sizeof(float) * 16));
		mClipMatUniform->create();
		const QMatrix4x4& matrix = RHI->clipSpaceCorrMatrix();
		batch->updateDynamicBuffer(mClipMatUniform.get(), 0, sizeof(float) * 16, matrix.data());
	}
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