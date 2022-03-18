#ifndef QRhiUniformMgr_h__
#define QRhiUniformMgr_h__

#include "QRhiUniform.h"

class QRhiUniformMgr {
	friend class QRhiUniform;
	friend class QSceneRenderer;
public:
	static QRhiUniformMgr* instance();
	void update(QRhiResourceUpdateBatch* batch);
private:
	QRhiUniformMgr() {}
	void AddUniform(QRhiUniform* material);
	void RemoveUniform(QRhiUniform* material);
private:
	QList<QRhiUniform*> mUniformList;
};

#endif // QRhiUniformMgr_h__
