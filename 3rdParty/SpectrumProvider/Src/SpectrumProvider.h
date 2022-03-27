#ifndef SpectrumProvider_h__
#define SpectrumProvider_h__

#include <memory>
#include <vector>
#include <mutex>

class SpectrumProvider
{
	friend class SpectrumMgr;
public:
	SpectrumProvider();
	~SpectrumProvider();

	int getChannelIndex() const { return mChannelIndex; }
	void setChannelIndex(int val) { mChannelIndex = val; }

	void setSpectrumLevel(int val);
	int getSpectrumLevel();

	int getSpectrumCount();

	int getLowFreq() const;
	void setLowFreq(int val);

	int getHighFreq() const;
	void setHighFreq(int val);

	float getSmoothFactorRange() const;
	void setSmoothFactorRange(float val);

	float getSmoothFactorRise() const;
	void setSmoothFactorRise(float val);

	float getSmoothFactorFall() const;
	void setSmoothFactorFall(float val);

	struct Element {
		float freq;
		float amp;
	};

	void setBarSize(int size);
	int getBarSize() const;
	const std::vector<SpectrumProvider::Element>& getBars() const { return mBars; }
protected:
	void updateFreq();
private:
	short mChannelIndex = 0;
	int mSpectrumLevel = 12;
	int mLowFreq = 0;
	int mHighFreq = 8000;

	float mSmoothFactorRange = 0.5f;
	float mSmoothFactorRise = 0.6f;
	float mSmoothFactorFall = 0.009f;//1.0f - no smooth
public:
	std::mutex mMutex;
	std::vector<Element> mBars= std::vector<Element>(100);
};

#endif // SpectrumProvider_h__