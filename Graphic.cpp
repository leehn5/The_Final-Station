#include "stdafx.h"
#include "Graphic.h"

ID2D1HwndRenderTarget* Graphic::_RT = nullptr;

void Graphic::SetRendertarget()
{
	assert(_RT == nullptr);
	_RT = GRAPHICMANAGER->GetRenderTarget();
}

HRESULT Graphic::Init(ID2D1Bitmap * bitmap, string key, wstring path, int maxFrameX, int maxFrameY)
{
	if (_graphicInfo != nullptr) Release();

	_graphicInfo = new GRAPHIC_INFO;
	_graphicInfo->bitmap = bitmap;

	_graphicInfo->imgKey = key;
	_graphicInfo->imgPath = path;

	_graphicInfo->size.x = (int)_graphicInfo->bitmap->GetPixelSize().width;
	_graphicInfo->size.y = (int)_graphicInfo->bitmap->GetPixelSize().height;

	_graphicInfo->scale = Vector2(1.0f, 1.0f);
	_graphicInfo->alpha = 1.0f;
	_graphicInfo->angle = 0.0f;
	_graphicInfo->frameWidth = _graphicInfo->size.x / maxFrameX;
	_graphicInfo->frameHeight = _graphicInfo->size.y / maxFrameY;
	_graphicInfo->maxFrameX = maxFrameX;
	_graphicInfo->maxFrameY = maxFrameY;
	_graphicInfo->curFrameX = 0;
	_graphicInfo->curFrameY = 0;

	if (_graphicInfo->bitmap == nullptr)
	{
		Release();
		return E_FAIL;
	}

	return S_OK;
}

void Graphic::Release()
{
	if (_graphicInfo)
	{
		_graphicInfo->bitmap->Release();
		_graphicInfo->bitmap = nullptr;
	}
}

void Graphic::Render(float x, float y, PIVOT pivot)
{
	_graphicInfo->size.x *= _graphicInfo->scale.x;
	_graphicInfo->size.y *= _graphicInfo->scale.y;

	Matrix3x2F rotation = Matrix3x2F::Rotation(_graphicInfo->angle, Point2F());
	Matrix3x2F trans = Matrix3x2F::Translation(x, y);

	D2D1_RECT_F dxArea;

	switch (pivot)
	{
	case LEFT_TOP:
		dxArea = RectF(0, 0, _graphicInfo->size.x, _graphicInfo->size.y);
		break;
	case CENTER:
		dxArea = RectF(-_graphicInfo->size.x / 2, -_graphicInfo->size.y / 2, _graphicInfo->size.x / 2, _graphicInfo->size.y / 2);
		break;
	case TOP:
		dxArea = RectF(-_graphicInfo->size.x / 2, 0, _graphicInfo->size.x / 2, _graphicInfo->size.y);
		break;
	case BOTTOM:
		dxArea = RectF(-_graphicInfo->size.x / 2, -_graphicInfo->size.y, _graphicInfo->size.x / 2, 0);
		break;
	case RIGHT_BOTTOM:
		dxArea = RectF(-_graphicInfo->size.x, -_graphicInfo->size.y, 0, 0);
		break;
	case LEFT_BOTTOM:
		dxArea = RectF(0, -_graphicInfo->size.y, _graphicInfo->size.x, 0);
		break;
	}

	_RT->SetTransform(Matrix3x2F::Identity() * rotation * trans* CAMERA->GetMatrix());
	if (_graphicInfo->bitmap) _RT->DrawBitmap(_graphicInfo->bitmap, dxArea, _graphicInfo->alpha);
}

