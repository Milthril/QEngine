﻿#ifndef QParticleUpdater_h__
#define QParticleUpdater_h__

#include "RHI\QRhiUniform.h"

class QParticleUpdater :public QRhiUniform {
public:
	QParticleUpdater();
	QByteArray getUpdateCode() const { return mUpdateCode; }
	void setUpdateCode(QByteArray val) { mUpdateCode = val; }
protected:
	QByteArray mUpdateCode;
};

#endif // QParticleUpdater_h__