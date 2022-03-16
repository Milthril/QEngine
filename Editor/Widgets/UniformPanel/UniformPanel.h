#ifndef UniformPanel_h__
#define UniformPanel_h__

#include <QWidget>

class QRhiUniform;
class QTreeWidget;

class UniformPanel : public QWidget {
public:
	UniformPanel(std::shared_ptr<QRhiUniform> uniform);
protected:
	void updateParamPanel();
private:
	std::shared_ptr<QRhiUniform> mUniform;
	QTreeWidget* mParamsPanel;
};

#endif // UniformPanel_h__