void Graphic::Render(Vector2 pos, float alpha, bool flipX, PIVOT pivot, bool cameraAffect)
{
	_graphicInfo->size.x *= _graphicInfo->scale.x;
	_graphicInfo->size.y *= _graphicInfo->scale.y;

	Matrix3x2F scale = Matrix3x2F::Scale(1, 1);
	if (flipX) scale = scale * Matrix3x2F::Scale(-1, 1);
	Matrix3x2F rotation = Matrix3x2F::Rotation(_graphicInfo->angle, Point2F());
	Matrix3x2F trans = Matrix3x2F::Translation(pos.x, pos.y);

	D2D1_RECT_F dxArea;

	switch (pivot)
	{
	case LEFT_TOP:
		dxArea = RectF(0, 0, _graphicInfo->size.x, _graphicInfo->size.y);
		break;
	case RIGHT_TOP:
		dxArea = RectF(-_graphicInfo->size.x, 0, 0, _graphicInfo->size.y);
		break;
	case CENTER:
		dxArea = RectF(-_graphicInfo->size.x / 2, -_graphicInfo->size.y / 2, _graphicInfo->size.x / 2, _graphicInfo->size.y / 2);
		break;
	case TOP:
		dxArea = RectF(-_graphicInfo->size.x / 2, 0, _graphicInfo->size.x / 2, _graphicInfo->size.y);
		break;
	case BOTTOM:
		dxArea = RectF(-_graphicInfo->size.x / 2, -_graphicInfo->size.y, _graphicInfo->size.x / 2, 0);
		break;
	case RIGHT_BOTTOM:
		dxArea = RectF(-_graphicInfo->size.x, -_graphicInfo->size.y, 0, 0);
		break;
	case LEFT_BOTTOM:
		dxArea = RectF(0, -_graphicInfo->size.y, _graphicInfo->size.x, 0);
		break;
	}

	_RT->SetTransform(scale * rotation * trans);
	if (cameraAffect) _RT->SetTransform(scale * rotation * trans * CAMERA->GetMatrix());

	if (_graphicInfo->bitmap) _RT->DrawBitmap(_graphicInfo->bitmap, dxArea, alpha);
}

void Graphic::Render(Vector2 pos, Vector2 scale, float angle, Vector2 angleAnchor, bool flipX, float alpha, PIVOT pivot, bool cameraAffect)
{
	//Matrix3x2F scale_ = Matrix3x2F::Scale(1, 1);
	Matrix3x2F scale_ = Matrix3x2F::Scale(scale.x, scale.y);

	// 20200117 TODO : Flip부터 잡아보자
	if (flipX) scale_ = scale_ * Matrix3x2F::Scale(-1, 1);

	Matrix3x2F rotation = Matrix3x2F::Rotation(angle, Point2F(angleAnchor.x, angleAnchor.y));
	Matrix3x2F trans = Matrix3x2F::Translation(pos.x, pos.y);

	D2D1_RECT_F dxArea;

	switch (pivot)
	{
	case LEFT_TOP:
		dxArea = RectF(0, 0, _graphicInfo->size.x, _graphicInfo->size.y);
		break;
	case CENTER:
		dxArea = RectF(-_graphicInfo->size.x / 2, -_graphicInfo->size.y / 2, _graphicInfo->size.x / 2, _graphicInfo->size.y / 2);
		break;
	case TOP:
		dxArea = RectF(-_graphicInfo->size.x / 2, 0, _graphicInfo->size.x / 2, _graphicInfo->size.y);
		break;
	case BOTTOM:
		dxArea = RectF(-_graphicInfo->size.x / 2, -_graphicInfo->size.y, _graphicInfo->size.x / 2, 0);
		break;
	case RIGHT_BOTTOM:
		dxArea = RectF(-_graphicInfo->size.x, -_graphicInfo->size.y, 0, 0);
		break;
	case LEFT_BOTTOM:
		dxArea = RectF(0, -_graphicInfo->size.y, _graphicInfo->size.x, 0);
		break;
	}

	_RT->SetTransform(scale_ * rotation * trans);
	if (cameraAffect) _RT->SetTransform(scale_ * rotation * trans * CAMERA->GetMatrix());
	if (_graphicInfo->bitmap) _RT->DrawBitmap(_graphicInfo->bitmap, dxArea, alpha);
}

void Graphic::Render(Vector2 dest, Vector2 sour, Vector2 srcSize, Vector2 scale, PIVOT pivot, float alpha, float angle, bool cameraAffect)
{
	Matrix3x2F scale_ = Matrix3x2F::Scale(scale.x, scale.y);
	//if (flipX) scale_ = scale_ * Matrix3x2F::Scale(-1, 1);

	Matrix3x2F rotation = Matrix3x2F::Rotation(angle, Point2F());
	Matrix3x2F trans = Matrix3x2F::Translation(dest.x, dest.y);

	D2D1_RECT_F dxArea;

	switch (pivot)
	{
	case LEFT_TOP:
		dxArea = RectF(0, 0, srcSize.x, srcSize.y);
		break;
	case CENTER:
		dxArea = RectF(-srcSize.x / 2, -srcSize.y / 2, srcSize.x / 2, srcSize.y / 2);
		break;
	case TOP:
		dxArea = RectF(-srcSize.x / 2, 0, srcSize.x / 2, srcSize.y);
		break;
	case BOTTOM:
		dxArea = RectF(-srcSize.x / 2, -srcSize.y, srcSize.x / 2, 0);
		break;
	case RIGHT_BOTTOM:
		dxArea = RectF(-srcSize.x, -srcSize.y, 0, 0);
		break;
	case LEFT_BOTTOM:
		dxArea = RectF(0, -srcSize.y, srcSize.x, 0);
		break;
	}

	D2D1_RECT_F dxSrc = RectF(sour.x, sour.y, srcSize.x, srcSize.y);

	_RT->SetTransform(scale_ * rotation * trans);
	if (cameraAffect) _RT->SetTransform(scale_ * rotation * trans * CAMERA->GetMatrix());
	if (_graphicInfo->bitmap) _RT->DrawBitmap(_graphicInfo->bitmap, dxArea, alpha, D2D1_BITMAP_INTERPOLATION_MODE_LINEAR, dxSrc);
}

