#include "QScenePrimitiveGenerator.h"
#include "QQueue"
#include "Scene\QSceneComponent.h"
#include "QEngine.h"
#include "..\..\StaticMesh\QStaticMeshComponent.h"
#include "..\..\SkeletonMesh\QSkeletonMeshComponent.h"

QCombo QScenePrimitiveGenerator::getCurrentPrimitive()  {
	QCombo combo;
	mCompCache.clear();
	QQueue<std::shared_ptr<QSceneComponent>> qObject;
	for (auto comp : Engine->scene()->getSceneCompList()) {
		qObject.push_back(comp);
	}
	while (!qObject.isEmpty()) {
		std::shared_ptr<QSceneComponent> parent = qObject.takeFirst();
		if (parent->type() == QSceneComponent::StaticMesh) {
			combo.addItem(parent->objectName());
			mCompCache[parent->objectName()] = parent;
		}
		if (parent->type() == QSceneComponent::None) {
			auto skeletonMesh = std::dynamic_pointer_cast<QSkeletonMesh>(parent);
			if (skeletonMesh) {
				combo.addItem(skeletonMesh->objectName());
				mCompCache[skeletonMesh->objectName()] = skeletonMesh;
			}
		}
		for (auto child : parent->getChildren()) {
			qObject.push_back(child);
		}
	}

	return combo;
}

void QScenePrimitiveGenerator::setCurrentPrimitive(QCombo val) {
	QString current = val.getCurrentItem();
	auto Iter = mCompCache.find(current);
	if (Iter!=mCompCache.end()) {
		std::shared_ptr<QSceneComponent>& comp = Iter.value();
		if (comp->type() == QSceneComponent::StaticMesh) {
			QStaticMeshComponent* staticMesh = dynamic_cast<QStaticMeshComponent*>(comp.get());
			if (staticMesh) {
				mGenFunctor = [staticMesh, this](QVector<QParticleSystem::Particle>& particles) {
					uint count = staticMesh->getVertexCount();
					const QMatrix4x4& matModel = staticMesh->calculateModelMatrix();
					for (auto& particle : particles) {
						QVector4D vec(staticMesh->getVertices()[mRandom.bounded(count)].position, 1.0);
						particle.position = matModel* vec;
					}
				};
			}
		}
	}
}

void QScenePrimitiveGenerator::generate(QVector<QParticleSystem::Particle>& particles) {
	if (mGenFunctor)
		mGenFunctor(particles);
}
