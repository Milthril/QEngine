#ifndef ImporterModelTask_h__
#define ImporterModelTask_h__

#include "ImporterTask.h"

class ImporterModelTask :public ImporterTask {
public:
	virtual void executable() override;

	inline static QStringList mTextureNameMap = { "None","Diffuse","Specular","Ambient","Emissive","Height","Normals","Shininess","Opacity","Displacement","Lightmap","Reflection",
		"BaseColor","NormalCamera","EmissionColor","Metalness","DiffuseRoughnes","AmbientOcclusion",
		"Unknown","Sheen","Clearcoat","Transmission" };
}; 


#endif // ImporterModelTask_h__
