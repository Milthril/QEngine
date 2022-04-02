#ifndef IPainter_h__
#define IPainter_h__

#include "RHI\QRhiDefine.h"

class IPainter {
public:
	virtual void compile() = 0;
	virtual void paint(QRhiCommandBuffer* cmdBuffer) = 0;
};


#endif // IPainter_h__
