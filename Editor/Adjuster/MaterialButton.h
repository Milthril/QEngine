#ifndef MaterialButton_h__
#define MaterialButton_h__

#include"Adjuster/Adjuster.h"

class QMaterial;

class MaterialButton :public Adjuster {
public:
	MaterialButton(std::shared_ptr<QMaterial> material);

	QVariant getValue() override;
	void setValue(QVariant var) override;

protected:
private:
};

#endif // MaterialButton_h__
