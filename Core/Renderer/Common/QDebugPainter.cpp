#include "QDebugPainter.h"
#include "QApplication"
#include "qevent.h"
#include "QEngine.h"

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

void QDebugPainter::drawCommand(QRhiCommandBuffer* cmdBuffer, QRhiRenderTarget* outputTarget)
{

}

void QDebugPainter::initRhiResource()
{
	QVector<Vertex> verteices;
	mVertexBuffer.reset(RHI->newBuffer(QRhiBuffer::Immutable, QRhiBuffer::VertexBuffer, sizeof(Vertex) * verteices.size()));
	Q_ASSERT(mVertexBuffer->create());

	mUniformBuffer.reset(RHI->newBuffer(QRhiBuffer::Type::Dynamic, QRhiBuffer::UniformBuffer, sizeof(QMatrix4x4)));
	Q_ASSERT(mUniformBuffer->create());
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
	return QObject::eventFilter(watched, event);
}


