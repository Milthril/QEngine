#ifndef QBoundedDouble_h__
#define QBoundedDouble_h__

#include <QMetaType>

class QBoundedDouble {
public:
	QBoundedDouble() {};
	QBoundedDouble(double number):value_(number) {};
	QBoundedDouble(double number, double min, double max);
	QBoundedDouble(const QBoundedDouble& other);
	void setBound(double min, double max);

	double min() const;

	void setMin(double min);

	double max() const;
	void setMax(double max);

	double number() const;

	bool setNumber(double number);

	bool operator==(const QBoundedDouble& other);

	friend QDataStream& operator<<(QDataStream& out, const QBoundedDouble& var);
	friend QDataStream& operator>>(QDataStream& in, QBoundedDouble& var);

private:
	double value_ = 0;
	double min_ = 0, max_ = 100;
};
Q_DECLARE_METATYPE(QBoundedDouble)

#endif // QSliderNumber_h__