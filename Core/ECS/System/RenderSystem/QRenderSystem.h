#ifndef QRenderSystem_h__
#define QRenderSystem_h__

#include <QObject>

class QRenderer;
class IRenderableComponent;

class QRenderSystem: public QObject {
public:
	void addRenderItem(IRenderableComponent* comp);
	void removeRenderItem(IRenderableComponent* comp);
private:
	std::shared_ptr<QRenderer> mRenderer;
	QList<IRenderableComponent*> mRenderItemList;
};


#endif // QRenderSystem_h__
