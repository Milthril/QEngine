#include "IPositionGenerator.h"
#include "QCubeGenerator.h"
#include "QSphererGenerator.h"

QSubClass<IPositionGenerator>::QSubClass() {
	mClassCreatorMap["Cube"] = []() { return new QCubeGenerator; };
	mClassCreatorMap["Sphere"] = []() { return new QSphereGenerator; };
}