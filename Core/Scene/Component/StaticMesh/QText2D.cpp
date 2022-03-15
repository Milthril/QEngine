#include "QText2D.h"
#include "QFontMetrics"
#include "QPainterPath"
#include "QEarCut.h"

QText2D::QText2D(QString text, QFont font, Qt::Orientation o, uint32_t spacing)
	: mText(text)
	, mFont(font)
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

double QText2D::getSpacing() const
{
	return mSpacing;
}

void QText2D::setSpacing(double val)
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
	mFont.setPixelSize(80);
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

	const QList<QPolygonF>& polygonList = fontPath.toSubpathPolygons();

	QList<QList<QPolygonF>> polygons;
	QList<QPolygonF> holes;

	for (const auto& polygon : polygonList) {
		if (PolygonIsClockwise(polygon)) {
			polygons << QList<QPolygonF>{polygon};
		}
		else {
			holes << polygon;
		}
	}
	for (const auto& hole : holes) {
		for (auto& polygon : polygons) {
			if (polygon.first().containsPoint(hole.first(), Qt::FillRule::WindingFill)) {
				polygon << hole;
			}
		}
	}
	QText2D::Index offset = 0;
	for (const auto& polygonSet : polygons) {
		for (auto& polygon : polygonSet) {
			for (auto& point : polygon) {
				QText2D::Vertex vertex;
				vertex.position = QVector3D(point.x(), point.y(), 0);
				vertices.push_back(vertex);
			}
		}
		auto localIndices = QEarcut<QText2D::Index>(polygonSet);
		for (auto it : localIndices) {
			indices << it + offset;
		}
		offset = vertices.size();
	}

	for (auto& vertex : vertices) {
		vertex.position.setY(textSize.height() - vertex.position.y());
		vertex.position -= QVector3D(textSize.width() / 2.0f, textSize.height() / 2.0f, 0.0f);
		vertex.position /= mFont.pixelSize();
	}

	setVertices(vertices);
	setIndices(indices);
}