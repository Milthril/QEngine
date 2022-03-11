#include "ByteArrayLoader.h"
#include <QBoxLayout>
#include <QFile>
#include <QFileDialog>
#include <QPushButton>
#include "Effect/QNeumorphism.h"

ByteArrayLoader::ByteArrayLoader(QByteArray value, QWidget* parent /*= nullptr*/)
	: btLoad_(new QPushButton("Load"))
{
	setValue(value);
	btLoad_->setGraphicsEffect(new QNeumorphism);
	QHBoxLayout* h = new QHBoxLayout(this);
	h->setContentsMargins(0, 0, 0, 0);
	h->addWidget(btLoad_);
	connect(btLoad_, &QPushButton::clicked, this, &ByteArrayLoader::loadFile);
}

QVariant ByteArrayLoader::getValue()
{
	return data_;
}

void ByteArrayLoader::setValue(QVariant var)
{
	data_ = var.toByteArray();
	btLoad_->setToolTip(data_.size() > 1024 ? data_.mid(0, 1024) + "..." : data_);
	Q_EMIT valueChanged(data_);
}

void ByteArrayLoader::loadFile()
{
	QString filePath = QFileDialog::getOpenFileName(nullptr, "Load File");
	if (!QFile::exists(filePath))
		return;
	QFile file(filePath);
	if (file.open(QFile::ReadOnly)) {
		setValue(file.readAll());
		file.close();
	}
}