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