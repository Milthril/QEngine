#include "QDefaultRenderer.h"
#include "Scene\Component\StaticMesh\QStaticMeshComponent.h"
#include "QDefaultProxyStaticMesh.h"
#include "QDefaultProxyParticle.h"
#include "QDefaultProxySkyBox.h"
#include "QEngine.h"
#include "QDefaultProxySkeletonModel.h"
#include "Renderer/CommonPass/DebugDrawPass.h"

QDefaultRenderer::QDefaultRenderer()
{
}

void QDefaultRenderer::render()
{
}

void QDefaultRenderer::requestReadbackCompId(const QPoint& screenPt)
{
	mReadPoint = screenPt;
}

std::shared_ptr<ISceneComponentRenderProxy> QDefaultRenderer::createStaticMeshProxy(std::shared_ptr<QStaticMeshComponent> comp)
{
	return std::make_shared<QDefaultProxyStaticMesh>(comp);
}

std::shared_ptr<ISceneComponentRenderProxy> QDefaultRenderer::createSkeletonMeshProxy(std::shared_ptr<QSkeletonModelComponent> comp)
{
	return std::make_shared<QDefaultProxySkeletonModel>(comp);
}

std::shared_ptr<ISceneComponentRenderProxy> QDefaultRenderer::createParticleProxy(std::shared_ptr<QParticleComponent> comp)
{
	return std::make_shared<QDefaultProxyParticle>(comp);
}

std::shared_ptr<ISceneComponentRenderProxy> QDefaultRenderer::createSkyBoxProxy(std::shared_ptr<QSkyBoxComponent> comp)
{
	return std::make_shared<QDefaultProxySkyBox>(comp);
}