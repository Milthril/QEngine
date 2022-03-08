#include "MyGame.h"
#include "Render\Material\QMaterialBaseColor.h"

MyGame::MyGame(int argc, char** argv)
	:QEngine(argc, argv)
{
	cube = std::make_shared<QCube>();;

	material = std::make_shared<QMaterial>();
	material->addParamFloat("Red", 1);
	material->addParamVec3("Green", QVector3D(0, 1, 0));
	material->addParamVec4("Blue", QVector4D(0, 0, 1, 1));
	material->setShadingCode("FragColor = vec4( material.Red ,material.Green.g, material.Blue.b, 1.0);");
	cube->setMaterial(material);
	scene()->addPrimitive(cube);
}

void MyGame::onGameLoop()
{
	float time = QTime::currentTime().msecsSinceStartOfDay();

	material->setParam<float>("Red", (qSin(time / 100) * 5 + 5));
}