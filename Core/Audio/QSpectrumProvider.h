#ifndef QSpectrumProvider_h__
#define QSpectrumProvider_h__

#include "QObject"
#include "SpectrumProvider.h"
#include "ExtType\QBoundedInt.h"
#include "ExtType\QBoundedDouble.h"
#include "ExtType\QRange.h"

class QSpectrumProvider :public QObject, public SpectrumProvider {
	Q_OBJECT
	Q_PROPERTY(QBoundedInt ChannelIndex READ getChannelIndexLimit WRITE setChannelIndexLimit)
	Q_PROPERTY(QBoundedInt BarCount  READ getBarCountLimit WRITE setBarCountLimit)
	Q_PROPERTY(QRange FrequencyRange  READ getFrequencyRangeLimit WRITE setFrequencyRangeLimit)
	Q_PROPERTY(QBoundedDouble SmoothFactorRange  READ getSmoothRangeFactorLimit WRITE setSmoothRangeFactorLimit)
	Q_PROPERTY(QBoundedDouble SmoothFactorRise  READ getSmoothFactorRiseLimit WRITE setSmoothFactorRiseLimit)
	Q_PROPERTY(QBoundedDouble SmoothFactorFall  READ getSmoothFactorFallLimit WRITE setSmoothFactorFallLimit)
public:
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

};


#endif // QSpectrumProvider_h__
