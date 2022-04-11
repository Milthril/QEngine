#ifndef IAsset_h__
#define IAsset_h__

class IAsset {
public:
	enum Type {
		None = 0,
		Material,
		SkyBox,
		StaticMesh,
	};
	virtual IAsset::Type type() = 0;
};


#endif // IAsset_h__
