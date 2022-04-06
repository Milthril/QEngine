#ifndef QWidgetShadowMaker_h__
#define QWidgetShadowMaker_h__

#include <QGraphicsEffect>

class QWidgetShadowMaker : public QGraphicsEffect
{
public:
	QWidgetShadowMaker(qreal blurRadius = 10, qreal distance = 3, qreal strength = 0.8, qreal angle = 45, bool inset = false);
	~QWidgetShadowMaker();

	qreal strength() const;
	void setStrength(const qreal& strength);

	qreal blurRadius() const;
	void setBlurRadius(const qreal& blurRadius);

	qreal distance() const;
	void setDistance(const qreal& distance);

	qreal angle() const;
	void setAngle(const qreal& angle);

	bool inset() const;
	void setInset(bool inset);

	static QList<QWidgetShadowMaker*> instances;
	static void setEffectEnabled(bool enabled);
protected:
	QRectF boundingRectFor(const QRectF& rect) const override;
	void draw(QPainter* painter) override;
private:
	qreal blurRadius_;
	qreal distance_;
	qreal strength_;
	qreal angle_;
	bool inset_;
	static bool enabled_;
};

#endif // QWidgetShadowMaker_h__
