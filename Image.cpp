#include "stdafx.h"
#include "Image.h"

shared_ptr<Graphic> Image::CreateImage(string imgKey, string path, int maxFrameX, int maxFrameY)
{
	wstring wPath;
	wPath.assign(path.begin(), path.end());

	shared_ptr<Graphic> newImage = shared_ptr<Graphic>(new Graphic);
	newImage->Init(Direct2D::GetInstance()->CreateBitmap(wPath), imgKey, wPath, maxFrameX, maxFrameY);
	return newImage;
}