#ifndef SkeletonModel_h__
#define SkeletonModel_h__

#include "Asset\IAsset.h"
#include "qvectornd.h"
#include "QMatrix4x4"
#include "QList"

namespace Asset {

class SkeletonModel :public IAsset {
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
		QVector<SkeletonModel::Vertex> mVertices;
		QVector<SkeletonModel::Index> mIndices;
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
protected:
	virtual IAsset::Type type() override {
		return IAsset::SkeletonModel;
	}

private:
	QVector<Mesh> mMeshList;
	QVector<QString> mMaterialPathList;
	QString mSkeletonPath;
};

}

Q_ENGINE_DECLARE_ASSET_TYPE(Asset::SkeletonModel, IAsset::SkeletonModel);



#endif // SkeletonModel_h__
