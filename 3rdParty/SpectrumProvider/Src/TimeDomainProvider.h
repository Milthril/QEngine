#ifndef TimeDomainProvider_h__
#define TimeDomainProvider_h__

#include <memory>
#include <vector>
#include <mutex>

class TimeDomainProvider
{
	friend class SpectrumMgr;
public:
	TimeDomainProvider();
	~TimeDomainProvider();

	int getChannelIndex() const { return mChannelIndex; }
	void setChannelIndex(int val) { mChannelIndex = val; }

	int getSampleCount() const { return mSampleCount; }
	void setSampleCount(int val) { mSampleCount = val; }

	float getSmoothFactorRise() const { return mSmoothFactorRise; }
	void setSmoothFactorRise(float val) { mSmoothFactorRise = val; }

	float getSmoothFactorFall() const { return mSmoothFactorFall; }
	void setSmoothFactorFall(float val) { mSmoothFactorFall = val; }

	float getRms() const { return mRms; }
	float getPeak() const { return mPeak; }
private:
	float mRms = 0.0f;
	float mPeak = 0.0f;
	int mChannelIndex = 0;
	int mSampleCount = 1 << 8;
	float mSmoothFactorRise = 0.6f;
	float mSmoothFactorFall = 0.5f;
};

#endif // TimeDomainProvider_h__