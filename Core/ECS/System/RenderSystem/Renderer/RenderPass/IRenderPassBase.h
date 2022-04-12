#ifndef IRenderPassBase_h__
#define IRenderPassBase_h__

#include "ECS/System/RenderSystem/RHI/QRhiDefine.h"

class IRenderPassBase {
public:
	void setup() {
		if(mFuncSetup)
			mFuncSetup();
	}
	virtual void compile() = 0;
	virtual void execute(QRhiCommandBuffer* cmdBuffer) = 0;
	virtual QRhiTexture* getOutputTexture() = 0;

	void setFuncSetup(std::function<void()> val) { mFuncSetup = val; }
private:
	std::function<void()> mFuncSetup;
};

#endif // IRenderPassBase_h__
