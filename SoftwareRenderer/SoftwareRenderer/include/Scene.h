#pragma once

#include <vector>

class DeviceContext;
class Shape;

class Scene
{
protected:
	std::vector<Shape*> _objects;

public:
	Scene();
	~Scene();

	bool Init();
	void Shutdown();
	void Update(float deltaTime);
	void Draw(DeviceContext* deviceContext);
};

