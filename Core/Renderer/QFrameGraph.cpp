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

void QFrameGraphNode::tryExec() {
	bool canExec = true;
	for (auto& dep : mDependencyList) {
		if (!dep->isFinished) {
			canExec = false;
		}
	}
	if (canExec)
		executableNewThread();
}

void QFrameGraphNode::executableNewThread() {
	mThread = std::make_shared<std::thread>([this]() {
		mRenderPass->execute();
		isFinished = true;
		for (int i = 0; i < mSubPassList.size(); i++) {		//
			mSubPassList[i]->tryExec();
		}
	});
	mThread->detach();
}

void QFrameGraph::compile() {
	for (auto& node : mGraphNodeMap) {
		node->tryCompile();
	}
}

void QFrameGraph::executable() {
	for (auto& node : mGraphNodeMap) {
		node->isFinished = false;
	}
	for (auto& node : mGraphNodeMap) {
		node->tryExec();
	}

	//std::unique_lock<std::mutex> lk(mMutex);
	//mConditionVar.wait(lk, [this]() {
	//	bool isFinishedAll = true;
	//	for (auto& node : mGraphNodeMap) {
	//		if (!node->isFinished)
	//			isFinishedAll = false;
	//	}
	//	return isFinishedAll;
	//});

	bool isFinishedAll = false;
	while (!isFinishedAll) {
		isFinishedAll = true;
		for (auto& node : mGraphNodeMap) {
			if (!node->isFinished)
				isFinishedAll = false;
		}
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
	return this;
}

QFrameGraphBuilder* QFrameGraphBuilder::dependency(QStringList dependencyList) {
	Q_ASSERT(mFrameGraph->mGraphNodeMap.contains(mCurrentNodeName));
	std::shared_ptr<QFrameGraphNode>& node = mFrameGraph->mGraphNodeMap[mCurrentNodeName];
	for (QString& dep : dependencyList) {
		Q_ASSERT(mFrameGraph->mGraphNodeMap.contains(mCurrentNodeName));
		std::shared_ptr<QFrameGraphNode>& depNode = mFrameGraph->mGraphNodeMap[dep];
		node->mDependencyList << depNode.get();
		depNode->mSubPassList << node;
	}
	return this;
}

std::shared_ptr<QFrameGraph> QFrameGraphBuilder::end() {
	return mFrameGraph;
}