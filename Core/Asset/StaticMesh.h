#ifndef StaticMesh_h__
#define StaticMesh_h__

#include "IAsset.h"
#include "Material.h"

namespace Asset {

class StaticMesh : public IAsset {
	using Topology = QRhiGraphicsPipeline::Topology;
	struct Vertex {
		QVector3D position;
		QVector3D normal;
		QVector3D tangent;
		QVector3D bitangent;
		QVector2D texCoord;
	};
	using Index = uint32_t;
private:
	QVector<Vertex> mVertices;
	QVector<Index> mIndices;
	Asset::Material mMaterial;
public:
	virtual IAsset::Type type() override {
		return IAsset::StaticMesh;
	}
};

}


#endif // StaticMesh_h__
