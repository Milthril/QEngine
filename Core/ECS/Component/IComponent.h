#ifndef IComponent_h__
#define IComponent_h__

#include <QObject>
#include <QMap>

class QEntity;

class IComponent :public QObject {
	friend class QEntity;
	Q_OBJECT
public:
	IComponent() {}
	virtual void setupEntity(QEntity* entity) {
		mEntity = entity;
	}
protected:
	QEntity* mEntity = nullptr;
};

class ComponentFactory {
public:
	static ComponentFactory* instance();
	using ComponentCreator = std::function<IComponent* ()>;

	template<typename Component>
	void registerComponent(QString name) {
		mComponentCreatorMap[name] = []() {
			return new Component();
		};
	}

	IComponent* createComponent(QString name);

	QStringList getComponentList() {
		return mComponentCreatorMap.keys();
	}
private:
	QMap<QString, ComponentCreator> mComponentCreatorMap;
};

#define Q_COMPONENT(TypeName) \
	struct TypeName##Register{ \
		TypeName##Register(){ \
			ComponentFactory::instance()->registerComponent<TypeName>(#TypeName); \
		} \
	};	\
	inline static TypeName##Register __componentRegister;


#endif // QComponent_h__