﻿#ifndef IRenderPassBase_h__
#define IRenderPassBase_h__

#include "RHI/QRhiDefine.h"

class IRenderPassBase {
public:
	void setup() {
		if(mFuncSetup)
			mFuncSetup();
	}
	virtual void compile() {}
	virtual void execute(QRhiCommandBuffer* cmdBuffer) {}
	void setFuncSetup(std::function<void()> val) { mFuncSetup = val; }
private:
	std::function<void()> mFuncSetup;
};

#endif // IRenderPassBase_h__