void Graphic::FrameRender(float x, float y, int curFrameX, int curFrameY, PIVOT pivot)
{
	_graphicInfo->curFrameX = curFrameX;
	_graphicInfo->curFrameY = curFrameY;


	if (_graphicInfo->curFrameX > _graphicInfo->maxFrameX - 1) _graphicInfo->curFrameX = _graphicInfo->maxFrameX - 1;
	if (_graphicInfo->curFrameY > _graphicInfo->maxFrameY - 1) _graphicInfo->curFrameY = _graphicInfo->maxFrameY - 1;

	int frame = _graphicInfo->curFrameY * _graphicInfo->maxFrameX + _graphicInfo->curFrameX;

	_graphicInfo->size = GetFrameSize(frame);

	_graphicInfo->size.x *= _graphicInfo->scale.x;
	_graphicInfo->size.y *= _graphicInfo->scale.y;

	Matrix3x2F rotation = Matrix3x2F::Rotation(_graphicInfo->angle, Point2F());
	Matrix3x2F trans = Matrix3x2F::Translation(x, y);

	D2D1_RECT_F dxArea;

	switch (pivot)
	{
	case LEFT_TOP:
		dxArea = RectF(0, 0, _graphicInfo->size.x, _graphicInfo->size.y);
		break;
	case CENTER:
		dxArea = RectF(-_graphicInfo->size.x / 2, -_graphicInfo->size.y / 2, _graphicInfo->size.x / 2, _graphicInfo->size.y / 2);
		break;
	case TOP:
		dxArea = RectF(-_graphicInfo->size.x / 2, 0, _graphicInfo->size.x / 2, _graphicInfo->size.y);
		break;
	case BOTTOM:
		dxArea = RectF(-_graphicInfo->size.x / 2, -_graphicInfo->size.y, _graphicInfo->size.x / 2, 0);
		break;
	case RIGHT_BOTTOM:
		dxArea = RectF(-_graphicInfo->size.x, -_graphicInfo->size.y, 0, 0);
		break;
	case LEFT_BOTTOM:
		dxArea = RectF(0, -_graphicInfo->size.y, _graphicInfo->size.x, 0);
		break;
	}


	D2D1_RECT_F dxSrc = RectF(_graphicInfo->curFrameX * _graphicInfo->frameWidth, _graphicInfo->curFrameY * _graphicInfo->frameHeight,
		_graphicInfo->curFrameX * _graphicInfo->frameWidth + _graphicInfo->frameWidth,
		_graphicInfo->curFrameY * _graphicInfo->frameHeight + _graphicInfo->frameHeight);

	_RT->SetTransform(Matrix3x2F::Identity() * rotation * trans* CAMERA->GetMatrix());

	if (_graphicInfo->bitmap) _RT->DrawBitmap(_graphicInfo->bitmap, dxArea, _graphicInfo->alpha, D2D1_BITMAP_INTERPOLATION_MODE_LINEAR, dxSrc);
}

