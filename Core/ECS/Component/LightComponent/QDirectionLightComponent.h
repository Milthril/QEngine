#ifndef QDirectionLightComponent_h__
#define QDirectionLightComponent_h__

#include "ILightComponent.h"

class  QDirectionLightComponent :public ILightComponent {
	Q_OBJECT
	Q_COMPONENT(QDirectionLightComponent)
	Q_PROPERTY(QVector3D Direction READ getDirection WRITE setDirection)
	Q_PROPERTY(QVector3D Radiance READ getRadiance WRITE setRadiance)
public:
	QDirectionLightComponent();

	QVector3D getDirection();
	void setDirection(QVector3D vec);

	QVector3D getRadiance();
	void setRadiance(QVector3D vec);
	virtual QByteArray getLightingCode(QString lightBlockName) override;
};

#endif // QDirectionLightComponent_h__
