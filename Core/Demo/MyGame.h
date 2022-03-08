#ifndef MyGame_h__
#define MyGame_h__

#include "QEngine.h"
#include "QDateTime"
#include "Render\Scene\Component\CustomShape\QCube.h"
#include "Render\Scene\Component\QLightComponent.h"
#include "Render\Scene\Component\CustomShape\QSphere.h"
#include "Render\Scene\Component\CustomShape\QText2D.h"
#include "Render\Scene\Component\QParticleComponent.h"
#include "qrandom.h"
#include <memory>
#include "Render\Material\QMaterialBaseColor.h"

class MyGame :public QEngine {
public:
	MyGame(int argc, char** argv);

	std::shared_ptr<QCube> cube;
	std::shared_ptr<QParticleComponent> particles = std::make_shared<QParticleComponent>();
	std::shared_ptr<QSphere> sphere = std::make_shared<QSphere>();
	std::shared_ptr<QText2D> text = std::make_shared<QText2D>("default");
	std::shared_ptr<QMaterial> material;
	QRandomGenerator rand;
protected:
	void onGameLoop() override;
};

#endif // MyGame_h__
