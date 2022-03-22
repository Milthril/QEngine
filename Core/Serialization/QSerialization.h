#ifndef QSerialization_h__
#define QSerialization_h__

#include <QVariant>

#define Q_AUTO(type,name)\
    Q_PROPERTY(type name READ get_##name WRITE set_##name NOTIFY name##Changed) \
    type& get_##name() { return name; } \
    void set_##name(type var){ \
        if(name==var) \
            return; \
        name = var;  \
        emit name##Changed(QVariant::fromValue(var)); \
    } \
    Q_SIGNAL void name##Changed(QVariant); \
    type name

namespace QSerialization {
QByteArray serialize(QObject* src);

void deserialize(QObject* dst, QByteArray data);

QObject* deserialize(QByteArray data);
}

#endif // QSerialization_h__
