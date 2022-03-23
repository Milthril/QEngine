#ifndef QTickEventHandler_h__
#define QTickEventHandler_h__

#include"QList"

class QTickEventHandler {
public:
	QTickEventHandler();
	virtual	~QTickEventHandler();
	static void processEvent();
protected:
	virtual void tickEvent() = 0;
private:
	inline static QList<QTickEventHandler*> mAllHandler;
};

#endif // QTickEventHandler_h__
