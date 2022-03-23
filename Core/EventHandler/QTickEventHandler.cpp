#include "QTickEventHandler.h"

QTickEventHandler::QTickEventHandler()
{
	mAllHandler << this;
}

QTickEventHandler::~QTickEventHandler()
{
	mAllHandler.removeOne(this);
}