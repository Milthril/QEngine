#include "TimeDomainProvider.h"
#include "SpectrumMgr.h"

TimeDomainProvider::TimeDomainProvider()
{
	SpectrumMgr::instacne()->addTimeDomainProvider(this);
}

TimeDomainProvider::~TimeDomainProvider()
{
	SpectrumMgr::instacne()->removeTimeDomainProvider(this);
}