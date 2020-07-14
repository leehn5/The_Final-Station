#pragma once
class BspNode
{
public:
	Vector2 _startPos;
	Vector2 _endPos;

	BspNode* _left;
	BspNode* _right;

public:
	BspNode* GetLeftNode() { return _left; }
	BspNode* GetRightNode() { return _right; }
};

