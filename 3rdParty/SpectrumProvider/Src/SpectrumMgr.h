#ifndef SpectrumMgr_h__
#define SpectrumMgr_h__

#include <list>
#include <memory>
#include <map>
#include <functional>
#include <future>
#include <fftw3.h>

class SpectrumProvider;
class WinAudioCapture;
class TimeDomainProvider;

class SpectrumMgr {
public:
	SpectrumMgr();
	static SpectrumMgr* instacne();

	void addSpectrum(SpectrumProvider* spectrum);
	void removeSpectrum(SpectrumProvider* spectrum);

	void addTimeDomainProvider(TimeDomainProvider* time);
	void removeTimeDomainProvider(TimeDomainProvider* time);
private:
	void start();
	void stop();

	void requestStart();
	void requestStop();
protected:
	void calculateSpectrum();
	void calculateTimeDomain();

	unsigned char* getDataPtr(size_t dataSize);
	unsigned int getCurrentBitsPerSample();
	unsigned int getCurrentNumOfChannels();

	struct SpectrumContex {
		std::vector<double> mWindow;
		double* mInput = nullptr;
		double* mOutput = nullptr;
		fftw_plan mFFTPlan;
		std::vector<double> mOuputVar;
		std::vector<double> mSmooth;
		std::vector<double> mSmoothFall;
		float rangeLoSmooth = 0.0f;
		float rangeHiSmooth = 0.0f;
	};

	struct TimeContex {
		double mRmsCache;
		double mPeakCache;
	};
private:
	std::map<SpectrumProvider*, SpectrumContex> mSpectrumMap;
	std::map<TimeDomainProvider*, TimeContex> mTimeDomainMap;

	std::shared_ptr<WinAudioCapture> mCapture;
	using FuncPcmToReal = std::function<double(unsigned char*)>;
	FuncPcmToReal mFuncPcmToReal[4];
	FuncPcmToReal mCurrentFunc;

	double mSpectrumAnalyserMultiplier = 0.15;

	std::atomic<bool> mRunning = false;
	std::promise<bool> mStoped;
	std::shared_ptr<std::thread> mCalculateThread;
	std::mutex mMutex;
};

#endif // SpectrumMgr_h__
