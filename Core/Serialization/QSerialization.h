#ifndef QSerialization_h__
#define QSerialization_h__

#include <type_traits>

QByteArray serializeQObject(QObject* src);
void deserializeQObject(QObject* dst, QByteArray data);
QObject* createQObject(QByteArray data);

template<typename T, typename std::enable_if<std::is_pointer<T>::value>::type* = nullptr>
inline QDataStream& operator<<(QDataStream& out, const T& ptr) {
	int id = -1;
	if (ptr != nullptr) {
		id = QMetaType::fromType<std::remove_pointer<T>::type>().id();
	}
	out << id;
	if (ptr != nullptr) {
		operator<<(out, *ptr);
	}
	return out;
}


template<typename T, typename std::enable_if<std::is_pointer<T>::value>::type* = nullptr>
inline QDataStream& operator>>(QDataStream& in, T& ptr) {
	int id;
	in >> id;
	if (id == -1) {
		return in;
	}
	if (ptr != nullptr) {
		operator>>(in, *ptr);
		return in;
	}
	
	QMetaType metaType(id);
	if (!metaType.isValid() && !QMetaType::canConvert(metaType, QMetaType::fromType<std::remove_pointer<T>::type>()))
		return in;

	T newPtr = static_cast<T>(metaType.create());
	if (newPtr) {
		ptr = newPtr;
		operator>>(in, *ptr);
	}
	return in;
}

template<typename T>
inline QDataStream& operator<<(QDataStream& out,const std::shared_ptr<T>& ptr) {
	return operator<<(out, ptr.get());
}

template<typename T>
inline QDataStream& operator>>(QDataStream& in, std::shared_ptr<T>& ptr) {
	int id;
	in >> id;
	if (id == -1) {
		return in;
	}
	if (ptr != nullptr) {
		operator>>(in, *ptr);
		return in;
	}

	QMetaType metaType(id);
	if (!metaType.isValid() && !QMetaType::canConvert(metaType, QMetaType::fromType<T>()))
		return in;
	T* newPtr = static_cast<T*>(metaType.create());
	if (newPtr) {
		operator>>(in, *newPtr);
		ptr.reset(newPtr);
	}
	return in;
}

#endif // QSerialization_h__
