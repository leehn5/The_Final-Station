#pragma once
#include"DrawComponent.h"

class Object;

class Sprite : public DrawComponent
{
private:
	bool _isFlipX;
	bool _isFillRect;
	bool _isShowRect;

	bool _isFrame;
	bool _isLoop;
	bool _isPlay;

	int _curFrameX;
	int _curFrameY;

	int _maxFrameX;
	int _maxFrameY;

	float _count;
	float _FPS;
	float _strokeWidth;

	float _alpha;

	string _imgKey;
	Graphic* _graphic;

	ColorF _color = NULL;
	PIVOT _pivot;

	Vector2 _scale;
	Vector2 _pos;
	Vector2 _angleAnchor;

public:
	Sprite();
	virtual void Init(BOOL isFrame = false, BOOL isLoop = false);
	virtual void Render();
	virtual void Update() override;

	inline void SetFlipX(bool value) { _isFlipX = value; }
	inline void SetFrameX(int frameX) { _curFrameX = frameX; }
	inline void SetFrameY(int frameY) { _curFrameY = frameY; }
	inline void SetFPS(float time) { _FPS = 1.0f / time; }
	inline void SetAlpha(float alpha) { _alpha = alpha; }
	inline void SetStrokeWidth(float strokeWidth) { _strokeWidth = strokeWidth; }
	inline void SetFillRect(bool fillRect) { _isFillRect = fillRect; }
	inline void SetShowRect(bool show) { _isShowRect = show; }
	//inline void SetCameraAffect(bool isAffect) { _isCameraAffect = isAffect; }
	inline void SetScale(Vector2(scale)) { _scale = scale; }
	inline void SetPosition(Vector2(pos)) { _pos = pos; }
	inline void SetAngleAnchor(Vector2(pos)) { _angleAnchor = pos; }

	inline string GetImgKey() { return _imgKey; }
	inline float GetAlpha() { return _alpha; }
	inline float GetFPS() { return _FPS; }
	inline bool GetFlipX() { return _isFlipX; }
	inline bool GetFillRect() { return _isFillRect; }
	inline bool GetShowRect() { return _isShowRect; }
	//inline bool GetCameraAffect() { return _isCameraAffect; }
	inline Vector2 GetScale() { return _scale; }
	inline Vector2 GetPosition() { return _pos; }
	inline Vector2 GetAngleAnchor() { return _angleAnchor; }

	inline int GetCurrentFrameX() { return _curFrameX; }
	inline int GetCurrentFrameY() { return _curFrameY; }
	inline int GetMaxFrameX() { return _maxFrameX; }
	inline int GetMaxFrameY() { return _maxFrameY; }

	inline int GetFrameWidth() { return _graphic->GetFrameWidth() * _scale.x; }
	inline int GetFrameHeight() { return _graphic->GetFrameHeight() * _scale.y; }
	void Start();
	void Stop();
	void Pause();
	void Resume();
	void PlayAnimation();

	void SetImgName(string key);
	void SetMaxFrameX(int maxFrameX) { _maxFrameX = maxFrameX - 1; }
	void SetMaxFrameY(int maxFrameY) { _maxFrameY = maxFrameY - 1; }
	inline void SetRectColor(ColorF color) { _color = color; }
	inline void SetPivot(PIVOT pivot) { _pivot = pivot; }
	void SetIsFrame(bool isFrame) { _isFrame = isFrame; }
	void SetIsLoop(bool isLoop) { _isLoop = isLoop; }

	ColorF GetRectColor() { return _color; }

	bool IsFrameEnd();
	inline Graphic* GetGraphic() { return _graphic; }
};