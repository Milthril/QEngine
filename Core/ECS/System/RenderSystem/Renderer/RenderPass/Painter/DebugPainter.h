#ifndef DebugPainter_h__
#define DebugPainter_h__

#include "ImGuiPainter.h"
#include "ImGuizmo.h"
#include <GraphEditor.h>

template <typename T, std::size_t N>
struct Array {
	T data[N];
	const size_t size() const { return N; }

	const T operator [] (size_t index) const { return data[index]; }
	operator T* () {
		T* p = new T[N];
		memcpy(p, data, sizeof(data));
		return p;
	}
};

template <typename T, typename ... U> Array(T, U...)->Array<T, 1 + sizeof...(U)>;


struct FrameGraphDelegate : public GraphEditor::Delegate {

	void UpdateGraph();
	bool AllowedLink(GraphEditor::NodeIndex from, GraphEditor::NodeIndex to) override;
	void SelectNode(GraphEditor::NodeIndex nodeIndex, bool selected) override;
	void MoveSelectedNodes(const ImVec2 delta) override;
	virtual void RightClick(GraphEditor::NodeIndex nodeIndex, GraphEditor::SlotIndex slotIndexInput, GraphEditor::SlotIndex slotIndexOutput) override;
	void AddLink(GraphEditor::NodeIndex inputNodeIndex, GraphEditor::SlotIndex inputSlotIndex, GraphEditor::NodeIndex outputNodeIndex, GraphEditor::SlotIndex outputSlotIndex) override;
	void DelLink(GraphEditor::LinkIndex linkIndex) override;
	void CustomDraw(ImDrawList* drawList, ImRect rectangle, GraphEditor::NodeIndex nodeIndex) override;
	const size_t GetTemplateCount() override;
	const GraphEditor::Template GetTemplate(GraphEditor::TemplateIndex index) override;
	const size_t GetNodeCount() override;
	const GraphEditor::Node GetNode(GraphEditor::NodeIndex index) override;
	const size_t GetLinkCount() override;
	const GraphEditor::Link GetLink(GraphEditor::LinkIndex index) override;
	// Graph datas
	static const inline GraphEditor::Template mTemplates[] = {
		{
			IM_COL32(160, 160, 180, 255),
			IM_COL32(100, 100, 140, 255),
			IM_COL32(110, 110, 150, 255),
			1,
			Array{"Input"},
			nullptr,
			1,
			Array{"Output"},
			nullptr
		},
	};

	struct Node {
		QByteArray mName;
		GraphEditor::TemplateIndex templateIndex;
		float x, y;
		bool mSelected;
	};

	std::vector<Node> mNodes;
	std::vector<GraphEditor::Link> mLinks;;
};


class DebugPainter :public ImGuiPainter {
	Q_OBJECT
public:
	DebugPainter();
	void setupDebugTexture(QRhiTexture* texture) {
		mDebugTexture = texture;
	}
	void paintImgui() override;

	virtual void resourceUpdate(QRhiResourceUpdateBatch* batch) override;
	virtual void compile() override;
	virtual void paint(QRhiCommandBuffer* cmdBuffer, QRhiRenderTarget* renderTarget) override;

protected:
	bool eventFilter(QObject* watched, QEvent* event) override;
private:
	QRhiTexture* mDebugTexture = nullptr;
	ImGuizmo::OPERATION mOpt = ImGuizmo::OPERATION::TRANSLATE;
	QRhiReadbackResult mReadReult;
	QRhiReadbackDescription mReadDesc;
	QPoint mReadPoint;

	QRhiSPtr<QRhiGraphicsPipeline> mOutlinePipeline;
	QRhiSPtr<QRhiSampler> mOutlineSampler;
	QRhiSPtr<QRhiShaderResourceBindings> mOutlineBindings;
	QRhiSPtr<QRhiBuffer> mUniformBuffer;

	FrameGraphDelegate mFrameGraphDelegate;
	GraphEditor::Options mFrameGraphOption;
	GraphEditor::ViewState mFrameGraphViewState;
};

#endif // DebugPainter_h__