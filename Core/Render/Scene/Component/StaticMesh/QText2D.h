#ifndef QText2D_h__
#define QText2D_h__

#include "Render/Scene/Component/StaticMesh/QStaticMeshComponent.h"
#include "QFont"

class QText2D :public QStaticMeshComponent {
	Q_OBJECT
		Q_PROPERTY(QString Text READ getText WRITE setText)
		Q_PROPERTY(QFont Font READ getFont WRITE setFont)
		Q_PROPERTY(Qt::Orientation Orientation READ getOrientation WRITE setOrientation)
		Q_PROPERTY(uint32_t Spacing READ getSpacing WRITE setSpacing)
public:
	QText2D(QString text, QFont font = QFont(), Qt::Orientation o = Qt::Horizontal, uint32_t spacing = 0);

	QString getText() const;
	void setText(QString val);

	QFont getFont() const;
	void setFont(QFont val);

	Qt::Orientation getOrientation() const;
	void setOrientation(Qt::Orientation val);

	uint32_t getSpacing() const;
	void setSpacing(uint32_t val);
private:
	void recreateVertexData();
	QString mText;
	QFont mFont;
	Qt::Orientation mOrientation;
	uint32_t mSpacing;
};

#endif // QText2D_h__