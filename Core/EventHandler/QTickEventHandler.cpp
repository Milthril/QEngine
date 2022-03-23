#include "QTickEventHandler.h"

QTickEventHandler::QTickEventHandler()
{
	mAllHandler << this;
}

QTickEventHandler::~QTickEventHandler()
{
	mAllHandler.removeOne(this);
}

void QTickEventHandler::processEvent()
{
	for (auto& handler : mAllHandler)
		handler->tickEvent();
}