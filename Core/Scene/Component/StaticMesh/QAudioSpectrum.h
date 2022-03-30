#ifndef QAudioSpectrum_h__
#define QAudioSpectrum_h__

#include "Scene/Component/StaticMesh/QStaticMeshComponent.h"
#include "Audio/QSpectrumProvider.h"
#include "EventHandler/QTickEventHandler.h"

class QAudioSpectrum :public QStaticMeshComponent ,public QTickEventHandler{
	Q_OBJECT
	Q_PROPERTY(QSpectrumProvider* SpectrumProvider READ getSpectrumProvider WRITE setSpectrumProvider)
	Q_PROPERTY(float MaxHeight READ getMaxHeight WRITE setMaxHeight)
	Q_PROPERTY(float Spacing READ getSpacing WRITE setSpacing)
public:
	QAudioSpectrum();

	QSpectrumProvider* getSpectrumProvider() const { return mSpectrumProvider.get(); }
	void setSpectrumProvider(QSpectrumProvider* val) { mSpectrumProvider.reset(val); }

	float getSpacing() const { return mSpacing; }
	void setSpacing(float val) { mSpacing = val; }

	float getMaxHeight() const { return mMaxHeight; }
	void setMaxHeight(float val) { mMaxHeight = val; }

protected:
	void tickEvent() override;
private:
	std::shared_ptr<QSpectrumProvider> mSpectrumProvider = std::make_shared<QSpectrumProvider>();
	float mMaxHeight = 100;
	float mSpacing = 1;
};

#endif // QAudioSpectrum_h__