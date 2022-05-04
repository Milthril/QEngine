#include "FromComponent.h"
#include "ECS\Component\IComponent.h"
#include "ECS\QEntity.h"
#include "QEngineCoreApplication.h"
#include "ECS\Component\RenderableComponent\QStaticMeshComponent.h"
#include "ECS\Component\RenderableComponent\QSkeletonModelComponent.h"

QCombo QPositionGenerator::FromComponent::getComponent() {
	QCombo combo;
	combo.addItem("Null");
	mCompCache.clear();
	QMap<QString, int> counter;
	for (const auto& it : TheEngine->world()->findChildren<QStaticMeshComponent*>()) {
		QString name = QString("%1-StaitcMesh(%2)").arg(it->getEntity()->objectName()).arg(counter[it->getEntity()->objectName()]++);
		combo.addItem(name);
		mCompCache[name] = it;
	}
	for (const auto& it : TheEngine->world()->findChildren<QSkeletonModelComponent*>()) {
		QString name = QString("%1-SkeletonModel(%2)").arg(it->getEntity()->objectName()).arg(counter[it->getEntity()->objectName()]++);
		combo.addItem(name);
		mCompCache[name] = it;
	}
	return combo;
}

void QPositionGenerator::FromComponent::setComponent(QCombo val) {
	QString current = val.getCurrentItem();
	auto Iter = mCompCache.find(current);
	if (Iter != mCompCache.end()) {
		IComponent* comp = Iter.value();
		setComponent(comp);
	}
	else {
		setComponent(nullptr);
	}
}

void QPositionGenerator::FromComponent::setComponent(IComponent* comp) {
	if (comp == nullptr) {
		mGenFunctor = nullptr;
		return;
	}
	QStaticMeshComponent* staticMesh = dynamic_cast<QStaticMeshComponent*>(comp);
	if (staticMesh) {
		mGenFunctor = [staticMesh, this](QVector<Asset::ParticleSystem::Particle>& particles) {
			if (staticMesh->getStaticMesh()) {
				uint count = staticMesh->getStaticMesh()->getVertices().size();
				const QMatrix4x4& matModel = staticMesh->getEntity()->calculateMatrixModel();
				for (auto& particle : particles) {
					QVector4D vec(staticMesh->getStaticMesh()->getVertices()[mRandom.bounded(count)].position, 1.0);
					particle.position = matModel * vec;
				}
			}
		};
	}
	else {
		QSkeletonModelComponent* skeletonModel = dynamic_cast<QSkeletonModelComponent*>(comp);
		if (skeletonModel) {
			mGenFunctor = [skeletonModel, this](QVector<Asset::ParticleSystem::Particle>& particles) {
				const auto& assetSkeletonModel = skeletonModel->getSkeletonModel();
				const auto& assetSkeleton = skeletonModel->getSkeleton();
				if (assetSkeleton&&assetSkeletonModel) {
					uint count = assetSkeletonModel->getVertices().size();
					const QMatrix4x4& matModel = skeletonModel->getEntity()->calculateMatrixModel();
					for (auto& particle : particles) {
						const auto& vertex = skeletonModel->getSkeletonModel()->getVertices()[mRandom.bounded(count)];
						auto matBone = assetSkeleton->getCurrentPosesMatrix()[vertex.boneIndex[0]] * vertex.boneWeight[0];
						matBone += assetSkeleton->getCurrentPosesMatrix()[vertex.boneIndex[1]] * vertex.boneWeight[1];
						matBone += assetSkeleton->getCurrentPosesMatrix()[vertex.boneIndex[2]] * vertex.boneWeight[2];
						matBone += assetSkeleton->getCurrentPosesMatrix()[vertex.boneIndex[3]] * vertex.boneWeight[3];
						QVector4D vec(vertex.position, 1.0);
						particle.position = matModel * QMatrix4x4(matBone) * vec;
					}
				}
			};
		}
	}
}

void QPositionGenerator::FromComponent::generate(QVector<Asset::ParticleSystem::Particle>& particles) {
	if (mGenFunctor)
		mGenFunctor(particles);
}

