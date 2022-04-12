#ifndef IRenderableComponent_h__
#define IRenderableComponent_h__

#include "IComponent.h"
#include "ECS\System\RenderSystem\IRenderable.h"

class IRenderableComponent : public IComponent,public IRenderable {
public:
	virtual ~IRenderableComponent();
	virtual void setupEntity(QEntity* entity) override;
};

#endif // IRenderableComponent_h__
