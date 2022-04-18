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
	void setVertices(QVector<Asset::StaticMesh::Vertex>&& val) { mVertices = val; }

	const QVector<Asset::StaticMesh::Index>& getIndices() const { return mIndices; }
	void setIndices(QVector<Asset::StaticMesh::Index>&& val) { mIndices = val; }

	QString getMaterialPath() const { return mMaterialPath; }
	void setMaterialPath(QString val) { mMaterialPath = val; }

	void serialize(QDataStream& out) override;
	void deserialize(QDataStream& in) override;
private:
	QVector<Vertex> mVertices;
	QVector<Index> mIndices;
	QString mMaterialPath;
public:
	virtual IAsset::Type type() override {
		return IAsset::StaticMesh;
	}
};
}

Q_ENGINE_DECLARE_ASSET_TYPE(Asset::StaticMesh, IAsset::StaticMesh);


#endif // StaticMesh_h__