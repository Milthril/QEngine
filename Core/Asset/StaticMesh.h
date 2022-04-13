#ifndef StaticMesh_h__
#define StaticMesh_h__

#include "IAsset.h"
#include "Material.h"
#include "private\qrhi_p.h"

namespace Asset {

class StaticMesh : public IAsset {
public:
	using Topology = QRhiGraphicsPipeline::Topology;

	struct Vertex {
		QVector3D position;
		QVector3D normal;
		QVector3D tangent;
		QVector3D bitangent;
		QVector2D texCoord;

		friend QDataStream& operator<<(QDataStream& out, const Vertex& var){
			out << var.position << var.normal << var.tangent << var.bitangent << var.texCoord;
			return out;
		}
		friend QDataStream& operator>>(QDataStream& in, Vertex& var) {
			in >> var.position >> var.normal >> var.tangent >> var.bitangent >> var.texCoord;
			return in;
		}
	};
	using Index = uint32_t;

	const QVector<Asset::StaticMesh::Vertex>& getVertices() const { return mVertices; }
	void setVertices(QVector<Asset::StaticMesh::Vertex> val) { mVertices = val; }

	const QVector<Asset::StaticMesh::Index>& getIndices() const { return mIndices; }
	void setIndices(QVector<Asset::StaticMesh::Index> val) { mIndices = val; }

	std::shared_ptr<Asset::Material>& getMaterial() { return mMaterial; }
	void setMaterial(std::shared_ptr<Asset::Material>  val) { mMaterial = val; }

	friend QDataStream& operator<<(QDataStream& out, const StaticMesh& var);
	friend QDataStream& operator>>(QDataStream& in, StaticMesh& var);
private:
	QVector<Vertex> mVertices;
	QVector<Index> mIndices;
	std::shared_ptr<Asset::Material> mMaterial;
public:
	virtual IAsset::Type type() override {
		return IAsset::StaticMesh;
	}
};
}
Q_DECLARE_BUILTIN_METATYPE(Asset::StaticMesh, 100002, Asset::StaticMesh);

#endif // StaticMesh_h__