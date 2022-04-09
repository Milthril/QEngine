#ifndef QScenePrimitiveGenerator_h__
#define QScenePrimitiveGenerator_h__

class QScenePrimitiveGenerator : public IPositionGenerator {
	Q_OBJECT;
	Q_ENABLE_LUA;
	REGISTER_SUBCLASS(IPositionGenerator, QScenePrimitiveGenerator);
	Q_PROPERTY(QCombo CurrentPrimitive READ getSolid WRITE setSolid);
	QCombo getCurrentPrimitive() const { return mCurrentPrimitive; }
	void setCurrentPrimitive(QCombo val) { mCurrentPrimitive = val; }
	void generate(QVector<QParticleSystem::Particle>& particles) override;
private:
	std::function<void(QVector<QParticleSystem::Particle>&)> mGenFunctor;
}

#endif // QScenePrimitiveGenerator_h__
