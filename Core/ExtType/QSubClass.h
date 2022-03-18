#ifndef QSubClass_h__
#define QSubClass_h__

#include <functional>
#include <QHash>
#include <QString>
#include <memory>

class QSubClassInterface {
public:
	virtual QStringList getSubList() = 0;
	virtual bool setSubClass(QString name) = 0;
};
//Q_DECLARE_METATYPE(QSubClassInterface)

template<typename _Ty>
class QSubClassBase : public QSubClassInterface, public std::shared_ptr<_Ty> {
public:
	using Creator = std::function<_Ty* ()>;

	virtual QStringList getSubList() {
		return mClassCreatorMap.keys();
	}

	virtual bool setSubClass(QString name) {
		auto subClassCreator = mClassCreatorMap.find(name);
		if (subClassCreator != mClassCreatorMap.end()) {
			this->reset((*subClassCreator)());
			return true;
		}
		return false;
	};
protected:
	QHash<QString, Creator> mClassCreatorMap;
};

template<typename _Ty>
class QSubClass :public QSubClassBase <_Ty> {
	QSubClass() {
		static_assert(false, "you need create it");
	}
};

#endif // QSubClass_h__