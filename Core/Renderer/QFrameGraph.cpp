#include "QFrameGraph.h"
#include "QQueue"

void QFrameGraphNode::tryCompile() {
	bool canSetup = true;
	for (auto& dep : mDependencyList) {
		if (!dep->isCompiled) {
			canSetup = false;
		}
	}
	if (canSetup) {
		mRenderPass->setup();
		mRenderPass->compile();
		isCompiled = true;
	}
	for (int i = 0; i < mSubPassList.size(); i++) {
		mSubPassList[i]->tryCompile();
	}
}

void QFrameGraph::compile() {
	for (auto& node : mGraphNodeMap) {
		node->tryCompile();
	}
}

void QFrameGraph::executable(QRhiCommandBuffer* cmdBuffer) {
	for (auto& renderPass : mRenderQueue) {
		renderPass->mRenderPass->execute(cmdBuffer);
	}
}

QFrameGraphBuilder* QFrameGraphBuilder::begin() {
	mFrameGraph = std::make_shared<QFrameGraph>();
	return this;
}

QFrameGraphBuilder* QFrameGraphBuilder::node(QString name, std::shared_ptr<IRenderPassBase> renderPass, std::function<void()> funcSetup) {
	std::shared_ptr<QFrameGraphNode>& node = mFrameGraph->mGraphNodeMap[name];
	node.reset(new QFrameGraphNode);
	node->mName = name;
	node->mRenderPass = renderPass;
	node->mRenderPass->setFuncSetup(funcSetup);
	mCurrentNodeName = name;
	mFrameGraph->mRenderQueue << node;
	return this;
}

QFrameGraphBuilder* QFrameGraphBuilder::dependency(QStringList dependencyList) {
	Q_ASSERT(mFrameGraph->mGraphNodeMap.contains(mCurrentNodeName));
	std::shared_ptr<QFrameGraphNode>& node = mFrameGraph->mGraphNodeMap[mCurrentNodeName];
	for (QString& dep : dependencyList) {
		Q_ASSERT(mFrameGraph->mGraphNodeMap.contains(mCurrentNodeName));
		std::shared_ptr<QFrameGraphNode>& depNode = mFrameGraph->mGraphNodeMap[dep];
		node->mDependencyList << depNode.get();
		depNode->mSubPassList << node.get();
	}
	return this;
}

std::shared_ptr<QFrameGraph> QFrameGraphBuilder::end() {
	return mFrameGraph;
}