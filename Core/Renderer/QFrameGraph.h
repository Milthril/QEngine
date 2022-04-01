#ifndef QFrameGraph_h__
#define QFrameGraph_h__
#include "IRenderPassBase.h"

class QFrameGraphNode {
	friend class QFrameGraph;
private:
	void tryCompile();
	void tryExec();
	void executableNewThread();
public:
	QString mName;
	std::shared_ptr<IRenderPassBase> mRenderPass;
	QList<QFrameGraphNode*> mDependencyList;
	QList<std::shared_ptr<QFrameGraphNode>> mSubPassList;
	std::shared_ptr<std::thread> mThread;
	std::atomic_bool isFinished = false;
	std::atomic_bool isCompiled = false;
};

class QFrameGraph {
	friend class QFrameGraphBuilder;
public:
	void compile();
	void executable();
private:
	QHash<QString, std::shared_ptr<QFrameGraphNode>> mGraphNodeMap;
	std::mutex mMutex;
	std::condition_variable mConditionVar;
};

class QFrameGraphBuilder {
public:
	QFrameGraphBuilder* begin();
	QFrameGraphBuilder* node(QString name, std::shared_ptr<IRenderPassBase> renderPass, std::function<void()> funcSetup);
	QFrameGraphBuilder* dependency(QStringList dependencyList);
	std::shared_ptr<QFrameGraph> end();
private:
	std::shared_ptr<QFrameGraph> mFrameGraph;
	QString mCurrentNodeName;
	QList<std::shared_ptr<IRenderPassBase>> mRenderPassNodeList;
};

#endif // QFrameGraphBuilder_h__
