#ifndef QText2D_h__
#define QText2D_h__
#include "Render/Scene/Component//QShapeComponent.h"
#include "QFont"

class QText2D :public QShapeComponent {
	Q_OBJECT
		Q_PROPERTY(QString text READ getText WRITE setText)
		Q_PROPERTY(QFont font READ getFont WRITE setFont)
		Q_PROPERTY(QColor color READ getColor WRITE setColor)
		Q_PROPERTY(Qt::Orientation orientation READ getOrientation WRITE setOrientation)
		Q_PROPERTY(uint32_t spacing READ getSpacing WRITE setSpacing)
public:
	QText2D(QString text, QFont font = QFont(), QColor color = Qt::white, Qt::Orientation o = Qt::Horizontal, uint32_t spacing = 0);

	QString getText() const;
	void setText(QString val);

	QColor getColor() const;
	void setColor(QColor val);

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
	QColor mColor;
	Qt::Orientation mOrientation;
	uint32_t mSpacing;
};

#endif // QText2D_h__