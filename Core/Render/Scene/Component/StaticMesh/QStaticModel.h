#ifndef QStaticModel_h__
#define QStaticModel_h__

#include "QStaticMeshComponent.h"

class QStaticModel :public QStaticMeshComponent {
public:
	void loadFromFile(const QString filePath);
private:
	QSceneComponent::ProxyType type() override {
		return QSceneComponent::None;
	}
};

#endif // QStaticModel_h__
