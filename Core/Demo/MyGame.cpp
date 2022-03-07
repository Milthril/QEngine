#include "MyGame.h"

MyGame::MyGame(int argc, char** argv)
	:QEngine(argc, argv)
{
	particles->setShape(cube);
	particles->setPosition(QVector3D(-0.8, 0.8, 0));

	sphere->setPosition(QVector3D(0.8, 0.8, 0));
	sphere->setScale(QVector3D(0.4, 0.4, 0.4));

	text->setPosition(QVector3D(0, -0.8, 0));

	scene()->addPrimitive(text);
	scene()->addPrimitive(sphere);
	scene()->addPrimitive(particles);
}

void MyGame::onGameLoop()
{
	float time = QTime::currentTime().msecsSinceStartOfDay();
	cube->setDefaultBaseColor(QVector4D(0.1, 0.5, 0.9, 1) * (sin(time / 100) * 5 + 6));
	QVector<QParticleComponent::Particle> particleList(10);
	for (auto& particle : particleList) {
		particle.velocity = QVector3D(rand.bounded(-50, 50) / 20000.0, rand.bounded(-50, 50) / 20000.0, 0);
		particle.rotation = QVector3D(rand.bounded(360.0), rand.bounded(360.0), rand.bounded(360.0)).toVector4D();
		particle.scaling = QVector3D(0.05, 0.05, 0.05).toVector4D();
	}
	particles->createPartilces(particleList);

	sphere->setDefaultBaseColor(QVector4D(0.8, 0.5, 0.4, 1) * (20 + 20 * qSin(time / 500)));

	text->setText(QTime::currentTime().toString());
	text->setDefaultBaseColor(QVector4D(0.5, 0.1, 0.8, 1) * (20 + 20 * qSin(time / 600)));
}