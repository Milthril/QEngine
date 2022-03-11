#ifndef QRhiInclude_h__
#define QRhiInclude_h__

#include <memory>
#include "private/qrhi_p.h"
#include "Render\RHI\QRhiSignal.h"

template<typename _Ty>
class QRhiSPtr :public std::shared_ptr<_Ty> {
public:
	void reset(_Ty* res) noexcept {
		std::shared_ptr<_Ty>::reset(res, [](_Ty* res) {
			res->destroy();
		});
	}
};

#endif // QRhiInclude_h__
