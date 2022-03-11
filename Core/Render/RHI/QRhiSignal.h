#ifndef QRhiSignal_h__
#define QRhiSignal_h__

class QRhiSignal {
public:
	QRhiSignal(bool var = false) :bSignal(var) {};
	void active();
	bool receive();
private:
	bool bSignal = false;
};

#endif // QRhiSignal_h__
