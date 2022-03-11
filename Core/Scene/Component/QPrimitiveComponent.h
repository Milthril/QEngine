#ifndef QPrimitiveComponent_h__
#define QPrimitiveComponent_h__

#include "Scene\QSceneComponent.h"
#include "RHI\QRhiDefine.h"
#include "Scene\Material\QMaterial.h"

class QPrimitiveComponent :public QSceneComponent {
	Q_OBJECT
public:
	using Index = uint32_t;
	using Topology = QRhiGraphicsPipeline::Topology;
	bool getVisible() const { return mVisible; }
	void setVisible(bool val) { mVisible = val; }
protected:
	bool mVisible;
};

#endif // QPrimitiveComponent_h__
