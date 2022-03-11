#ifndef QBoundedInt_h__
#define QBoundedInt_h__

#include <QMetaType>

class QBoundedInt {
public:
	QBoundedInt(){}
	QBoundedInt(int number):value_(number) {}
	QBoundedInt(int number, int min, int max);
	QBoundedInt(const QBoundedInt& other);

	void setBound(int min, int max);

	int min() const;

	void setMin(int min);

	int max() const;
	void setMax(int max);

	int number() const;

	bool setNumber(int number);

	bool operator==(const QBoundedInt& other);

	friend QDataStream& operator<<(QDataStream& out, const QBoundedInt& var);
	friend QDataStream& operator>>(QDataStream& in, QBoundedInt& var);

private:
	int value_ = 0;
	int min_ = 0, max_ = 0;
};

Q_DECLARE_METATYPE(QBoundedInt)

#endif // QSliderNumber_h__