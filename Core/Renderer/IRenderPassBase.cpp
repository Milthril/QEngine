#include "IRenderPassBase.h"

void IRenderPassBase::addSubPass(std::shared_ptr<IRenderPassBase> subPass)
{
	subPass->mDependencyList << this;
	mSubPassList << subPass;
}