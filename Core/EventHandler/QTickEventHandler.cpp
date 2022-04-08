#include "QTickEventHandler.h"
#include <QDebug>
QTickEventHandler::QTickEventHandler()
{
	mAllHandler << this;
}

QTickEventHandler::~QTickEventHandler()
{
	mAllHandler.removeOne(this);
}

void QTickEventHandler::processEvent(float deltaSecond)
{
	for (auto& handler : mAllHandler)
		handler->tickEvent(deltaSecond);
}