void Graphic::FrameRender(Vector2 pos, int curFrameX, int curFrameY, float alpha, PIVOT pivot, bool cameraAffect)
{
	_graphicInfo->curFrameX = curFrameX;
	_graphicInfo->curFrameY = curFrameY;

	if (_graphicInfo->curFrameX > _graphicInfo->maxFrameX - 1) _graphicInfo->curFrameX = _graphicInfo->maxFrameX - 1;
	if (_graphicInfo->curFrameY > _graphicInfo->maxFrameY - 1) _graphicInfo->curFrameY = _graphicInfo->maxFrameY - 1;

	int frame = _graphicInfo->curFrameY * _graphicInfo->maxFrameX + _graphicInfo->curFrameX;

	_graphicInfo->size = GetFrameSize(frame);

	_graphicInfo->size.x *= _graphicInfo->scale.x;
	_graphicInfo->size.y *= _graphicInfo->scale.y;

	Matrix3x2F scale;
	scale = Matrix3x2F::Scale(1, 1);
	if (_graphicInfo->isFlipX) scale = Matrix3x2F::Scale(-1, 1);
	Matrix3x2F rotation = Matrix3x2F::Rotation(_graphicInfo->angle, Point2F());
	Matrix3x2F trans = Matrix3x2F::Translation(pos.x, pos.y);

	D2D1_RECT_F dxArea;

	switch (pivot)
	{
	case LEFT_TOP:
		dxArea = RectF(0, 0, _graphicInfo->size.x, _graphicInfo->size.y);
		break;
	case CENTER:
		dxArea = RectF(-_graphicInfo->size.x / 2, -_graphicInfo->size.y / 2, _graphicInfo->size.x / 2, _graphicInfo->size.y / 2);
		break;
	case TOP:
		dxArea = RectF(-_graphicInfo->size.x / 2, 0, _graphicInfo->size.x / 2, _graphicInfo->size.y);
		break;
	case BOTTOM:
		dxArea = RectF(-_graphicInfo->size.x / 2, -_graphicInfo->size.y, _graphicInfo->size.x / 2, 0);
		break;
	case RIGHT_BOTTOM:
		dxArea = RectF(-_graphicInfo->size.x, -_graphicInfo->size.y, 0, 0);
		break;
	case LEFT_BOTTOM:
		dxArea = RectF(0, -_graphicInfo->size.y, _graphicInfo->size.x, 0);
		break;
	}

	D2D1_RECT_F dxSrc = RectF(_graphicInfo->curFrameX * _graphicInfo->frameWidth, _graphicInfo->curFrameY * _graphicInfo->frameHeight,
		_graphicInfo->curFrameX * _graphicInfo->frameWidth + _graphicInfo->frameWidth,
		_graphicInfo->curFrameY * _graphicInfo->frameHeight + _graphicInfo->frameHeight);

	//D2D1_MATRIX_3X2_F cameraMatrix;
	//cameraMatrix = Matrix3x2F::Scale(D2D1::SizeF(1, 1));
	//cameraMatrix = cameraMatrix * Matrix3x2F::Rotation(0);
	//cameraMatrix = cameraMatrix * Matrix3x2F::Translation(100, 100);

	//Matrix3x2F::in // 역행렬

	//_RT->SetTransform(Matrix3x2F::Identity() * rotation * trans * cameraMatrix);

	//_RT->SetTransform(Matrix3x2F::Identity() * rotation * trans * CAMERA->GetMatrix());
	_RT->SetTransform(scale * rotation * trans);
	if (cameraAffect) _RT->SetTransform(scale * rotation * trans * CAMERA->GetMatrix());

	if (_graphicInfo->bitmap) _RT->DrawBitmap(_graphicInfo->bitmap, dxArea, alpha, D2D1_BITMAP_INTERPOLATION_MODE_LINEAR, dxSrc);
}

