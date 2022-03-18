#ifndef QSubClass_h__
#define QSubClass_h__

#include <QHash>
#include <QString>
#include <memory>
#include "qobjectdefs.h"
#include <QDebug>

class QSubClassMgr {
public:
	struct Info{
		const QMetaObject* parent;
		QList<const QMetaObject*> children;
	};
	static QSubClassMgr* instance();
	void registerInfo(const QMetaObject* parent, const QMetaObject* child);
	template<typename Parent,typename Child>
	void registerInfo() {
			registerInfo(&Parent::staticMetaObject, &Child::staticMetaObject);
	}

	Info getInfo(const QString& parentName);
	QStringList getSubList(const QString& parentName);
private:
	QHash<QString,Info> mInfo;
};

#define REGISTER_SUBCLASS(Parent,Child) \
		struct __Register {\
			__Register() {\
				QSubClassMgr::instance()->registerInfo<Parent,Child>();\
			}\
		};\
		inline static __Register __register;

template<typename _Ty>
class QSubClass : public std::shared_ptr<_Ty> {
public:
	QSubClass() {}
	template <class _Ty2>
	QSubClass(const _Ty2& _Other) :std::shared_ptr<_Ty>(_Other) {
	
	}
	QStringList getSubList() {
		return QSubClassMgr::instance()->getSubList(_Ty::staticMetaObject.className());
	}
	QString getCurrentClass() {
		if (this->get() != nullptr) {
			return this->get()->metaObject()->className();
		}
		return "";
	}

	template<typename Child>
	bool setSubClass() {
		return setSubClass(Child::staticMetaObject.className());
	}
	bool setSubClass(const QString& name) {
		const QSubClassMgr::Info& info = QSubClassMgr::instance()->getInfo(_Ty::staticMetaObject.className());
		for (auto& meta : info.children) {
			if (meta->className() == name) {
				this->reset(dynamic_cast<_Ty*>(meta->newInstance()));
				return this->get()!=nullptr;
			}
		}
		return false;
	};

};



#endif // QSubClass_h__