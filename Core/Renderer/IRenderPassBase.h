#ifndef QRenderPass_h__
#define QRenderPass_h__

#include "RHI/QRhiDefine.h"

class QRenderPass {
public:
	virtual void compile() {}
	virtual void execute() {}
	void addSubPass(std::shared_ptr<QRenderPass> subPass);
private:
	std::function<void()> mFuncSetup;
	QList<QRenderPass*> mDependencyList;
	QList<std::shared_ptr<QRenderPass>> mSubPassList;
};

#endif // QRenderPass_h__
