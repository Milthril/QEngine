#include "QCube.h"

QCube::QCube()
{
	setTopology(QPrimitiveComponent::Topology::Triangles);
	setBufferType(QRhiBuffer::Static);

	QVector<Vertex> getVertices = {
		//position					normal		tangent		bitangent   texCoord			baseColor
		  Vertex{QVector3D(-1.0f,-1.0f,-1.0f),QVector3D(),QVector3D(),QVector3D(),QVector2D(0.0f, 1.0f)},
		  Vertex{QVector3D(-1.0f,-1.0f, 1.0f),QVector3D(),QVector3D(),QVector3D(),QVector2D(1.0f, 1.0f)},
		  Vertex{QVector3D(-1.0f, 1.0f, 1.0f),QVector3D(),QVector3D(),QVector3D(),QVector2D(1.0f, 0.0f)},
		  Vertex{QVector3D(-1.0f, 1.0f, 1.0f),QVector3D(),QVector3D(),QVector3D(),QVector2D(1.0f, 0.0f)},
		  Vertex{QVector3D(-1.0f, 1.0f,-1.0f),QVector3D(),QVector3D(),QVector3D(),QVector2D(0.0f, 0.0f)},
		  Vertex{QVector3D(-1.0f,-1.0f,-1.0f),QVector3D(),QVector3D(),QVector3D(),QVector2D(0.0f, 1.0f)},

		  Vertex{QVector3D(-1.0f,-1.0f,-1.0f),QVector3D(),QVector3D(),QVector3D(),QVector2D(1.0f, 1.0f)},
		  Vertex{QVector3D(1.0f, 1.0f,-1.0f), QVector3D(),QVector3D(),QVector3D(),QVector2D(0.0f, 0.0f)},
		  Vertex{QVector3D(1.0f,-1.0f,-1.0f), QVector3D(),QVector3D(),QVector3D(),QVector2D(0.0f, 1.0f)},
		  Vertex{QVector3D(-1.0f,-1.0f,-1.0f),QVector3D(),QVector3D(),QVector3D(),QVector2D(1.0f, 1.0f)},
		  Vertex{QVector3D(-1.0f, 1.0f,-1.0f),QVector3D(),QVector3D(),QVector3D(),QVector2D(1.0f, 0.0f)},
		  Vertex{QVector3D(1.0f, 1.0f,-1.0f), QVector3D(),QVector3D(),QVector3D(),QVector2D(0.0f, 0.0f)},

		  Vertex{QVector3D(-1.0f,-1.0f,-1.0f),QVector3D(),QVector3D(),QVector3D(),QVector2D(1.0f, 0.0f)},
		  Vertex{QVector3D(1.0f,-1.0f,-1.0f), QVector3D(),QVector3D(),QVector3D(),QVector2D(1.0f, 1.0f)},
		  Vertex{QVector3D(1.0f,-1.0f, 1.0f), QVector3D(),QVector3D(),QVector3D(),QVector2D(0.0f, 1.0f)},
		  Vertex{QVector3D(-1.0f,-1.0f,-1.0f),QVector3D(),QVector3D(),QVector3D(),QVector2D(1.0f, 0.0f)},
		  Vertex{QVector3D(1.0f,-1.0f, 1.0f), QVector3D(),QVector3D(),QVector3D(),QVector2D(0.0f, 1.0f)},
		  Vertex{QVector3D(-1.0f,-1.0f, 1.0f),QVector3D(),QVector3D(),QVector3D(),QVector2D(0.0f, 0.0f)},

		  Vertex{QVector3D(-1.0f, 1.0f,-1.0f),QVector3D(),QVector3D(),QVector3D(),QVector2D(1.0f, 0.0f)},
		  Vertex{QVector3D(-1.0f, 1.0f, 1.0f),QVector3D(),QVector3D(),QVector3D(),QVector2D(0.0f, 0.0f)},
		  Vertex{QVector3D(1.0f, 1.0f, 1.0f), QVector3D(),QVector3D(),QVector3D(),QVector2D(0.0f, 1.0f)},
		  Vertex{QVector3D(-1.0f, 1.0f,-1.0f),QVector3D(),QVector3D(),QVector3D(),QVector2D(1.0f, 0.0f)},
		  Vertex{QVector3D(1.0f, 1.0f, 1.0f), QVector3D(),QVector3D(),QVector3D(),QVector2D(0.0f, 1.0f)},
		  Vertex{QVector3D(1.0f, 1.0f,-1.0f), QVector3D(),QVector3D(),QVector3D(),QVector2D(1.0f, 1.0f)},

		  Vertex{QVector3D(1.0f, 1.0f,-1.0f),QVector3D(),QVector3D(),QVector3D(),QVector2D(1.0f, 0.0f)},
		  Vertex{QVector3D(1.0f, 1.0f, 1.0f),QVector3D(),QVector3D(),QVector3D(),QVector2D(0.0f, 0.0f)},
		  Vertex{QVector3D(1.0f,-1.0f, 1.0f),QVector3D(),QVector3D(),QVector3D(),QVector2D(0.0f, 1.0f)},
		  Vertex{QVector3D(1.0f,-1.0f, 1.0f),QVector3D(),QVector3D(),QVector3D(),QVector2D(0.0f, 1.0f)},
		  Vertex{QVector3D(1.0f,-1.0f,-1.0f),QVector3D(),QVector3D(),QVector3D(),QVector2D(1.0f, 1.0f)},
		  Vertex{QVector3D(1.0f, 1.0f,-1.0f),QVector3D(),QVector3D(),QVector3D(),QVector2D(1.0f, 0.0f)},

		  Vertex{QVector3D(-1.0f, 1.0f, 1.0f),QVector3D(),QVector3D(),QVector3D(),QVector2D(0.0f, 0.0f)},
		  Vertex{QVector3D(-1.0f,-1.0f, 1.0f),QVector3D(),QVector3D(),QVector3D(),QVector2D(0.0f, 1.0f)},
		  Vertex{QVector3D(1.0f, 1.0f, 1.0f), QVector3D(),QVector3D(),QVector3D(),QVector2D(1.0f, 0.0f)},
		  Vertex{QVector3D(-1.0f,-1.0f, 1.0f),QVector3D(),QVector3D(),QVector3D(),QVector2D(0.0f, 1.0f)},
		  Vertex{QVector3D(1.0f,-1.0f, 1.0f), QVector3D(),QVector3D(),QVector3D(),QVector2D(1.0f, 1.0f)},
		  Vertex{QVector3D(1.0f, 1.0f, 1.0f), QVector3D(),QVector3D(),QVector3D(),QVector2D(1.0f, 0.0f)},
	};
	setVertices(getVertices);
}