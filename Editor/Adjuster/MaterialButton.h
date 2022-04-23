#ifndef MaterialButton_h__
#define MaterialButton_h__

#include"Adjuster/Adjuster.h"
namespace Asset {
class Material;
}
class QComboBox;
class QPushButton;

class MaterialButton :public Adjuster {
public:
	MaterialButton(std::shared_ptr<Asset::Material> material);
	QVariant getValue() override;
	void setValue(QVariant var) override;
protected:
	QComboBox* cbMaterialList;
	QPushButton* btEdit;
	std::shared_ptr<Asset::Material> mMaterial;
};

#endif // MaterialButton_h__
