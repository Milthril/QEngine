#ifndef IPositionGenerator_h__
#define IPositionGenerator_h__

#include "QObject"
#include "qvectornd.h"
#include "ExtType\QSubClass.h"

class IPositionGenerator :public QObject {
	Q_OBJECT
public:
	virtual QVector3D generate() = 0;
};

template<>
class QSubClass<IPositionGenerator> : public QSubClassBase<IPositionGenerator> {
public:
	QSubClass();
};

Q_DECLARE_METATYPE(QSubClass<IPositionGenerator>)

#endif // IPositionGenerator_h__
