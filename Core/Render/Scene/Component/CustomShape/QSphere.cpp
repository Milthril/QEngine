#include "QSphere.h"

QSphere::QSphere()
{
	setTopology(QPrimitiveComponent::Topology::Triangles);
	setBufferType(QRhiBuffer::Type::Dynamic);
	recreateVertexData();
}

int QSphere::getStackCount() const
{
	return mStackCount;
}

int QSphere::getSectorCount() const
{
	return mSectorCount;
}

void QSphere::setSubdivide(int stackCount, int sectorCount)
{
	mStackCount = stackCount;
	mSectorCount = sectorCount;
	recreateVertexData();
}

void QSphere::recreateVertexData()
{
	float radius = 1.0f;
	float x, y, z, xy;                              // vertex position
	float sectorStep = 2 * M_PI / getSectorCount();
	float stackStep = M_PI / mStackCount;
	float sectorAngle, stackAngle;

	QVector<QSphere::Vertex> vertices;
	vertices.resize((mStackCount + 1) * (getSectorCount() + 1));
	for (int i = 0; i <= mStackCount; ++i) {
		stackAngle = M_PI / 2 - i * stackStep;        // starting from M_PI/2 to -M_PI/2
		xy = radius * cosf(stackAngle);             // r * cos(u)
		z = radius * sinf(stackAngle);              // r * sin(u)

		for (int j = 0; j <= getSectorCount(); ++j)
		{
			sectorAngle = j * sectorStep;           // starting from 0 to 2M_PI
			x = xy * cosf(sectorAngle);             // r * cos(u) * cos(v)
			y = xy * sinf(sectorAngle);             // r * cos(u) * sin(v)

			QSphere::Vertex& vertex = vertices[i * (getSectorCount() + 1) + j];
			vertex.position = QVector3D(x, y, z);
			vertex.normal = vertex.position;
			vertex.texCoord = QVector2D((float)j / getSectorCount(), (float)i / mStackCount);
		}
	}

	QVector<QSphere::Index> indices;
	int k1, k2;
	for (int i = 0; i < mStackCount; ++i) {
		k1 = i * (getSectorCount() + 1);     // beginning of current stack
		k2 = k1 + getSectorCount() + 1;      // beginning of next stack
		for (int j = 0; j < getSectorCount(); ++j, ++k1, ++k2)
		{
			// 2 triangles per sector excluding first and last stacks
			// k1 => k2 => k1+1
			if (i != 0)
			{
				indices.push_back(k1);
				indices.push_back(k2);
				indices.push_back(k1 + 1);
			}
			// k1+1 => k2 => k2+1
			if (i != (mStackCount - 1))
			{
				indices.push_back(k1 + 1);
				indices.push_back(k2);
				indices.push_back(k2 + 1);
			}
		}
	}
	setVertices(vertices);
	setIndices(indices);
}