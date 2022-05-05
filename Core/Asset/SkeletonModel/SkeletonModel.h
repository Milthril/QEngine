#ifndef SkeletonModel_h__
#define SkeletonModel_h__

#include "Asset\IAsset.h"
#include "qvectornd.h"
#include "QMatrix4x4"
#include "QList"

namespace Asset {

class SkeletonModel :public IAsset {
	friend class ImporterModelTask;
public:
	using Index = uint32_t;

	struct Vertex {
		QVector3D position;
		QVector3D normal;
		QVector3D tangent;
		QVector3D bitangent;
		QVector2D texCoord;
		uint32_t boneIndex[4] = { 0,0,0,0 };
		QVector4D boneWeight;
	};

	struct Mesh {
		uint32_t mVerticesOffset;
		uint32_t mVerticesRange;
		uint32_t mIndicesOffset;
		uint32_t mIndicesRange;
		uint32_t mMaterialIndex;
	};

	const QVector<SkeletonModel::Mesh>& getMeshList() const { return mMeshList; }
	void setMeshList(QVector<SkeletonModel::Mesh>&& val) { mMeshList = val; }

	const QVector<QString>& getMaterialPathList() const { return mMaterialPathList; }
	void setMaterialPathList(QVector<QString>&& val) { mMaterialPathList = val; }

	QString getSkeletonPath() const { return mSkeletonPath; }
	void setSkeletonPath(QString val) { mSkeletonPath = val; }

	virtual void serialize(QDataStream& out) override;
	virtual void deserialize(QDataStream& in) override;
	const QVector<Asset::SkeletonModel::Vertex>& getVertices() const { return mVertices; }
	const QVector<Asset::SkeletonModel::Index>& getIndices() const { return mIndices; }
protected:
	virtual IAsset::Type type() override {
		return IAsset::SkeletonModel;
	}
private:
	QVector<Mesh> mMeshList;
	QVector<QString> mMaterialPathList;
	QString mSkeletonPath;

	QVector<SkeletonModel::Vertex> mVertices;
	QVector<SkeletonModel::Index> mIndices;
};
}

Q_ENGINE_DECLARE_ASSET_TYPE(Asset::SkeletonModel, IAsset::SkeletonModel);



#endif // SkeletonModel_h__
