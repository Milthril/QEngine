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

class SpectrumMgr {
public:
	SpectrumMgr();
	static SpectrumMgr* instacne();
	void addSpectrum(SpectrumProvider* spectrum);
	void removeSpectrum(SpectrumProvider* spectrum);

private:
	void start();
	void stop();
protected:
	void calculateSpectrum();

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
private:
	std::map<SpectrumProvider*, SpectrumContex> mSpectrumMap;
	std::shared_ptr<WinAudioCapture> mCapture;

	using FuncPcmToReal = std::function<double(unsigned char*)>;
	FuncPcmToReal mFuncPcmToReal[4];
	double mSpectrumAnalyserMultiplier = 0.15;

	std::atomic<bool> mRunning = false;
	std::promise<bool> mStoped;
	std::shared_ptr<std::thread> mCalculateThread;
	std::mutex mMutex;
};

#endif // SpectrumMgr_h__
