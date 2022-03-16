#ifndef BindAdjuster_h__
#define BindAdjuster_h__

#include <functional>
#include <QMetaProperty>
#include <QHash>

class Adjuster;

class AdjusterFactory {
	typedef std::function<Adjuster* (QObject*, QString propertyName)> CreateAdjuster;
	AdjusterFactory();
	static AdjusterFactory* getInstance();
public:
	static Adjuster* create(QObject* object = nullptr, QMetaProperty property = {});
	static Adjuster* create(QObject* object = nullptr, QString propertyName = "");
	inline static bool isUpdating_ = false;
private:
	QHash<int, CreateAdjuster> AdjusterCreator_;
};

#endif // BindAdjuster_h__
