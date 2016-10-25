#pragma once

#include <vector>

#include "Shape.h"
#include "Mesh.h"

#include "Image.h"

namespace raytracer
{
	class Scene
	{
	protected:
		std::vector<Shape*> _shapes;
		std::vector<Mesh*> _meshes;

	public:
		Scene();
		~Scene();

		void Init();
		void Shutdown();

		void Render(Image* _image) const;

		inline const std::vector<Shape*> GetShapes() const
		{
			std::vector<Shape*> shapes;
			auto shapesIt = _shapes.begin();
			auto shapesEnd = _shapes.end();
			for(shapesIt; shapesIt != shapesEnd; ++shapesIt)
			{
				shapes.push_back((*shapesIt));
			}

			auto meshesIt = _meshes.begin();
			auto meshesEnd = _meshes.end();
			for(meshesIt; meshesIt != meshesEnd; ++meshesIt)
			{
				std::vector<Triangle*> triangles = (*meshesIt)->GetTriangles();
				auto trianglesIt = triangles.begin();
				auto trianglesEnd = triangles.end();
				for(trianglesIt; trianglesIt != trianglesEnd; ++trianglesIt)
				{
					shapes.push_back((*trianglesIt));
				}
			}

			return shapes;
		}

		inline const std::vector<Mesh*> GetMeshes() const
		{
			return _meshes;
		}
	};
}