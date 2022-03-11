
#include "QRhiSignal.h"

void QRhiSignal::active()
{
	bSignal = true;
}

bool QRhiSignal::receive()
{
	bool var = bSignal;
	bSignal = false;
	return var;
}
