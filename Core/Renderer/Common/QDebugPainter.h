#ifndef QDebugPainter_h__
#define QDebugPainter_h__

#include "QObject"
#include "RHI\QRhiWindow.h"

class QDebugPainter :public QObject{
	Q_OBJECT
public:
	enum Mode {
		Translate,
		Rotate,
		Scale
	};
	struct Vertex{
		QVector3D position;
		QVector3D color;
	};
	QDebugPainter();
	void drawCommand(QRhiCommandBuffer* cmdBuffer,  QRhiRenderTarget* outputTarget);
protected:
	void initRhiResource();
	bool eventFilter(QObject* watched, QEvent* event) override;
Q_SIGNALS:
	void currentCompChanged(std::shared_ptr<QSceneComponent>);
private:
	std::shared_ptr<QSceneComponent> mCurrentComp;
	QRhiSPtr<QRhiGraphicsPipeline> mPipeline;
	QRhiSPtr<QRhiBuffer> mVertexBuffer;
	QRhiSPtr<QRhiBuffer> mUniformBuffer;
};


#endif // QDebugPainter_h__