#include "DebugPainter.h"
#include "ECS/System/RenderSystem/QRenderSystem.h"
#include "ECS/Component/QCameraComponent.h"
#include "QEngineCoreApplication.h"
#include "qevent.h"
#include <GraphEditor.h>

DebugPainter::DebugPainter() {
}

void DebugPainter::paintImgui() {
	auto& io = ImGui::GetIO();
	ImGui::GetBackgroundDrawList()->AddText(ImGui::GetFont(), ImGui::GetFontSize(), ImVec2(1, 1), ImColor(0, 255, 0, 255), QString("FPS: %1").arg(TheRenderSystem->window()->getFPS()).toLocal8Bit().data());
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
layout (location = 0) out vec2 vUV;
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
