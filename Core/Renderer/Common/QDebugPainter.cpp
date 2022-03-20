#include "QDebugPainter.h"
#include "QApplication"
#include "qevent.h"
#include "QEngine.h"
#include "Scene/Component/Camera/QCameraComponent.h"

QDebugPainter::QDebugPainter()
{
	connect(Engine->renderer().get(), &QSceneRenderer::readBackCompId, this, [this](QSceneComponent::ComponentId id) {
		auto comp = Engine->scene()->searchCompById(id);
		if (comp != mCurrentComp) {
			mCurrentComp = comp;
			Q_EMIT currentCompChanged(mCurrentComp);
		}
	});
}

void QDebugPainter::paint()
{
	auto& io =ImGui::GetIO();
	ImGuizmo::BeginFrame();
	ImGuizmo::SetRect(0, 0, io.DisplaySize.x, io.DisplaySize.y);
	auto camera = Engine->scene()->getCamera();
	if (camera) {
		if (mCurrentComp) {
			QMatrix4x4 MAT;
			QMatrix4x4 Local = mCurrentComp->calculateLocalMatrix();
			QMatrix4x4 View = camera->getMatrixView();
			QMatrix4x4 Clip = RHI->clipSpaceCorrMatrix() * camera->getMatrixClip();
			QMatrix4x4 Parent = mCurrentComp->calculateParentMatrix();

			QMatrix4x4 World = Parent * Local;
			ImGuizmo::Manipulate(View.constData(), Clip.constData(), mOpt, ImGuizmo::LOCAL, World.data(), NULL, NULL, NULL,  NULL);
			QVector3D position;
			QVector3D rotation;
			QVector3D scaling;
			QMatrix4x4 ParentMat = mCurrentComp->calculateParentMatrix();
			QMatrix4x4 NewLocal = ParentMat.inverted()* World;

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

bool QDebugPainter::eventFilter(QObject* watched, QEvent* event)
{
	static QPoint pressedPos;
	if (watched != nullptr ) {
		switch (event->type())
		{
		case QEvent::MouseButtonPress:
			pressedPos = QCursor::pos();
			break;
		case QEvent::MouseButtonRelease: {
			QMouseEvent* mouse = static_cast<QMouseEvent*>(event);
			if (pressedPos == QCursor::pos() && mouse->button() == Qt::LeftButton) {
				Engine->renderer()->requestReadbackCompId(mouse->pos() * Engine->devicePixelRatio());
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
	return QImguiPainter::eventFilter(watched, event);
}


