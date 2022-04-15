#include "IComponent.h"
#include "ECS/QEntity.h"

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

QString IComponent::getEntityIdVec4String() const {
	QEntity::ID mId = mEntity->GetId();
	int r = (mId & 0x000000FF) >> 0;
	int g = (mId & 0x0000FF00) >> 8;
	int b = (mId & 0x00FF0000) >> 16;
	int a = (mId & 0xFF000000) >> 24;
	return QString("vec4(%1,%2,%3,%4)").arg(r / 255.0f).arg(g / 255.0f).arg(b / 255.0f).arg(a / 255.0f);
}
