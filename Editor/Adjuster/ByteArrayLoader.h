#ifndef ByteArrayLoader_h__
#define ByteArrayLoader_h__

#include "Adjuster.h"
#include <QByteArray>

class QPushButton;

class ByteArrayLoader : public Adjuster
{
	Q_OBJECT
public:
	ByteArrayLoader(QByteArray value, QWidget* parent = nullptr);
	QVariant getValue() override;
	void setValue(QVariant var) override;
protected:
	void loadFile();
	QByteArray data_;
	QPushButton* btLoad_;
};

#endif // ByteArrayLoader_h__
