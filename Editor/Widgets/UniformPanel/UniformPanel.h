#ifndef UniformPanel_h__
#define UniformPanel_h__

#include <QWidget>

class QRhiUniform;
class QTreeWidget;
class QPushButton;

class UniformPanel : public QWidget {
public:
	UniformPanel();
	void setUniform(std::shared_ptr<QRhiUniform> uniform);
protected:
	void updateParamPanel();
private:
	std::shared_ptr<QRhiUniform> mUniform;
	QPushButton* btNewParam;
	QTreeWidget* mParamsPanel;
};

#endif // UniformPanel_h__
