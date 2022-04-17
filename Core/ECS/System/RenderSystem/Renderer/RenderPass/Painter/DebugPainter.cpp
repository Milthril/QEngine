#include "DebugPainter.h"
#include "ECS/System/RenderSystem/QRenderSystem.h"
#include "ECS/Component/QCameraComponent.h"
#include "QEngineCoreApplication.h"
#include "qevent.h"

DebugPainter::DebugPainter() {
}

void DebugPainter::paintImgui() {
	auto& io = ImGui::GetIO();
	ImGui::GetBackgroundDrawList()->AddText(ImGui::GetFont(), ImGui::GetFontSize(), ImVec2(1, 1), ImColor(0, 255, 0, 255), QString("FPS: %1").arg(QRenderSystem::instance()->window()->getFPS()).toLocal8Bit().data());
	ImGuizmo::BeginFrame();
	ImGuizmo::SetRect(0, 0, io.DisplaySize.x, io.DisplaySize.y);
	QCameraComponent* camera = Engine->world()->getCurrentCamera();
	if (camera) {
		QEntity* currentEntiy = Engine->world()->getCurrentEntity();
		if (currentEntiy) {
			QMatrix4x4 MAT;

			QMatrix4x4 Model = currentEntiy->calculateMatrixModel();

			QMatrix4x4 View = camera->getMatrixView();

			QMatrix4x4 Clip =  camera->getMatrixClip();

			ImGuizmo::Manipulate(View.constData(), Clip.constData(), mOpt, ImGuizmo::LOCAL, Model.data(), NULL, NULL, NULL, NULL);

			currentEntiy->setMatrixModel(Model);

	/*		if (NewLocal != Local) {
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
				auto entity = Engine->world()->getEntityById(id);
				Engine->world()->setCurrentEntity(entity);
				mReadPoint = { 0,0 };
			}
		};
		batch->readBackTexture(mReadDesc,&mReadReult);
		RHI->finish();
	}
	ImGuiPainter::resourceUpdate(batch);
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
