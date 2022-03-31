#include "QRenderPass.h"

void QRenderPass::addSubPass(std::shared_ptr<QRenderPass> subPass)
{
	subPass->mDependencyList << this;
	mSubPassList << subPass;
}