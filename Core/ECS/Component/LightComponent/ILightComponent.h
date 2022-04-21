#ifndef ILightComponent_h__
#define ILightComponent_h__

#include "ECS\Component\IComponent.h"
#include "ECS\System\RenderSystem\RHI\QRhiUniform.h"

class ILightComponent :public IComponent, public QRhiUniform{
public:
	virtual void setupEntity(QEntity* entity) override;
	virtual ~ILightComponent();

	virtual QByteArray getLightingCode() = 0;
};


#endif // ILightComponent_h__
