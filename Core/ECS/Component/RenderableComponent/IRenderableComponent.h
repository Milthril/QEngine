#ifndef IRenderableComponent_h__
#define IRenderableComponent_h__

#include "ECS\Component\IComponent.h"
#include "ECS\System\RenderSystem\RHI\IRenderable.h"

class IRenderableComponent : public IComponent,public IRenderable {
public:
	virtual ~IRenderableComponent();
	virtual void setupEntity(QEntity* entity) override;

	struct UniformMatrix {
		QGenericMatrix<4, 4, float> MVP;
		QGenericMatrix<4, 4, float> M;
	};
};

#endif // IRenderableComponent_h__