#include "QText2DComponent.h"

QString QText2DComponent::getText() const
{
	return text;
}

void QText2DComponent::setText(QString val)
{
	text = val;
	hasNewText = true;
}