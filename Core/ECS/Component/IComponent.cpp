#include "IComponent.h"

ComponentFactory* ComponentFactory::instance() {
	static ComponentFactory ins;
	return &ins;
}

IComponent* ComponentFactory::createComponent(QString name) {
	auto Iter = mComponentCreatorMap.find(name);
	if (Iter != mComponentCreatorMap.end())
		return (*Iter)();
	return nullptr;
}

