#ifndef QSerialization_h__
#define QSerialization_h__

#include <QVariant>

namespace QSerialization {

QByteArray serialize(QObject* src);
void deserialize(QObject* dst, QByteArray data);
QObject* deserialize(QByteArray data);

}

#endif // QSerialization_h__
