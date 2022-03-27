#include "SpectrumProvider.h"
#include "SpectrumMgr.h"

SpectrumProvider::SpectrumProvider()
{
	SpectrumMgr::instacne()->addSpectrum(this);
	setSpectrumLevel(11);
	updateFreq();
}

SpectrumProvider::~SpectrumProvider()
{
	SpectrumMgr::instacne()->removeSpectrum(this);
}

void SpectrumProvider::setSpectrumLevel(int level)
{
	mSpectrumLevel = level;
}

int SpectrumProvider::getSpectrumLevel()
{
	return mSpectrumLevel;
}

int SpectrumProvider::getSpectrumCount()
{
	return 1 << mSpectrumLevel;
}

int SpectrumProvider::getLowFreq() const
{
	return mLowFreq;
}

void SpectrumProvider::setLowFreq(int val)
{
	mLowFreq = val;
	updateFreq();
}

int SpectrumProvider::getHighFreq() const
{
	return mHighFreq;
}

void SpectrumProvider::setHighFreq(int val)
{
	mHighFreq = val;
	updateFreq();
}

float SpectrumProvider::getSmoothFactorRange() const
{
	return mSmoothFactorRange;
}

void SpectrumProvider::setSmoothFactorRange(float val)
{
	mSmoothFactorRange = val;
}

float SpectrumProvider::getSmoothFactorRise() const
{
	return mSmoothFactorRise;
}

void SpectrumProvider::setSmoothFactorRise(float val)
{
	mSmoothFactorRise = val;
}

float SpectrumProvider::getSmoothFactorFall() const
{
	return mSmoothFactorFall;
}

void SpectrumProvider::setSmoothFactorFall(float val)
{
	mSmoothFactorFall = val;
}

void SpectrumProvider::setBarSize(int size)
{
	mMutex.lock();
	mBars.resize(std::max(4,size));
	mMutex.unlock();
	updateFreq();
}

int SpectrumProvider::getBarSize() const
{
	return mBars.size();
}

void SpectrumProvider::updateFreq()
{
	float freqScaleOff = 800;
	float step = (float)((log(mHighFreq / (mLowFreq + (freqScaleOff))) / mBars.size()) / log(2.0));
	mBars[0].freq = mLowFreq + freqScaleOff;
	float stepMul = (float)pow(2.0, step);

	for (int i = 1; i < mBars.size(); ++i)
	{
		mBars[i].freq = ((mBars[i - 1].freq * stepMul * 1.0f));
		mBars[i - 1].freq += -freqScaleOff;
	}
}
