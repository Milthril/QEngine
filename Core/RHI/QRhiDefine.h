#ifndef QRhiInclude_h__
#define QRhiInclude_h__

#include <memory>
#include "private/qrhi_p.h"
#include "RHI\QRhiSignal.h"

template<typename _Ty>
class QRhiSPtr :public std::shared_ptr<_Ty> {
public:
	void reset(_Ty* res) noexcept {
		if (res != nullptr) {
			std::shared_ptr<_Ty>::reset(res, [](_Ty* res) {
				res->destroy();
			});
		}
		else {
			std::shared_ptr<_Ty>::reset();
		}
	}
};

#endif // QRhiInclude_h__
