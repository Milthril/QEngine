#include "QDefaultRenderer.h"
#include "Scene\Component\StaticMesh\QStaticMeshComponent.h"
#include "QDefaultProxyStaticMesh.h"
#include "QDefaultProxyParticle.h"
#include "QDefaultProxySkyBox.h"
#include "QEngine.h"
#include "QDefaultProxySkeletonModel.h"
#include "Renderer/CommonPass/DebugDrawPass.h"

QDefaultRenderer::QDefaultRenderer()
	:mScenePass(this)
{
}

void QDefaultRenderer::setup() {
	mScenePass.setupSampleCount(4);
	mScenePass.setEnableOutputDebugId(true);
	mScenePass.setupSceneFrameSize(QSize(800, 600));
	mScenePass.compile();

	mSwapChainDrawPass.setupSwapChain(Engine->window()->getSwapChain());
	mSwapChainDrawPass.setupTexture(mScenePass.getOutputTexture());
	mSwapChainDrawPass.compile();
}

void QDefaultRenderer::render()
{
	mScenePass.execute();
	mSwapChainDrawPass.execute();
}

void QDefaultRenderer::requestReadbackCompId(const QPoint& screenPt)
{
	mReadPoint = screenPt;
}