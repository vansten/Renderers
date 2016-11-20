#pragma once

#include <vector>

#include "Shape.h"
#include "Mesh.h"

#include "Image.h"

#include "Light.h"

namespace raytracer
{
	class Scene
	{
	protected:
		std::vector<Shape*> _shapes;
		std::vector<Material*> _materials;
		std::vector<Light*> _lights;

	public:
		Scene();
		~Scene();

		void Init();
		void Shutdown();

		inline const std::vector<Shape*> GetShapes() const
		{
			std::vector<Shape*> shapes;
			auto shapesIt = _shapes.begin();
			auto shapesEnd = _shapes.end();
			for(shapesIt; shapesIt != shapesEnd; ++shapesIt)
			{
				shapes.push_back((*shapesIt));
			}

			return shapes;
		}

		inline const std::vector<Light*> GetLights() const
		{
			return _lights;
		}
	};
}