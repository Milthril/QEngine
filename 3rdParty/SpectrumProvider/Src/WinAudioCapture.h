#ifndef WinAudioCapture_h__
#define WinAudioCapture_h__

#include <windows.h>
#include <initguid.h>
#include <mmdeviceapi.h>
#include <thread>
#include <atomic>
#include <future>
#include <mutex>

const int MAX_BUFFER_SIZE = 100000;

class WinAudioCapture : public IMMNotificationClient {
	friend class SpectrumMgr;
public:
	WinAudioCapture();
	~WinAudioCapture();
	struct AudioFormat {
		unsigned int nChannels;          /* number of channels (i.e. mono, stereo...) */
		unsigned int nSamplesPerSec;     /* sample rate */
		unsigned int nAvgBytesPerSec;    /* for buffer estimation */
		unsigned int nBlockAlign;        /* block size of data */
		unsigned int wBitsPerSample;     /* number of bits per sample of mono data */
		unsigned int cbSize;             /* the count in bytes of the size of */
	};
	enum CaptrueType {
		Microphone,
		Loudspeaker
	};
public:
	void start(CaptrueType type = Microphone);
	void restart();
	void stop();
	const std::atomic<bool>& isRunning();
private:
	void onSubmitAudioData(unsigned char* data, size_t size);
protected:
	ULONG STDMETHODCALLTYPE AddRef() { return 0; };
	ULONG STDMETHODCALLTYPE Release() { return 0; };
	HRESULT STDMETHODCALLTYPE QueryInterface(
		REFIID, VOID**) {
		return 0;
	}
	HRESULT STDMETHODCALLTYPE OnDefaultDeviceChanged(
		EDataFlow flow, ERole role,
		LPCWSTR pwstrDeviceId);
	HRESULT STDMETHODCALLTYPE OnDeviceAdded(LPCWSTR pwstrDeviceId);
	HRESULT STDMETHODCALLTYPE OnDeviceRemoved(LPCWSTR pwstrDeviceId);
	HRESULT STDMETHODCALLTYPE OnDeviceStateChanged(LPCWSTR, DWORD);
	HRESULT STDMETHODCALLTYPE OnPropertyValueChanged(LPCWSTR, const PROPERTYKEY);
private:
	IMMDeviceEnumerator* mOutputEnumerator = nullptr;
	AudioFormat mFormat;
	std::atomic<bool> mRunning = false;
	std::promise<bool> mStoped;
	std::shared_ptr<std::thread> mCaptureThread;

	unsigned char mBuffer[MAX_BUFFER_SIZE];
	size_t mBufferSize = 0;
	size_t mCacheKeepSize = 1 << 14;

	std::mutex mMutex;
};

#endif // WinAudioCapture_h__
