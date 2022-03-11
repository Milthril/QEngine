#ifndef QCombo_h__
#define QCombo_h__

#include <QStringList>
#include <QMetaType>
#include "QDebug"

class QCombo {
public:
	QCombo();
	QCombo(int currentIndex, QStringList items);
	QCombo(QString currentItem, QStringList items);
	void clearItem();
	void addItem(QString item);
	void setComboList(QStringList list);
	bool operator==(const QCombo& other);
	friend QDebug operator<<(QDebug debug, const QCombo& range);
	friend QDataStream& operator<<(QDataStream& out, const QCombo& var);
	friend QDataStream& operator>>(QDataStream& in, QCombo& var);
public:
	int getCurrentIndex() const;
	void setCurrentIndex(int val);
	QString getCurrentItem() const;
	void setCurrentItem(QString str);
	QStringList items_ = {};
private:
	int currentIndex_ = 0;
};
Q_DECLARE_METATYPE(QCombo);

#endif // QCombo_h__