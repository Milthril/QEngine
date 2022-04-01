#include "DebugPainter.h"
#include "QApplication"
#include "qevent.h"
#include "QEngine.h"
#include "Scene/Component/Camera/QCameraComponent.h"

DebugPainter::DebugPainter()
{
	connect(Engine->renderer().get(), &ISceneRenderer::readBackCompId, this, [this](QSceneComponent::ComponentId id) {
		auto comp = Engine->scene()->searchCompById(id);
		if (comp.get() != mCurrentComp) {
			mCurrentComp = comp.get();
			Q_EMIT currentCompChanged(mCurrentComp);
		}
	});
}

void DebugPainter::paint()
{
	auto& io = ImGui::GetIO();
	ImGui::GetBackgroundDrawList()->AddText(ImGui::GetFont(), ImGui::GetFontSize(), ImVec2(1, 1), ImColor(0, 255, 0, 255), QString("FPS: %1").arg(Engine->window()->getFPS()).toLocal8Bit().data());
	ImGuizmo::BeginFrame();
	ImGuizmo::SetRect(0, 0, io.DisplaySize.x, io.DisplaySize.y);
	auto camera = Engine->scene()->getCamera();
	if (camera) {
		if (mCurrentComp) {
			QMatrix4x4 MAT;
			QMatrix4x4 Local = mCurrentComp->calculateLocalMatrix();
			QMatrix4x4 View = camera->getMatrixView();

			QMatrix4x4 mat = QMatrix4x4(1.0f, 0.0f, 0.0f, 0.0f,
										0.0f, -1.0f, 0.0f, 0.0f,
										0.0f, 0.0f, 0.5f, 0.5f,
										0.0f, 0.0f, 0.0f, 1.0f);
			QMatrix4x4 Clip = mat * camera->getMatrixClip();

			QMatrix4x4 Parent = mCurrentComp->calculateParentMatrix();

			QMatrix4x4 World = Parent * Local;
			ImGuizmo::Manipulate(View.constData(), Clip.constData(), mOpt, ImGuizmo::LOCAL, World.data(), NULL, NULL, NULL, NULL);
			QVector3D position;
			QVector3D rotation;
			QVector3D scaling;
			QMatrix4x4 ParentMat = mCurrentComp->calculateParentMatrix();
			QMatrix4x4 NewLocal = ParentMat.inverted() * World;

			ImGuizmo::DecomposeMatrixToComponents(NewLocal.constData(), (float*)&position, (float*)&rotation, (float*)&scaling);
			mCurrentComp->setPosition(position);
			mCurrentComp->setRotation(rotation);
			mCurrentComp->setScale(scaling);
			if (NewLocal != Local) {
				Engine->requestUpdatePropertyPanel();
			}
		}
	}
}

void DebugPainter::setCurrentCompInternal(QSceneComponent* comp)
{
	if (mCurrentComp != comp)
		mCurrentComp = comp;
}

bool DebugPainter::eventFilter(QObject* watched, QEvent* event)
{
	static QPoint pressedPos;
	if (watched != nullptr) {
		switch (event->type())
		{
		case QEvent::MouseButtonPress:
			pressedPos = QCursor::pos();
			break;
		case QEvent::MouseButtonRelease: {
			QMouseEvent* mouse = static_cast<QMouseEvent*>(event);
			if (pressedPos == QCursor::pos() && mouse->button() == Qt::LeftButton) {
				QPoint pt = mouse->pos() * Engine->devicePixelRatio();
				if (RHI->isYUpInNDC()) {
					pt.setY(mWindow->height() - pt.y());
				}
				Engine->renderer()->requestReadbackCompId(pt);
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