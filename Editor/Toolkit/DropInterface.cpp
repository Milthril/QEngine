#include "DropInterface.h"

DropInterface::DropInterface() {
	mDropList << this;
}

DropInterface::~DropInterface() {
	mDropList.removeOne(this);
}

