#ifndef SpectrumMgr_h__
#define SpectrumMgr_h__

#include <list>
#include <memory>
#include <map>
#include "kfr\dft\fft.hpp"
#include <functional>
#include <future>

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
		kfr::univector<double> mWindow;
		kfr::univector<double> mInput;
		kfr::univector<double> mOutput;
		std::shared_ptr<kfr::dct_plan<double>> mDctPlan;
		std::vector<kfr::u8> mTemp;
		kfr::univector<double> mSmooth;
		kfr::univector<double> mOuputVar;
		kfr::univector<double> mSmoothFall;
		float rangeLoSmooth = 0.0f;
		float rangeHiSmooth = 0.0f;
	};
private:
	std::map<SpectrumProvider*,SpectrumContex> mSpectrumMap;
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
