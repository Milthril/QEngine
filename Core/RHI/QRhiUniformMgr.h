#ifndef QRhiUniformMgr_h__
#define QRhiUniformMgr_h__

#include "QRhiUniform.h"

class QRhiUniformMgr {
	friend class QRhiUniform;
	friend class ISceneRenderer;
public:
	static QRhiUniformMgr* instance();
	void update(QRhiResourceUpdateBatch* batch);
private:
	QRhiUniformMgr() {}
	void AddUniform(QRhiUniform* material);
	void RemoveUniform(QRhiUniform* material);
private:
	QList<QRhiUniform*> mUniformList;
	QRhiSPtr<QRhiBuffer> mClipMatUniform;
};

#endif // QRhiUniformMgr_h__
