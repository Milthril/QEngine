#ifndef DropInterface_h__
#define DropInterface_h__

#include "QList"
#include "QClipboard"

class DropInterface {
public:
	DropInterface();
	~DropInterface();
	static void startDrag(const QMimeData *data) {
		if (!data)
			return;
		for (auto drop : mDropList) {
			if (drop->isVaild()) {
				drop->mCanDrop = drop->verifyCanDrop(data);
				if (drop->mCanDrop) {
					drop->updateDropState();
				}
			}
		}
	}
	static void endDrag() {
		for (auto drop : mDropList) {
			if (drop->mCanDrop) {
				drop->mCanDrop = false;
				drop->updateDropState();
			}
		}
	}
	bool canDrop() {
		return mCanDrop;
	}
private:
	virtual bool isVaild() = 0;
	virtual bool verifyCanDrop(const QMimeData* data) = 0;
	virtual void updateDropState() = 0;
private:
	inline static QList<DropInterface*> mDropList;
	bool mCanDrop = false;
};

#endif // DropInterface_h__
