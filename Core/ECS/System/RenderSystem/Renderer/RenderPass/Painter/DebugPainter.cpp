#include "DebugPainter.h"
#include "ECS/System/RenderSystem/QRenderSystem.h"
#include "ECS/Component/QCameraComponent.h"
#include "QEngineCoreApplication.h"
#include "qevent.h"

const ImVec2 TextureSize(100, 100);

void FrameGraphDelegate::UpdateGraph() {
	mNodes.clear();
	mLinks.clear();
	mTemplates.clear();
	std::shared_ptr<QFrameGraph> frameGraph = TheRenderSystem->renderer()->getFrameGraph();
	const QHash<QString, std::shared_ptr<QFrameGraphNode>>& nodeMap = frameGraph->getGraphNodeMap();

	QHash<QString, int> indexMap;

	for (auto Iter = nodeMap.begin(); Iter != nodeMap.end(); ++Iter) {
		Node node;
		node.mName = Iter.key().toLocal8Bit();
		node.x = mNodes.size() * 100;
		node.y = mNodes.size() * 0;
		node.mSelected = false;
		node.templateIndex = mTemplates.size();
		node.mRenderPass = Iter.value()->mRenderPass.get();
		for (auto& inputTex : Iter.value()->mRenderPass->getInputTextures().values()) {
			node.mInputSlot << inputTex->name();
			node.mInputNameCStr << node.mInputSlot.back().data();
		}
		for (auto& outputTex : Iter.value()->mRenderPass->getOutputTextures().values()) {
			node.mOutputSlot << outputTex->name();
			node.mOnputNameCStr << node.mOutputSlot.back().data();
		}

		indexMap[Iter.key()] = mNodes.size();

		GraphEditor::Template temp;
		temp.mHeaderColor = IM_COL32(160, 160, 180, 255);
		temp.mBackgroundColor = IM_COL32(100, 100, 140, 255);
		temp.mBackgroundColorOver = IM_COL32(110, 110, 150, 255);
		temp.mInputNames = node.mInputNameCStr.data();
		temp.mInputCount = node.mInputNameCStr.size();
		temp.mInputColors = nullptr;

		temp.mOutputNames = node.mOnputNameCStr.data();
		temp.mOutputCount = node.mOnputNameCStr.size();
		temp.mOutputColors = nullptr;
		mNodes.emplace_back(std::move(node));
		mTemplates.emplace_back(std::move(temp));
	}

	for (auto Iter = nodeMap.begin(); Iter != nodeMap.end(); ++Iter) {
		for (auto sub : Iter.value()->mSubPassList) {
			auto& outputList = Iter.value()->mRenderPass->getOutputTextures();
			auto& inputList = sub->mRenderPass->getInputTextures();
			for (int i = 0; i < outputList.size(); i++) {
				for (int j = 0; j < inputList.size(); j++) {
					if (outputList[i] == inputList[j]&& inputList[j]!=nullptr) {
						GraphEditor::Link link;
						link.mInputNodeIndex = indexMap[Iter.key()];
						link.mInputSlotIndex = i;
						link.mOutputNodeIndex = indexMap[sub->mName];
						link.mOutputSlotIndex = j;
						mLinks.emplace_back(std::move(link));
					}
				}
			}
		}
	}

	QHash<QString, bool> visited;
	int count = mNodes.size();
	int level = 0;;
	while (count) {
		QStringList levelList;
		for (auto Iter = nodeMap.begin(); Iter != nodeMap.end(); ++Iter) {
			if(visited[Iter.key()])
				continue;
			bool flag = true;
			for (auto dep : Iter.value()->mDependencyList) {
				if (!visited[dep->mName]) {
					flag = false;
				}
			}
			if (flag) {
				for (auto& node : mNodes) {
					if (node.mName == Iter.key()) {
						node.x = level * 150;
						node.y = level * 50 + levelList.size() * 150;
						break;
					}
				}
				levelList << Iter.key();
				count--;
			}
		}
		for (auto& level : levelList) {
			visited[level] = true;
		}
		level++;
	}

}

bool FrameGraphDelegate::AllowedLink(GraphEditor::NodeIndex from, GraphEditor::NodeIndex to) {
	return false;
}

void FrameGraphDelegate::SelectNode(GraphEditor::NodeIndex nodeIndex, bool selected) {
	mNodes[nodeIndex].mSelected = selected;
}

