#ifndef QSphere_h__
#define QSphere_h__

#include "Render/Scene/Component//QShapeComponent.h"

class QSphere :public QShapeComponent {
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