#pragma once
#include "Direct2D.h"

class Image
{
public:
	static shared_ptr<Graphic> CreateImage(string imgKey, string path, int maxFrameX = 1, int maxFrameY = 1);
};

