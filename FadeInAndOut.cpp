#include "stdafx.h"
#include "FadeInAndOut.h"

void FadeInAndOut::Update()
{
	if (isStart) _cbFP(this);
}

void FadeInAndOut::Render()
{
	GRAPHICMANAGER->DrawFillRect(Vector2(WINSIZEX / 2, WINSIZEY / 2), Vector2(WINSIZEX, WINSIZEY), 0.0f, ColorF(ColorF::Black, alpha), PIVOT::CENTER, false);
}

void FadeInAndOut::FadeInStart()
{
	_cbFP = static_cast<Callback_Function_Parameter>(FadeIn);
	isStart = true;
	alpha = 1.f;

	isFadeInEnd = false;
	isFadeOutEnd = false;
}

void FadeInAndOut::FadeIn(void* obj)
{
	FadeInAndOut* in = (FadeInAndOut*)obj;
	in->SetAlpha(in->GetAlpha() - 0.7f * TIMEMANAGER->getElapsedTime());

	if (in->GetAlpha() < 0.f)
	{
		in->SetIsStart(false);
		in->SetIsFadeInEnd(true);
	}
}

bool FadeInAndOut::GetIsFadeInEnd()
{
	if (isFadeInEnd == true) return true;
	return false;
}

void FadeInAndOut::FadeOutStart()
{
	_cbFP = static_cast<Callback_Function_Parameter>(FadeOut);
	isStart = true;
	alpha = 0.f;

	isFadeInEnd = false;
	isFadeOutEnd = false;
}

void FadeInAndOut::FadeOut(void* obj)
{
	FadeInAndOut* out = (FadeInAndOut*)obj;
	out->SetAlpha(out->GetAlpha() + 0.7f * TIMEMANAGER->getElapsedTime());

	if (out->GetAlpha() > 1.f)
	{
		out->SetIsStart(false);
		out->SetIsFadeOutEnd(true);
	}
}

bool FadeInAndOut::GetIsFadeOutEnd()
{
	if (isFadeOutEnd == true) return true;
	return false;
}