void FrameGraphDelegate::MoveSelectedNodes(const ImVec2 delta) {

}

void FrameGraphDelegate::RightClick(GraphEditor::NodeIndex nodeIndex, GraphEditor::SlotIndex slotIndexInput, GraphEditor::SlotIndex slotIndexOutput) {

}

void FrameGraphDelegate::AddLink(GraphEditor::NodeIndex inputNodeIndex, GraphEditor::SlotIndex inputSlotIndex, GraphEditor::NodeIndex outputNodeIndex, GraphEditor::SlotIndex outputSlotIndex) {
}

void FrameGraphDelegate::DelLink(GraphEditor::LinkIndex linkIndex) {
}

void FrameGraphDelegate::CustomDraw(ImDrawList* drawList, ImRect rectangle, GraphEditor::NodeIndex nodeIndex) {
	float scaleFactor = rectangle.GetWidth() / TextureSize.x;
	for (int i = 0; i < mNodes[nodeIndex].mOutputSlot.size(); i++) {
		ImVec2 pos(rectangle.Min.x, rectangle.Min.y + TextureSize.y* scaleFactor * i);
		if (mNodes[nodeIndex].mRenderPass) {
			QRhiTexture* tex = mNodes[nodeIndex].mRenderPass->getOutputTextures()[i];
			if (tex) {
				drawList->AddImage((ImTextureID)tex, pos, ImVec2(pos.x + TextureSize.x * scaleFactor, pos.y + TextureSize.y * scaleFactor));
			}
		}
	}
}

const size_t FrameGraphDelegate::GetTemplateCount() {
	return mTemplates.size();
}

const GraphEditor::Template FrameGraphDelegate::GetTemplate(GraphEditor::TemplateIndex index) {
	return mTemplates[index];
}

const size_t FrameGraphDelegate::GetNodeCount() {
	return mNodes.size();
}

const GraphEditor::Node FrameGraphDelegate::GetNode(GraphEditor::NodeIndex index) {
	const auto& myNode = mNodes[index];
	return GraphEditor::Node
	{
		myNode.mName.data(),
		myNode.templateIndex,
		ImRect(ImVec2(myNode.x, myNode.y), ImVec2(myNode.x + TextureSize.x, myNode.y + myNode.mOutputSlot.size() * TextureSize.y + 20)),
		myNode.mSelected
	};
}

const size_t FrameGraphDelegate::GetLinkCount() {
	return mLinks.size();
}

const GraphEditor::Link FrameGraphDelegate::GetLink(GraphEditor::LinkIndex index) {
	return mLinks[index];
}

DebugPainter::DebugPainter() {
	mFrameGraphOption.mRenderGrid = false;
	mFrameGraphOption.mDisplayLinksAsCurves = false;
	mFrameGraphOption.mBackgroundColor = ImColor(0, 0, 0, 0);
	mFrameGraphOption.mGridColor = ImColor(0, 0, 0, 0);
	mFrameGraphOption.mGridColor2 = ImColor(0, 0, 0, 0);
	mFrameGraphOption.mQuadSelection = ImColor(0, 0, 0, 0);
	mFrameGraphOption.mQuadSelectionBorder = ImColor(0, 0, 0, 0);
	mFrameGraphOption.mFrameFocus = ImColor(0, 0, 0, 0);
}

