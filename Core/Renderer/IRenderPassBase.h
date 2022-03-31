#ifndef IRenderPassBase_h__
#define IRenderPassBase_h__

#include "RHI/QRhiDefine.h"

class IRenderPassBase {
public:
	virtual void compile() {}
	virtual void execute() {}
	void addSubPass(std::shared_ptr<IRenderPassBase> subPass);
private:
	std::function<void()> mFuncSetup;
	QList<IRenderPassBase*> mDependencyList;
	QList<std::shared_ptr<IRenderPassBase>> mSubPassList;
};

#endif // IRenderPassBase_h__
