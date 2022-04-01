#include "WinAudioCapture.h"
#include <windows.h>
#include <AudioClient.h>
#include <initguid.h>
#include <setupapi.h>
#include <functiondiscoverykeys_devpkey.h>

#define EXIT_ON_ERROR(hres)  \
              if (FAILED(hres)) { goto Exit; }
#define SAFE_RELEASE(punk)  \
              if ((punk) != NULL)  \
                { (punk)->Release(); (punk) = NULL; }

WinAudioCapture::WinAudioCapture()
{
}

void WinAudioCapture::start()
{
	CoInitialize(NULL);
	CoCreateInstance(__uuidof(MMDeviceEnumerator),
					 nullptr,
					 CLSCTX_ALL,
					 __uuidof(IMMDeviceEnumerator),
					 (void**)&mOutputEnumerator);
	HRESULT hr;
	hr = mOutputEnumerator->RegisterEndpointNotificationCallback(this);
	if (FAILED(hr)) {
		printf("Unable to activate audio client: %x.\n", hr);
		return;
	}

	IMMDeviceCollection* pDeviceCollection = nullptr;
	IMMDevice* pDefaultDevice = nullptr;

	hr = mOutputEnumerator->GetDefaultAudioEndpoint(eRender, eMultimedia, &pDefaultDevice);
	if (FAILED(hr)) {
		printf("Unable to activate audio client: %x.\n", hr);
		return;
	}

	IPropertyStore* pProps = NULL;
	PROPVARIANT propVar;

	hr = pDefaultDevice->OpenPropertyStore(STGM_READ, &pProps);
	if (FAILED(hr)) {
		printf("Unable to activate audio client: %x.\n", hr);
		return;
	}
	PropVariantInit(&propVar);
	hr = pProps->GetValue(PKEY_AudioEngine_DeviceFormat, &propVar);
	if (FAILED(hr)) {
		printf("Unable to activate audio client: %x.\n", hr);
		return;
	}
	WAVEFORMATEX format = *(PWAVEFORMATEX)propVar.blob.pBlobData;
	format.cbSize = 0;
	format.wFormatTag = WAVE_FORMAT_PCM;

	SAFE_RELEASE(pProps);

	mStoped = std::promise<bool>();
	mCaptureThread = std::make_shared<std::thread>([pDefaultDevice, this, format]() {
		IAudioClient* _AudioClient;
		IAudioCaptureClient* _CaptureClient;
		HANDLE _AudioSamplesReadyEvent = NULL;
		HRESULT hr = CoInitializeEx(NULL, COINIT_MULTITHREADED);
		_AudioSamplesReadyEvent = CreateEventEx(NULL, NULL, 0, EVENT_MODIFY_STATE | SYNCHRONIZE);
		if (_AudioSamplesReadyEvent == NULL) {
			printf("Unable to create samples ready event: %d.\n", GetLastError());
			return false;
		}

		hr = pDefaultDevice->Activate(__uuidof(IAudioClient), CLSCTX_ALL, NULL, reinterpret_cast<void**>(&_AudioClient));
		if (FAILED(hr)) {
			printf("Unable to activate audio client: %x.\n", hr);
			return false;
		}

		size_t _FrameSize = (format.wBitsPerSample / 8) * format.nChannels;

		UINT32        _BufferSize;

		//Initialize Audio Engine
		hr = _AudioClient->Initialize(AUDCLNT_SHAREMODE_SHARED,
									  AUDCLNT_STREAMFLAGS_LOOPBACK | AUDCLNT_STREAMFLAGS_EVENTCALLBACK | AUDCLNT_STREAMFLAGS_NOPERSIST
									  , 20 * 10000, 0, &format, NULL);

		mFormat.nChannels = format.nChannels;
		mFormat.nSamplesPerSec = format.nSamplesPerSec;
		mFormat.nAvgBytesPerSec = format.nAvgBytesPerSec;
		mFormat.nBlockAlign = format.nBlockAlign;
		mFormat.wBitsPerSample = format.wBitsPerSample;
		mFormat.cbSize = format.cbSize;

		if (FAILED(hr)) {
			printf("Unable to initialize audio client: %x.\n", hr);
			return false;
		}

		hr = _AudioClient->GetBufferSize(&_BufferSize);
		if (FAILED(hr)) {
			printf("Unable to get audio client buffer: %x. \n", hr);
			return false;
		}
		hr = _AudioClient->SetEventHandle(_AudioSamplesReadyEvent);
		if (FAILED(hr)) {
			printf("Unable to set ready event: %x.\n", hr);
			return false;
		}

		hr = _AudioClient->GetService(IID_PPV_ARGS(&_CaptureClient));
		if (FAILED(hr)) {
			printf("Unable to get new capture client: %x.\n", hr);
			return false;
		}

		hr = _AudioClient->Start();
		if (FAILED(hr)) {
			printf("Unable to get new capture client: %x.\n", hr);
			return false;
		}
		mRunning = true;
		while (mRunning) {
			DWORD waitResult = WaitForSingleObject(_AudioSamplesReadyEvent, INFINITE);
			BYTE* pData;
			UINT32 framesAvailable;
			DWORD  flags;
			hr = _CaptureClient->GetBuffer(&pData, &framesAvailable, &flags, NULL, NULL);
			if (SUCCEEDED(hr)) {
				if (framesAvailable != 0) {
					if (flags & AUDCLNT_BUFFERFLAGS_SILENT) {
					}
					onSubmitAudioData(pData, framesAvailable * _FrameSize);
				}
				hr = _CaptureClient->ReleaseBuffer(framesAvailable);
				if (FAILED(hr)) {
					printf("Unable to release capture buffer: %x!\n", hr);
				}
			}
		}
		SAFE_RELEASE(_CaptureClient);
		SAFE_RELEASE(_AudioClient);
		mStoped.set_value_at_thread_exit(true);
		return true;
	});
	mCaptureThread->detach();
}

