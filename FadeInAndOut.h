#pragma once

class FadeInAndOut
{
private:
	float alpha;

	Callback_Function_Parameter _cbFP;

	bool isStart;
	bool isFadeInEnd;
	bool isFadeOutEnd;

private:
	static void FadeIn(void* obj);
	static void FadeOut(void* obj);

public:
	FadeInAndOut()
		: alpha(0.f), isStart(false), isFadeInEnd(false), isFadeOutEnd(false) { }

	void Update();
	void Render();

	// �����
	void FadeInStart();

	// ��ο���
	void FadeOutStart();

	bool GetIsFadeInEnd();
	bool GetIsFadeOutEnd();

	void SetAlpha(float value) { alpha = value; }
	float GetAlpha() { return alpha; }

	void SetIsStart(bool start) { isStart = start; }
	void SetIsFadeInEnd(bool fadeInEnd) { isFadeInEnd = fadeInEnd; }
	void SetIsFadeOutEnd(bool fadeOutEnd) { isFadeOutEnd = fadeOutEnd; }
};

