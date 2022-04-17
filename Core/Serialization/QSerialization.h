#ifndef QSerialization_h__
#define QSerialization_h__

#include <type_traits>
#include <QMetaType>
#include <QDataStream>

QByteArray serializeQObject(QObject* src);
void deserializeQObject(QObject* dst, QByteArray data);
QObject* createQObject(QByteArray data);


template <typename U>
struct has_intrusive_serialize {
	template <typename T, void(T::*)(QDataStream&) = &T::serialize>
	static constexpr bool check(T*) { return true; };   
	static constexpr bool check(...) { return false; };
	static constexpr bool value = check(static_cast<U*>(0));  
};

template <typename T, typename std::enable_if<has_intrusive_serialize<T>::value>::type* = nullptr>
inline QDataStream& operator<<(QDataStream& out,  T& cls) {
	cls.serialize(out);
	return out;
}

template <typename U>
struct has_intrusive_deserialize {
	template <typename T, void(T::*)(QDataStream&) = &T::deserialize>
	static constexpr bool check(T*) { return true; };  
	static constexpr bool check(...) { return false; }; 
	static constexpr bool value = check(static_cast<U*>(0)); 
};

template <typename T, typename std::enable_if<has_intrusive_deserialize<T>::value>::type* = nullptr>
inline QDataStream& operator>>(QDataStream& in, T& cls) {
	cls.deserialize(in);
	return in;
}

template<typename T, typename std::enable_if<std::is_pointer<T>::value && QMetaTypeId2<typename std::remove_pointer<T>::type>::IsBuiltIn>::type* = nullptr>
inline QDataStream& operator<<(QDataStream& out,const T& ptr) {
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


template<typename T, typename std::enable_if<std::is_pointer<T>::value && QMetaTypeId2<typename std::remove_pointer<T>::type>::IsBuiltIn>::type* = nullptr>
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

	QMetaType metaType = QMetaType::fromType<std::remove_pointer<T>::type>();
	Q_ASSERT(metaType.id() == id);
	if (!metaType.isValid())
		return in;
	T newPtr = static_cast<T>(metaType.create());
	if (newPtr) {
		ptr = newPtr;
		operator>>(in, *ptr);
	}
	return in;
}

template<typename T, typename std::enable_if<QMetaTypeId2<std::shared_ptr<T>>::Defined>::type* = nullptr>
inline QDataStream& operator<<(QDataStream& out,const std::shared_ptr<T>& ptr) {
	return operator<<(out, ptr.get());
}

template<typename T, typename std::enable_if<QMetaTypeId2<std::shared_ptr<T>>::Defined>::type* = nullptr>
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
	QMetaType metaType = QMetaType::fromType<std::remove_pointer<T>::type>();
	Q_ASSERT(metaType.id() == id);
	if (!metaType.isValid())
		return in;
	T* newPtr = static_cast<T*>(metaType.create());
	if (newPtr) {
		operator>>(in, *newPtr);
		ptr.reset(newPtr);
	}
	return in;
}

#endif // QSerialization_h__
