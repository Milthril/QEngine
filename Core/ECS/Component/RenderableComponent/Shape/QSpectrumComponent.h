#ifndef QSpectrumComponent_h__
#define QSpectrumComponent_h__

#include "qvectornd.h"
#include "IShapeComponent.h"
#include "SpectrumProvider.h"
#include "ExtType\QBoundedInt.h"
#include "ExtType\QBoundedDouble.h"
#include "ExtType\QRange.h"

class QSpectrumComponent :public IShapeComponent , public SpectrumProvider {
	Q_OBJECT
		Q_COMPONENT(QSpectrumComponent)
		Q_PROPERTY(float MaxHeight READ getMaxHeight WRITE setMaxHeight)
		Q_PROPERTY(float Spacing READ getSpacing WRITE setSpacing)
		Q_PROPERTY(QBoundedInt ChannelIndex READ getChannelIndexLimit WRITE setChannelIndexLimit)
		Q_PROPERTY(QBoundedInt BarCount  READ getBarCountLimit WRITE setBarCountLimit)
		Q_PROPERTY(QRange FrequencyRange  READ getFrequencyRangeLimit WRITE setFrequencyRangeLimit)
		Q_PROPERTY(QBoundedDouble SmoothFactorRange  READ getSmoothRangeFactorLimit WRITE setSmoothRangeFactorLimit)
		Q_PROPERTY(QBoundedDouble SmoothFactorRise  READ getSmoothFactorRiseLimit WRITE setSmoothFactorRiseLimit)
		Q_PROPERTY(QBoundedDouble SmoothFactorFall  READ getSmoothFactorFallLimit WRITE setSmoothFactorFallLimit)
public:
	QSpectrumComponent();
	void updatePrePass(QRhiCommandBuffer* cmdBuffer) override;

	QBoundedInt getChannelIndexLimit();
	void setChannelIndexLimit(QBoundedInt var);

	QBoundedInt getBarCountLimit();
	void setBarCountLimit(QBoundedInt var);

	QRange getFrequencyRangeLimit();
	void setFrequencyRangeLimit(QRange var);

	QBoundedDouble getSmoothRangeFactorLimit();
	void setSmoothRangeFactorLimit(QBoundedDouble var);

	QBoundedDouble getSmoothFactorRiseLimit();
	void setSmoothFactorRiseLimit(QBoundedDouble var);

	QBoundedDouble getSmoothFactorFallLimit();
	void setSmoothFactorFallLimit(QBoundedDouble var);

	float getSpacing() const { return mSpacing; }
	void setSpacing(float val) { mSpacing = val; }

	float getMaxHeight() const { return mMaxHeight; }
	void setMaxHeight(float val) { mMaxHeight = val; }

private:
	float mMaxHeight = 100;
	float mSpacing = 1;

};

#endif // QSpectrumComponent_h__