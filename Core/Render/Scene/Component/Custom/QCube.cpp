#include "QCube.h"

QCube::QCube()
{
	setTopology(QPrimitiveComponent::Triangles);

	QVector3D position;
	QVector3D normal;
	QVector3D tangent;
	QVector3D bitangent;
	QVector2D texCoord;
	QVector4D baseColor;

	QVector<Vertex> vertices = {
		//position   normal  tangent  bitangent  texCoord  baseColor
		Vertex{QVector3D(-1.0f,-1.0f,-1.0f),QVector3D(),QVector3D(),QVector3D(),QVector2D(0.0f, 1.0f),QVector4D()},
		Vertex{QVector3D(-1.0f,-1.0f, 1.0f),QVector3D(),QVector3D(),QVector3D(),QVector2D(1.0f, 1.0f),QVector4D()},
		Vertex{QVector3D(-1.0f, 1.0f, 1.0f),QVector3D(),QVector3D(),QVector3D(),QVector2D(1.0f, 0.0f),QVector4D()},
		Vertex{QVector3D(-1.0f, 1.0f, 1.0f),QVector3D(),QVector3D(),QVector3D(),QVector2D(1.0f, 0.0f),QVector4D()},
		Vertex{QVector3D(-1.0f, 1.0f,-1.0f),QVector3D(),QVector3D(),QVector3D(),QVector2D(0.0f, 0.0f),QVector4D()},
		Vertex{QVector3D(-1.0f,-1.0f,-1.0f),QVector3D(),QVector3D(),QVector3D(),QVector2D(0.0f, 1.0f),QVector4D()},

		Vertex{QVector3D(-1.0f,-1.0f,-1.0f),QVector3D(),QVector3D(),QVector3D(),QVector2D(1.0f, 1.0f),QVector4D()},
		Vertex{QVector3D(1.0f, 1.0f,-1.0f), QVector3D(),QVector3D(),QVector3D(),QVector2D(0.0f, 0.0f),QVector4D()},
		Vertex{QVector3D(1.0f,-1.0f,-1.0f), QVector3D(),QVector3D(),QVector3D(),QVector2D(0.0f, 1.0f),QVector4D()},
		Vertex{QVector3D(-1.0f,-1.0f,-1.0f),QVector3D(),QVector3D(),QVector3D(),QVector2D(1.0f, 1.0f),QVector4D()},
		Vertex{QVector3D(-1.0f, 1.0f,-1.0f),QVector3D(),QVector3D(),QVector3D(),QVector2D(1.0f, 0.0f),QVector4D()},
		Vertex{QVector3D(1.0f, 1.0f,-1.0f), QVector3D(),QVector3D(),QVector3D(),QVector2D(0.0f, 0.0f),QVector4D()},

		Vertex{QVector3D(-1.0f,-1.0f,-1.0f),QVector3D(),QVector3D(),QVector3D(),QVector2D(1.0f, 0.0f),QVector4D()},
		Vertex{QVector3D(1.0f,-1.0f,-1.0f), QVector3D(),QVector3D(),QVector3D(),QVector2D(1.0f, 1.0f),QVector4D()},
		Vertex{QVector3D(1.0f,-1.0f, 1.0f), QVector3D(),QVector3D(),QVector3D(),QVector2D(0.0f, 1.0f),QVector4D()},
		Vertex{QVector3D(-1.0f,-1.0f,-1.0f),QVector3D(),QVector3D(),QVector3D(),QVector2D(1.0f, 0.0f),QVector4D()},
		Vertex{QVector3D(1.0f,-1.0f, 1.0f), QVector3D(),QVector3D(),QVector3D(),QVector2D(0.0f, 1.0f),QVector4D()},
		Vertex{QVector3D(-1.0f,-1.0f, 1.0f),QVector3D(),QVector3D(),QVector3D(),QVector2D(0.0f, 0.0f),QVector4D()},

		Vertex{QVector3D(-1.0f, 1.0f,-1.0f),QVector3D(),QVector3D(),QVector3D(),QVector2D(1.0f, 0.0f),QVector4D()},
		Vertex{QVector3D(-1.0f, 1.0f, 1.0f),QVector3D(),QVector3D(),QVector3D(),QVector2D(0.0f, 0.0f),QVector4D()},
		Vertex{QVector3D(1.0f, 1.0f, 1.0f), QVector3D(),QVector3D(),QVector3D(),QVector2D(0.0f, 1.0f),QVector4D()},
		Vertex{QVector3D(-1.0f, 1.0f,-1.0f),QVector3D(),QVector3D(),QVector3D(),QVector2D(1.0f, 0.0f),QVector4D()},
		Vertex{QVector3D(1.0f, 1.0f, 1.0f), QVector3D(),QVector3D(),QVector3D(),QVector2D(0.0f, 1.0f),QVector4D()},
		Vertex{QVector3D(1.0f, 1.0f,-1.0f), QVector3D(),QVector3D(),QVector3D(),QVector2D(1.0f, 1.0f),QVector4D()},

		Vertex{QVector3D(1.0f, 1.0f,-1.0f),QVector3D(),QVector3D(),QVector3D(),QVector2D(1.0f, 0.0f),QVector4D()},
		Vertex{QVector3D(1.0f, 1.0f, 1.0f),QVector3D(),QVector3D(),QVector3D(),QVector2D(0.0f, 0.0f),QVector4D()},
		Vertex{QVector3D(1.0f,-1.0f, 1.0f),QVector3D(),QVector3D(),QVector3D(),QVector2D(0.0f, 1.0f),QVector4D()},
		Vertex{QVector3D(1.0f,-1.0f, 1.0f),QVector3D(),QVector3D(),QVector3D(),QVector2D(0.0f, 1.0f),QVector4D()},
		Vertex{QVector3D(1.0f,-1.0f,-1.0f),QVector3D(),QVector3D(),QVector3D(),QVector2D(1.0f, 1.0f),QVector4D()},
		Vertex{QVector3D(1.0f, 1.0f,-1.0f),QVector3D(),QVector3D(),QVector3D(),QVector2D(1.0f, 0.0f),QVector4D()},

		Vertex{QVector3D(-1.0f, 1.0f, 1.0f),QVector3D(),QVector3D(),QVector3D(),QVector2D(0.0f, 0.0f),QVector4D()},
		Vertex{QVector3D(-1.0f,-1.0f, 1.0f),QVector3D(),QVector3D(),QVector3D(),QVector2D(0.0f, 1.0f),QVector4D()},
		Vertex{QVector3D(1.0f, 1.0f, 1.0f), QVector3D(),QVector3D(),QVector3D(),QVector2D(1.0f, 0.0f),QVector4D()},
		Vertex{QVector3D(-1.0f,-1.0f, 1.0f),QVector3D(),QVector3D(),QVector3D(),QVector2D(0.0f, 1.0f),QVector4D()},
		Vertex{QVector3D(1.0f,-1.0f, 1.0f), QVector3D(),QVector3D(),QVector3D(),QVector2D(1.0f, 1.0f),QVector4D()},
		Vertex{QVector3D(1.0f, 1.0f, 1.0f), QVector3D(),QVector3D(),QVector3D(),QVector2D(1.0f, 0.0f),QVector4D()},
	};
	setVertices(vertices);
}