void DebugPainter::paintImgui() {
	auto& io = ImGui::GetIO();

	static GraphEditor::FitOnScreen fit = GraphEditor::Fit_None;

	ImGui::GetBackgroundDrawList()->AddText(ImGui::GetFont(), ImGui::GetFontSize(), ImVec2(0, mDebugTexture->pixelSize().height() - ImGui::GetFontSize()), ImColor(0, 255, 0, 255), QString("FPS: %1").arg(TheRenderSystem->window()->getFPS()).toLocal8Bit().data());

	ImGui::SetNextWindowPos(ImVec2(0, 0));
	ImGui::SetNextWindowSize(ImVec2(mDebugTexture->pixelSize().width(), mDebugTexture->pixelSize().height()));

	ImGuiWindowFlags windowFlags = 0;
	windowFlags |= ImGuiWindowFlags_NoMove;
	windowFlags |= ImGuiWindowFlags_NoResize;
	windowFlags |= ImGuiWindowFlags_NoBackground;

	ImGui::Begin("Frame Graph", NULL, windowFlags);
	if (ImGui::Button("Fit all nodes")) {
		fit = GraphEditor::Fit_AllNodes;
	}
	ImGui::SameLine();
	if (ImGui::Button("Fit selected nodes")) {
		fit = GraphEditor::Fit_SelectedNodes;
	}
	GraphEditor::Show(mFrameGraphDelegate, mFrameGraphOption, mFrameGraphViewState, true, &fit);

	ImGui::End();

	ImGuizmo::BeginFrame();
	ImGuizmo::SetRect(0, 0, io.DisplaySize.x, io.DisplaySize.y);

	QCameraComponent* camera = TheEngine->world()->getCurrentCamera();
	if (camera) {
		QEntity* currentEntiy = TheEngine->world()->getCurrentEntity();
		if (currentEntiy) {
			QMatrix4x4 MAT;

			QMatrix4x4 Model = currentEntiy->calculateMatrixModel();

			QMatrix4x4 View = camera->getMatrixView();

			QMatrix4x4 Clip =  camera->getMatrixClip();

			ImGuizmo::Manipulate(View.constData(), Clip.constData(), mOpt, ImGuizmo::LOCAL, Model.data(), NULL, NULL, NULL, NULL);

			currentEntiy->setMatrixModel(Model);

			/*if (NewLocal != Local) {
				Engine->requestUpdatePropertyPanel();
			}*/
		}
	}
}

void DebugPainter::resourceUpdate(QRhiResourceUpdateBatch* batch) {
	if (!mReadPoint.isNull()&&mDebugTexture) {
		mReadDesc.setTexture(mDebugTexture);
		mReadReult.completed = [this]() {
			if (!mReadPoint.isNull()) {
				const uchar* p = reinterpret_cast<const uchar*>(mReadReult.data.constData());
				int offset = (mReadReult.pixelSize.width() * mReadPoint.y() + mReadPoint.x()) * 4;
				uint32_t id = p[offset] + p[offset + 1] * 256 + p[offset + 2] * 256 * 256 + p[offset + 3] * 256 * 256 * 256;
				auto entity = TheEngine->world()->getEntityById(id);
				TheEngine->world()->setCurrentEntity(entity);
				mReadPoint = { 0,0 };
			}
		};
		batch->readBackTexture(mReadDesc,&mReadReult);
		RHI->finish();
	}
	QEntity* currentEntiy = TheEngine->world()->getCurrentEntity();
	QVector4D id(-1,-1,-1,-1);
	if (currentEntiy) {
		QEntity::ID mId = currentEntiy->GetId();
		int r = (mId & 0x000000FF) >> 0;
		int g = (mId & 0x0000FF00) >> 8;
		int b = (mId & 0x00FF0000) >> 16;
		int a = (mId & 0xFF000000) >> 24;
		id = QVector4D(r, g, b, a) / 255.0f;;
	}		
	batch->updateDynamicBuffer(mUniformBuffer.get(), 0, sizeof(QVector4D), &id);
	ImGuiPainter::resourceUpdate(batch);
}

