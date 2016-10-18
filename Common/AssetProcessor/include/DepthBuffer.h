#pragma once
class DepthBuffer
{
	friend class DeviceContext;

protected:
	float* _depth;
	int _width;
	int _height;

public:
	DepthBuffer(int width, int height);
	~DepthBuffer();

	void Clear();
	float GetDepth(int x, int y);
	void SetDepth(int x, int y, float value);
};

