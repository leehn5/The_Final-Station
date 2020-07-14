#pragma once

enum PIVOT
{
	LEFT_TOP,
	RIGHT_TOP,
	CENTER,
	BOTTOM,
	RIGHT_BOTTOM,
	LEFT_BOTTOM,
	TOP,
};

class Graphic
{
private:
	static ID2D1HwndRenderTarget* _RT;

public:
	static void SetRendertarget();

public:
	typedef struct tagGraphicInfo
	{
		ID2D1Bitmap*				bitmap;
		Vector2						size;
		Vector2						scale;
		float						angle;
		float						alpha;
		int							frameWidth;
		int							frameHeight;
		int							maxFrameX;
		int							maxFrameY;
		int							curFrameX;
		int							curFrameY;
		string						imgKey;
		wstring						imgPath;
		bool						isFlipX;
		bool						isCameraAffect;

		tagGraphicInfo()
		{
			bitmap = nullptr;
			size = Vector2(1.0f, 1.0f);
			scale = Vector2(1.0f, 1.0f);
			angle = 0.0f;
			alpha = 1.0f;
			frameWidth = 0;
			frameHeight = 0;
			maxFrameX = 0;
			maxFrameY = 0;
			curFrameX = 0;
			curFrameY = 0;
			isFlipX = false;
		}
	}GRAPHIC_INFO, *LPGRAPHIC_INFO;

private:
	LPGRAPHIC_INFO					_graphicInfo;

public:
	Graphic() {};
	~Graphic() {};

	HRESULT Init(ID2D1Bitmap* bitmap, string key, wstring path, int maxFrameX = 1, int maxFrameY = 1);
	void Release();
	void Render(float x, float y, PIVOT pivot = PIVOT::CENTER);
	void Render(Vector2 pos, float alpha = 1.0f, bool flipX = false, PIVOT pivot = PIVOT::CENTER, bool cameraAffect = true);
	void Render(Vector2 pos, Vector2 scale, float angle, Vector2 angleAnchor = Vector2::zero, bool flipX = false, float alpha = 1.0f, PIVOT pivot = PIVOT::CENTER, bool cameraAffect = true);

	// progress bar 에 사용하려고 만든 Render
	void Render(Vector2 dest, Vector2 sour, Vector2 srcSize, Vector2 scale = Vector2(1, 1), PIVOT pivot = PIVOT::CENTER, float alpha = 1.0f, float angle = 0.0f, bool cameraAffect = true);

	void FrameRender(float x, float y, int curFrameX, int curFrameY, PIVOT pivot = PIVOT::CENTER);
	void FrameRender(Vector2 pos, int curFrameX, int curFrameY, float alpha = 1.0f, PIVOT pivot = PIVOT::CENTER, bool cameraAffect = true);
	void FrameRender(Vector2 pos, int curFrameX, int curFrameY, Vector2 scale, float angle, Vector2 angleAnchor = Vector2::zero, bool flipX = false, float alpha = 1.0f, PIVOT pivot = PIVOT::CENTER, bool cameraAffect = true);

	// 수리 중
	void LoopRender(const LPRECT drawArea, int offsetX, int offsetY);

	void SetFlipX(bool isFlip) { _graphicInfo->isFlipX = isFlip; }
	void SetSize(Vector2 size);// { _graphicInfo->size = size; }
	//void SetFrameWidth(float w) { _graphicInfo->frameWidth = w; }
	//void SetFrameHeight(float h) { _graphicInfo->frameHeight = h; }

	void SetAngle(float angle) { _graphicInfo->angle = angle; }
	void SetScale(Vector2 scale) { _graphicInfo->scale = scale; }
	void SetAlpha(float alpha) { _graphicInfo->alpha = alpha; }
	void SetCameraAffect(bool affect) { _graphicInfo->isCameraAffect = affect; }

	void SetCurrentFrameX(int frame) { _graphicInfo->curFrameX = frame; }
	void SetCurrentFrameY(int frame) { _graphicInfo->curFrameY = frame; }

	bool GetFlipX() { return _graphicInfo->isFlipX; }
	/*UINT GetWidth() { return _graphicInfo->size.x; }
	UINT GetHeight() { return _graphicInfo->size.y; }*/
	float GetWidth() { return _graphicInfo->size.x; }
	float GetHeight() { return _graphicInfo->size.y; }
	Vector2 GetFrameSize(int frame) { return Vector2(_graphicInfo->frameWidth, _graphicInfo->frameHeight); }

	int GetMaxFrameX() { return _graphicInfo->maxFrameX - 1; }
	int GetMaxFrameY() { return _graphicInfo->maxFrameY - 1; }

	int GetCurrentFrameX() { return _graphicInfo->curFrameX; }
	int GetCurrentFrameY() { return _graphicInfo->curFrameY; }

	int GetFrameWidth() { return _graphicInfo->frameWidth; }
	int GetFrameHeight() { return _graphicInfo->frameHeight; }

	LPGRAPHIC_INFO GetGraphicInfo() { return _graphicInfo; }

	string GetImageKey() { return _graphicInfo->imgKey; }

	ID2D1Bitmap* GetBitmap() { return _graphicInfo->bitmap; }
};
