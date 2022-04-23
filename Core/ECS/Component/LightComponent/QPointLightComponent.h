#ifndef QPointLightComponent_h__
#define QPointLightComponent_h__

#include "ILightComponent.h"

class  QPointLightComponent :public ILightComponent {
	Q_OBJECT
		Q_COMPONENT(QPointLightComponent)
		Q_PROPERTY(QVector3D Radiance READ getRadiance WRITE setRadiance)
		Q_PROPERTY(float Distance READ getDistance WRITE setDistance)
public:
	QPointLightComponent();

	QVector3D getRadiance();
	void setRadiance(QVector3D vec);

	float getDistance();
	void setDistance(float var);

	virtual void updateResource(QRhiResourceUpdateBatch* batch) override;
	virtual QByteArray getLightingCode(QString lightBlockName) override;
private:
	float mDistance;
};

#endif // QPointLightComponent_h__
