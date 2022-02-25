#include "QText2D.h"
#include "QFontMetrics"
#include "QPainterPath"
#include "QEarCut.h"

QText2D::QText2D(QString text, QFont font, QColor color, Qt::Orientation o, uint32_t spacing)
	:mText(text)
	, mFont(font)
	, mColor(color)
	, mOrientation(o)
	, mSpacing(spacing)
{
	setTopology(QPrimitiveComponent::Topology::Triangles);
	setBufferType(QRhiBuffer::Type::Dynamic);
	recreateVertexData();
}

QString QText2D::getText() const
{
	return mText;
}

void QText2D::setText(QString val)
{
	if (val != mText) {
		mText = val;
		recreateVertexData();
	}
}

QColor QText2D::getColor() const
{
	return mColor;
}

void QText2D::setColor(QColor val)
{
	if (mColor != val) {
		mColor = val;
		recreateVertexData();
	}
}

QFont QText2D::getFont() const
{
	return mFont;
}

void QText2D::setFont(QFont val)
{
	if (val != mFont) {
		mFont = val;
		recreateVertexData();
	}
}

Qt::Orientation QText2D::getOrientation() const
{
	return mOrientation;
}

void QText2D::setOrientation(Qt::Orientation val)
{
	if (mOrientation != val) {
		mOrientation = val;
		recreateVertexData();
	}
}

uint32_t QText2D::getSpacing() const
{
	return mSpacing;
}

void QText2D::setSpacing(uint32_t val)
{
	if (mSpacing != val) {
		mSpacing = val;
		recreateVertexData();
	}
}

void QText2D::recreateVertexData()
{
	QVector<QText2D::Vertex> vertices;
	QVector<QText2D::Index> indices;
	QFontMetrics fontMetrics(mFont);
	QPainterPath fontPath;
	QSize textSize;
	if (mOrientation == Qt::Orientation::Horizontal) {
		textSize = { 0,fontMetrics.height() };
		for (int i = 0; i < mText.size(); i++) {
			fontPath.addText(textSize.width(), fontMetrics.ascent(), mFont, mText[i]);
			textSize.setWidth(textSize.width() + mSpacing + fontMetrics.horizontalAdvance(mText[i]));
		}
	}
	else {
		textSize = { 0,fontMetrics.ascent() };
		for (int i = 0; i < mText.size(); i++) {
			fontPath.addText(0, textSize.height(), mFont, mText[i]);
			textSize.setWidth(qMax(textSize.width(), fontMetrics.horizontalAdvance(mText[i])));
			textSize.setHeight(textSize.height() + mSpacing + fontMetrics.height());
		}
	}

	QList<QPolygonF> polygonList = fontPath.toFillPolygons();
	for (auto it : polygonList) {
		for (auto point : it) {
			qDebug() << point.x() << point.y();
		}
		qDebug() << "----";
	}
	qDebug() << "----End";

	QText2D::Index offset = 0;
	for (int i = 0; i < polygonList.size(); i++) {
		polygonList[i].pop_back();
		QVector<QPolygonF> singlePolygon;
		singlePolygon << QPolygonF();
		int index = 0;
		for (int j = 0; j < polygonList[i].size(); j++) {
			const QPointF& point = polygonList[i][j];
			singlePolygon.back() << point;
			QText2D::Vertex vertex;
			vertex.position = QVector3D(point.x(), point.y(), 0);
			vertex.baseColor << mColor;
			vertices.push_back(vertex);
			if (polygonList[i][index] == polygonList[i][j]
				&& j != polygonList[i].size() - 1
				&& j != 0) {
				singlePolygon << QPolygonF();
			}
		}
		auto localIndices = mapbox::earcut<QText2D::Index>(singlePolygon);

		for (auto it : localIndices) {
			indices << it + offset;
		}
		offset += polygonList[i].size();
	}

	for (auto& vertex : vertices) {
		vertex.position.setY(textSize.height() - vertex.position.y());
		vertex.position /= 9;
	}

	setVertices(vertices);
	setIndices(indices);
}