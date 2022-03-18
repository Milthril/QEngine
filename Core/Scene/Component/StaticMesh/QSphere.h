#ifndef QSphere_h__
#define QSphere_h__

#include "Scene/Component/StaticMesh/QStaticMeshComponent.h"

class QSphere :public QStaticMeshComponent {
	Q_OBJECT
	REGISTER_SUBCLASS(QStaticMeshComponent,QSphere)
public:
	Q_INVOKABLE QSphere();
	int getStackCount() const;
	int getSectorCount() const;
	void setSubdivide(int stackCount, int sectorCount);
private:
	void recreateVertexData();
	int mStackCount = 50;
	int mSectorCount = 50;
};

#endif // QSphere_h__