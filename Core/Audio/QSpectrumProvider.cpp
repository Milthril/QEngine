#include "QSpectrumProvider.h"

QBoundedInt QSpectrumProvider::getChannelIndexLimit()
{
	QBoundedInt BInt(getChannelIndex(),0,1);
	return BInt;
}

void QSpectrumProvider::setChannelIndexLimit(QBoundedInt var)
{
	setChannelIndex(var.number());
}

QBoundedInt QSpectrumProvider::getBarCountLimit()
{
	QBoundedInt BInt(getBarSize(), 4,1000);
	return BInt;
}

void QSpectrumProvider::setBarCountLimit(QBoundedInt var)
{
	setBarSize(var.number());
}

QRange QSpectrumProvider::getFrequencyRangeLimit()
{
	QRange range(getLowFreq(), getHighFreq(), 0, 48000);
	return range;
}

void QSpectrumProvider::setFrequencyRangeLimit(QRange var)
{
	setLowFreq(var.getLower());
	setHighFreq(var.getUpper());
}

QBoundedDouble QSpectrumProvider::getSmoothRangeFactorLimit()
{
	return QBoundedDouble(getSmoothFactorRange(), 0, 1);
}

void QSpectrumProvider::setSmoothRangeFactorLimit(QBoundedDouble var)
{
	setSmoothFactorRange(var.number());
}

QBoundedDouble QSpectrumProvider::getSmoothFactorRiseLimit()
{
	return QBoundedDouble(getSmoothFactorRise(), 0, 1);
}

void QSpectrumProvider::setSmoothFactorRiseLimit(QBoundedDouble var)
{
	setSmoothFactorRise(var.number());
}

QBoundedDouble QSpectrumProvider::getSmoothFactorFallLimit()
{
	return QBoundedDouble(getSmoothFactorFall(), 0, 1);
}

void QSpectrumProvider::setSmoothFactorFallLimit(QBoundedDouble var)
{
	setSmoothFactorFall(var.number());
}