void Graphic::FrameRender(Vector2 pos, int curFrameX, int curFrameY, Vector2 scale, float angle, Vector2 angleAnchor, bool flipX, float alpha, PIVOT pivot, bool cameraAffect)
{
	_graphicInfo->curFrameX = curFrameX;
	_graphicInfo->curFrameY = curFrameY;

	if (_graphicInfo->curFrameX > _graphicInfo->maxFrameX - 1) _graphicInfo->curFrameX = _graphicInfo->maxFrameX - 1;
	if (_graphicInfo->curFrameY > _graphicInfo->maxFrameY - 1) _graphicInfo->curFrameY = _graphicInfo->maxFrameY - 1;
	Matrix3x2F scale_ = Matrix3x2F::Scale(scale.x, scale.y);
	if (flipX) scale_ = scale_ * Matrix3x2F::Scale(-1, 1);
	Matrix3x2F rotation = Matrix3x2F::Rotation(angle, Point2F(angleAnchor.x, angleAnchor.y));
	Matrix3x2F trans = Matrix3x2F::Translation(pos.x, pos.y);

	D2D1_RECT_F dxArea;

	switch (pivot)
	{
	case LEFT_TOP:
		dxArea = RectF(0, 0, _graphicInfo->frameWidth * scale.x, _graphicInfo->frameHeight * scale.y);
		break;
	case CENTER:
		dxArea = RectF(-_graphicInfo->frameWidth / 2 * scale.x, -_graphicInfo->frameHeight / 2 * scale.y, _graphicInfo->frameWidth / 2 * scale.x, _graphicInfo->frameHeight / 2 * scale.y);
		break;
	case TOP:
		dxArea = RectF(-_graphicInfo->frameWidth / 2 * scale.x, 0, _graphicInfo->frameWidth / 2 * scale.x, _graphicInfo->frameHeight * scale.y);
		break;
	case BOTTOM:
		dxArea = RectF(-_graphicInfo->frameWidth / 2 * scale.x, -_graphicInfo->frameHeight * scale.y, _graphicInfo->frameWidth / 2 * scale.x, 0);
		break;
	case RIGHT_BOTTOM:
		dxArea = RectF(-_graphicInfo->frameWidth * scale.x, -_graphicInfo->frameHeight * scale.y, 0, 0);
		break;
	case LEFT_BOTTOM:
		dxArea = RectF(0, -_graphicInfo->frameHeight * scale.y, _graphicInfo->frameWidth * scale.x, 0);
		break;
	}

	D2D1_RECT_F dxSrc = RectF(_graphicInfo->curFrameX * _graphicInfo->frameWidth, _graphicInfo->curFrameY * _graphicInfo->frameHeight,
		_graphicInfo->curFrameX * _graphicInfo->frameWidth + _graphicInfo->frameWidth,
		_graphicInfo->curFrameY * _graphicInfo->frameHeight + _graphicInfo->frameHeight);

	_RT->SetTransform(scale_ * rotation * trans);
	if (cameraAffect) _RT->SetTransform(scale_ * rotation * trans * CAMERA->GetMatrix());
	_RT->DrawBitmap(_graphicInfo->bitmap, &dxArea, alpha, D2D1_BITMAP_INTERPOLATION_MODE_LINEAR, &dxSrc);
}

void Graphic::LoopRender(const LPRECT drawArea, int offsetX, int offsetY)
{
	if (offsetX < 0) offsetX = _graphicInfo->frameWidth + (offsetX % _graphicInfo->frameWidth);
	if (offsetY < 0) offsetY = _graphicInfo->frameHeight + (offsetY % _graphicInfo->frameHeight);

	D2D1_RECT_F dxSour;
	float sourWidth;
	float sourHeight;

	D2D1_RECT_F dxDest;

	float drawAreaX = drawArea->left;
	float drawAreaY = drawArea->top;
	float drawAreaW = drawArea->right - drawArea->left;
	float drawAreaH = drawArea->bottom - drawArea->top;

	for (int y = 0; y < drawAreaH; y += sourHeight)
	{
		dxSour.top = (y + offsetY) % _graphicInfo->frameHeight;
		dxSour.bottom = _graphicInfo->frameHeight;
		sourHeight = dxSour.bottom - dxSour.top;

		if (y + sourHeight > drawAreaH)
		{
			dxSour.bottom -= (y + sourHeight) - drawAreaH;
			sourHeight = dxSour.bottom - dxSour.top;
		}

		dxDest.top = y + drawAreaY;
		dxDest.bottom = dxDest.top + sourHeight;

		for (int x = 0; x < drawAreaW; x += sourWidth)
		{
			dxSour.left = (x + offsetX) % _graphicInfo->frameWidth;
			dxSour.right = _graphicInfo->frameWidth;
			sourWidth = dxSour.right - dxSour.left;

			if (x + sourWidth > drawAreaW)
			{
				dxSour.right -= (x + sourWidth) - drawAreaW;
				sourWidth = dxSour.right - dxSour.left;
			}

			dxDest.left = x + drawAreaX;
			dxDest.right = dxDest.left + sourWidth;

			Render(Vector2(dxDest.left, dxDest.top), Vector2(dxSour.left, dxSour.top),
				Vector2(dxSour.right - dxSour.left, dxSour.bottom - dxSour.top), Vector2(1, 1), PIVOT::LEFT_TOP);
		}
	}
}

void Graphic::SetSize(Vector2 size)
{
	_graphicInfo->size *= size;

	_graphicInfo->frameWidth = _graphicInfo->size.x / _graphicInfo->maxFrameX;
	_graphicInfo->frameHeight = _graphicInfo->size.y / _graphicInfo->maxFrameY;
}
