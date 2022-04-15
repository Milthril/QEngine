#ifndef AssetBox_h__
#define AssetBox_h__

#include "Adjuster/Adjuster.h"
#include "Asset/IAsset.h"
#include "Widgets/Buttons/ImageButton.h"
#include "QLineEdit"

class AssetBox : public Adjuster {
	Q_OBJECT
public:
	AssetBox(std::shared_ptr<IAsset> asset, IAsset::Type type, QWidget* parent = nullptr);

	QVariant getValue() override;
	void setValue(QVariant var) override;

	virtual void dragEnterEvent(QDragEnterEvent* event) override;
	virtual void dropEvent(QDropEvent* event) override;
	virtual void mouseDoubleClickEvent(QMouseEvent* event) override;
protected:
	IAsset::Type mType;
	std::shared_ptr<IAsset> mDefaultAsset;
	std::shared_ptr<IAsset> mCurrentAsset;
	QLineEdit mName;
	ImageButton btOpenFile;
	ImageButton btReset;
};

#endif // AssetBox_h__

