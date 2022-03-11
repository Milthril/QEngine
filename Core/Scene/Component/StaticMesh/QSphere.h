#ifndef QSphere_h__
#define QSphere_h__

#include "Scene/Component/StaticMesh/QStaticMeshComponent.h"

class QSphere :public QStaticMeshComponent {
public:
	QSphere();
	int getStackCount() const;
	int getSectorCount() const;
	void setSubdivide(int stackCount, int sectorCount);
private:
	void recreateVertexData();
	int mStackCount = 50;
	int mSectorCount = 50;
};

#endif // QSphere_h__