void WinAudioCapture::restart()
{
	if (isRunning()) {
		stop();
	}
	start();
}

void WinAudioCapture::stop()
{
	if (!mRunning) {
		return;
	}
	mRunning = false;
	std::future<bool> future = mStoped.get_future();
	future.wait();
}

const std::atomic<bool>& WinAudioCapture::isRunning()
{
	return mRunning;
}

void WinAudioCapture::onSubmitAudioData(unsigned char* data, size_t size)
{
	std::lock_guard<std::mutex> locker(mMutex);
	if (mBufferSize + size >= MAX_BUFFER_SIZE) {
		size_t moveSize = mCacheKeepSize * (mFormat.wBitsPerSample / 8) * mFormat.nChannels;
		memcpy_s(mBuffer, moveSize, mBuffer + mBufferSize - moveSize, moveSize);
		mBufferSize = moveSize;
	}
	memcpy_s(mBuffer + mBufferSize, size, data, size);
	mBufferSize += size;
}

HRESULT STDMETHODCALLTYPE WinAudioCapture::OnDefaultDeviceChanged(EDataFlow flow, ERole role, LPCWSTR pwstrDeviceId)
{
	restart();
	return NULL;
}

HRESULT STDMETHODCALLTYPE WinAudioCapture::OnDeviceAdded(LPCWSTR pwstrDeviceId)
{
	return NULL;
}

HRESULT STDMETHODCALLTYPE WinAudioCapture::OnDeviceRemoved(LPCWSTR pwstrDeviceId)
{
	return NULL;
}

HRESULT STDMETHODCALLTYPE WinAudioCapture::OnDeviceStateChanged(LPCWSTR, DWORD)
{
	restart();
	return 0;
}

HRESULT STDMETHODCALLTYPE WinAudioCapture::OnPropertyValueChanged(LPCWSTR, const PROPERTYKEY)
{
	return 0;
}