void DebugPainter::compile() {
	ImGuiPainter::compile();

	mUniformBuffer.reset(RHI->newBuffer(QRhiBuffer::Type::Dynamic, QRhiBuffer::UniformBuffer, sizeof(QVector4D)));
	mUniformBuffer->create();

	mOutlineSampler.reset(RHI->newSampler(QRhiSampler::Nearest,
				   QRhiSampler::Nearest,
				   QRhiSampler::None,
				   QRhiSampler::ClampToEdge,
				   QRhiSampler::ClampToEdge));
	mOutlineSampler->create();
	mOutlinePipeline.reset(RHI->newGraphicsPipeline());
	QRhiGraphicsPipeline::TargetBlend blendState;
	blendState.enable = true;
	mOutlinePipeline->setTargetBlends({ blendState });
	mOutlinePipeline->setSampleCount(mSampleCount);

	QString vsCode = R"(#version 450
layout (location = 0 ) out vec2 vUV;
out gl_PerVertex{
	vec4 gl_Position;
};
void main() {
	vUV = vec2((gl_VertexIndex << 1) & 2, gl_VertexIndex & 2);
	gl_Position = vec4(vUV * 2.0f - 1.0f, 0.0f, 1.0f);
	%1
}
)";
	QShader vs = QRenderSystem::createShaderFromCode(QShader::VertexStage, vsCode.arg(RHI->isYUpInNDC() ? "	vUV.y = 1 - vUV.y;" : "").toLocal8Bit());

	QShader fs = QRenderSystem::createShaderFromCode(QShader::FragmentStage, R"(#version 450
layout (location = 0) in vec2 vUV;
layout (location = 0) out vec4 outFragColor;

layout (binding = 0) uniform sampler2D uDebugId;
layout (binding = 1) uniform DebugID{
	vec4 ID;
}Current;

void main() {
	vec2 texOffset = 1.0 / textureSize(uDebugId, 0);		// gets size of single texel
	
	int count = 0;

	count += (texture(uDebugId,vUV) == Current.ID ? 1 : 0 );		//±ßÔµ¼ì²â
	count += (texture(uDebugId,vUV+vec2(texOffset.x,0)) == Current.ID ? 1 : 0 );
	count += (texture(uDebugId,vUV-vec2(texOffset.x,0)) == Current.ID ? 1 : 0 );
	count += (texture(uDebugId,vUV+vec2(0,texOffset.y)) == Current.ID ? 1 : 0 );
	count += (texture(uDebugId,vUV-vec2(0,texOffset.y)) == Current.ID ? 1 : 0 );

	if(count>0&&count<5){
		outFragColor = vec4(1.0,0.8,0.4,1.0);
	}
	else{
		outFragColor = vec4(0);
	}
}
)");
	mOutlinePipeline->setShaderStages({
		{ QRhiShaderStage::Vertex, vs },
		{ QRhiShaderStage::Fragment, fs }
							   });
	QRhiVertexInputLayout inputLayout;

	mOutlineBindings.reset(RHI->newShaderResourceBindings());

	mOutlineBindings->setBindings({
		QRhiShaderResourceBinding::sampledTexture(0,QRhiShaderResourceBinding::FragmentStage,mDebugTexture,mOutlineSampler.get()),
		QRhiShaderResourceBinding::uniformBuffer(1,QRhiShaderResourceBinding::FragmentStage,mUniformBuffer.get())
	 });

	mOutlineBindings->create();
	mOutlinePipeline->setVertexInputLayout(inputLayout);
	mOutlinePipeline->setShaderResourceBindings(mOutlineBindings.get());
	mOutlinePipeline->setRenderPassDescriptor(mRenderPassDesc);
	mOutlinePipeline->create();

	mFrameGraphDelegate.UpdateGraph();
}

void DebugPainter::paint(QRhiCommandBuffer* cmdBuffer, QRhiRenderTarget* renderTarget) {
	cmdBuffer->setGraphicsPipeline(mOutlinePipeline.get());
	cmdBuffer->setViewport(QRhiViewport(0, 0, renderTarget->pixelSize().width(), renderTarget->pixelSize().height()));
	cmdBuffer->setShaderResources(mOutlineBindings.get());
	cmdBuffer->draw(4);

	ImGuiPainter::paint(cmdBuffer, renderTarget);
}

bool DebugPainter::eventFilter(QObject* watched, QEvent* event) {
	static QPoint pressedPos;
	if (watched != nullptr) {
		switch (event->type()) {
		case QEvent::MouseButtonPress:
			pressedPos = QCursor::pos();
			break;
		case QEvent::MouseButtonRelease: {
			QMouseEvent* mouse = static_cast<QMouseEvent*>(event);
			if (pressedPos == QCursor::pos() && mouse->button() == Qt::LeftButton) {
				QPoint pt = mouse->pos() * qApp->devicePixelRatio();
				if (RHI->isYUpInNDC()) {
					pt.setY(mWindow->height() - pt.y());
				}
				mReadPoint = pt;
			}
			pressedPos = { 0,0 };
			break;
		}
		case QEvent::MouseMove: {
			if (qApp->mouseButtons() & Qt::LeftButton) {
			}
			break;
		}
		case QEvent::KeyPress: {
			QKeyEvent* keyEvent = static_cast<QKeyEvent*>(event);
			if (keyEvent->key() == Qt::Key_W) {
				mOpt = ImGuizmo::OPERATION::TRANSLATE;
			}
			else if (keyEvent->key() == Qt::Key_E) {
				mOpt = ImGuizmo::OPERATION::ROTATE;
			}
			else if (keyEvent->key() == Qt::Key_R) {
				mOpt = ImGuizmo::OPERATION::SCALE;
			}
			break;
		}
		case QEvent::KeyRelease: {
			break;
		}
		}
	}
	return ImGuiPainter::eventFilter(watched, event);
}

