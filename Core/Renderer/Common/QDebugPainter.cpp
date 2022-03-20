#include "QDebugPainter.h"
#include "QApplication"
#include "qevent.h"
#include "QEngine.h"
#include "ImGuizmo.h"
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
		QMatrix4x4 MAT;
		QMatrix4x4 Model;
		QMatrix4x4 View = camera->getMatrixView();
		QMatrix4x4 Clip = RHI->clipSpaceCorrMatrix() * camera->getMatrixClip();
		if (mCurrentComp) {
			Model = mCurrentComp->calculateModelMatrix();
			ImGuizmo::Manipulate(View.constData(), Clip.constData(), ImGuizmo::OPERATION::TRANSLATE, ImGuizmo::LOCAL, Model.data(), NULL, NULL, NULL,  NULL);
			QVector3D position;
			QVector3D rotation;
			QVector3D scaling;
			ImGuizmo::DecomposeMatrixToComponents(Model.constData(), (float*)&position, (float*)&rotation, (float*)&scaling);
			mCurrentComp->setPosition(position);
			mCurrentComp->setRotation(rotation);
			mCurrentComp->setScale(scaling);
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
			break;
		}
		case QEvent::KeyRelease: {
			break;
		}
		}
	}
	return QImguiPainter::eventFilter(watched, event);
}


