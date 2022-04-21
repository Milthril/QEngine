#ifndef QDirectionLightComponent_h__
#define QDirectionLightComponent_h__

#include "ILightComponent.h"

class  QDirectionLightComponent :public ILightComponent {
	Q_OBJECT
	Q_COMPONENT(QDirectionLightComponent)
	Q_PROPERTY(QVector3D Direction READ getDirection WRITE setDirection)
public:
	QDirectionLightComponent();
	QVector3D getDirection();
	void setDirection(QVector3D vec);
	virtual QByteArray getLightingCode() override;
};


#endif // QDirectionLightComponent_h__
