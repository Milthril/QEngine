#include "QAudioSpectrum.h"
#include "QDateTime"
#include "QCube.h"

QAudioSpectrum::QAudioSpectrum()
{
	setTopology(QPrimitiveComponent::Topology::Triangles);
	setBufferType(QRhiBuffer::Dynamic);
}

void QAudioSpectrum::tickEvent(float deltaSeconds)
{
	QVector<Vertex> vertices;
	const auto& bars = mSpectrumProvider->getBars();
	float width = bars.size() * 2 + mSpacing * (bars.size() - 1);
	float offset = -width / 2;
	for (int i = 0; i < bars.size(); i++) {
		float height = mMaxHeight * bars[i].amp;
		QVector<Vertex> cube = {
			//position					normal		tangent		bitangent   texCoord			baseColor
			  Vertex{QVector3D(-1.0f,-1.0f,-1.0f),QVector3D(),QVector3D(),QVector3D(),QVector2D(0.0f, 1.0f)},
			  Vertex{QVector3D(-1.0f,-1.0f, 1.0f),QVector3D(),QVector3D(),QVector3D(),QVector2D(1.0f, 1.0f)},
			  Vertex{QVector3D(-1.0f, height, 1.0f),QVector3D(),QVector3D(),QVector3D(),QVector2D(1.0f, 0.0f)},
			  Vertex{QVector3D(-1.0f, height, 1.0f),QVector3D(),QVector3D(),QVector3D(),QVector2D(1.0f, 0.0f)},
			  Vertex{QVector3D(-1.0f, height,-1.0f),QVector3D(),QVector3D(),QVector3D(),QVector2D(0.0f, 0.0f)},
			  Vertex{QVector3D(-1.0f,-1.0f,-1.0f),QVector3D(),QVector3D(),QVector3D(),QVector2D(0.0f, 1.0f)},

			  Vertex{QVector3D(-1.0f,-1.0f,-1.0f),QVector3D(),QVector3D(),QVector3D(),QVector2D(1.0f, 1.0f)},
			  Vertex{QVector3D(1.0f, height,-1.0f), QVector3D(),QVector3D(),QVector3D(),QVector2D(0.0f, 0.0f)},
			  Vertex{QVector3D(1.0f,-1.0f,-1.0f), QVector3D(),QVector3D(),QVector3D(),QVector2D(0.0f, 1.0f)},
			  Vertex{QVector3D(-1.0f,-1.0f,-1.0f),QVector3D(),QVector3D(),QVector3D(),QVector2D(1.0f, 1.0f)},
			  Vertex{QVector3D(-1.0f, height,-1.0f),QVector3D(),QVector3D(),QVector3D(),QVector2D(1.0f, 0.0f)},
			  Vertex{QVector3D(1.0f, height,-1.0f), QVector3D(),QVector3D(),QVector3D(),QVector2D(0.0f, 0.0f)},

			  Vertex{QVector3D(-1.0f,-1.0f,-1.0f),QVector3D(),QVector3D(),QVector3D(),QVector2D(1.0f, 0.0f)},
			  Vertex{QVector3D(1.0f,-1.0f,-1.0f), QVector3D(),QVector3D(),QVector3D(),QVector2D(1.0f, 1.0f)},
			  Vertex{QVector3D(1.0f,-1.0f, 1.0f), QVector3D(),QVector3D(),QVector3D(),QVector2D(0.0f, 1.0f)},
			  Vertex{QVector3D(-1.0f,-1.0f,-1.0f),QVector3D(),QVector3D(),QVector3D(),QVector2D(1.0f, 0.0f)},
			  Vertex{QVector3D(1.0f,-1.0f, 1.0f), QVector3D(),QVector3D(),QVector3D(),QVector2D(0.0f, 1.0f)},
			  Vertex{QVector3D(-1.0f,-1.0f, 1.0f),QVector3D(),QVector3D(),QVector3D(),QVector2D(0.0f, 0.0f)},

			  Vertex{QVector3D(-1.0f, height,-1.0f),QVector3D(),QVector3D(),QVector3D(),QVector2D(1.0f, 0.0f)},
			  Vertex{QVector3D(-1.0f, height, 1.0f),QVector3D(),QVector3D(),QVector3D(),QVector2D(0.0f, 0.0f)},
			  Vertex{QVector3D(1.0f,  height, 1.0f), QVector3D(),QVector3D(),QVector3D(),QVector2D(0.0f, 1.0f)},
			  Vertex{QVector3D(-1.0f, height,-1.0f),QVector3D(),QVector3D(),QVector3D(),QVector2D(1.0f, 0.0f)},
			  Vertex{QVector3D(1.0f,  height, 1.0f), QVector3D(),QVector3D(),QVector3D(),QVector2D(0.0f, 1.0f)},
			  Vertex{QVector3D(1.0f,  height,-1.0f), QVector3D(),QVector3D(),QVector3D(),QVector2D(1.0f, 1.0f)},

			  Vertex{QVector3D(1.0f, height,-1.0f),QVector3D(),QVector3D(),QVector3D(),QVector2D(1.0f, 0.0f)},
			  Vertex{QVector3D(1.0f, height, 1.0f),QVector3D(),QVector3D(),QVector3D(),QVector2D(0.0f, 0.0f)},
			  Vertex{QVector3D(1.0f,-1.0f, 1.0f),QVector3D(),QVector3D(),QVector3D(),QVector2D(0.0f, 1.0f)},
			  Vertex{QVector3D(1.0f,-1.0f, 1.0f),QVector3D(),QVector3D(),QVector3D(),QVector2D(0.0f, 1.0f)},
			  Vertex{QVector3D(1.0f,-1.0f,-1.0f),QVector3D(),QVector3D(),QVector3D(),QVector2D(1.0f, 1.0f)},
			  Vertex{QVector3D(1.0f, height,-1.0f),QVector3D(),QVector3D(),QVector3D(),QVector2D(1.0f, 0.0f)},

			  Vertex{QVector3D(-1.0f, height, 1.0f),QVector3D(),QVector3D(),QVector3D(),QVector2D(0.0f, 0.0f)},
			  Vertex{QVector3D(-1.0f,-1.0f, 1.0f),QVector3D(),QVector3D(),QVector3D(),QVector2D(0.0f, 1.0f)},
			  Vertex{QVector3D(1.0f,  height, 1.0f), QVector3D(),QVector3D(),QVector3D(),QVector2D(1.0f, 0.0f)},
			  Vertex{QVector3D(-1.0f,-1.0f, 1.0f),QVector3D(),QVector3D(),QVector3D(),QVector2D(0.0f, 1.0f)},
			  Vertex{QVector3D(1.0f, -1.0f, 1.0f), QVector3D(),QVector3D(),QVector3D(),QVector2D(1.0f, 1.0f)},
			  Vertex{QVector3D(1.0f,  height, 1.0f), QVector3D(),QVector3D(),QVector3D(),QVector2D(1.0f, 0.0f)},
		};
		for (auto& vertex : cube) {
			vertex.position += QVector3D(offset, 0, 0);
		}
		offset += 2 + mSpacing;
		vertices << cube;
	}
	setVertices(std::move(vertices));
	autoFillIndices();
}
