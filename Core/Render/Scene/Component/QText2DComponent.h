#ifndef QText2DComponent_h__
#define QText2DComponent_h__

#include "QPrimitiveComponent.h"

class QText2DComponent :public QPrimitiveComponent {
	Q_OBJECT
		Q_PROPERTY(QString text READ getText WRITE setText)
public:
	QSceneComponent::Type type() override {
		return QSceneComponent::Text2D;
	}
	QString getText() const;
	void setText(QString val);

	uint8_t hasNewText : 1;
private:
	QString text;
};

#endif // QText2DComponent_h__