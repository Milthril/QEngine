#ifndef AssetBox_h__
#define AssetBox_h__

#include "Adjuster/Adjuster.h"
#include "Asset/IAsset.h"
#include "Widgets/Buttons/ImageButton.h"
#include "QLineEdit"
#include "Toolkit/DropInterface.h"

class AssetBox : public Adjuster, public DropInterface{
	Q_OBJECT
public:
	AssetBox(std::shared_ptr<IAsset> asset, IAsset::Type type, QWidget* parent = nullptr);
	~AssetBox();
	QVariant getValue() override;
	void setValue(QVariant var) override;
	virtual void dragEnterEvent(QDragEnterEvent* event) override;
	virtual void dropEvent(QDropEvent* event) override;
	virtual bool isVaild() override;
	virtual bool verifyCanDrop(const QMimeData* data) override;
	virtual void updateDropState() override;
	virtual void paintEvent(QPaintEvent* event) override;

	virtual void mouseDoubleClickEvent(QMouseEvent* event) override;
protected:
	IAsset::Type mType;
	std::shared_ptr<IAsset> mDefaultAsset;
	std::shared_ptr<IAsset> mCurrentAsset;
	QLineEdit mName;
	ImageButton btOpenFile;
	ImageButton btReset;
	inline static int count = 0;
private:
};

#endif // AssetBox_h__

