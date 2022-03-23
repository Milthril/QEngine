#ifndef QTickEventHandler_h__
#define QTickEventHandler_h__

class QTickEventHandler {
public:
	QTickEventHandler();
	virtual	~QTickEventHandler();
protected:
	virtual void tickEvent() = 0;
private:
	inline static QList<QTickEventHandler*> mAllHandler;
};

#endif // QTickEventHandler_h__
