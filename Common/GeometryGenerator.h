#pragma once
#include "MeshResource.h"

struct GeometryData 
{
	std::vector<Vertex> vertices;
	std::vector<Face> indices;
};

class GeometryGenerator
{
public:
	static void MakeSquare(GeometryData& out, const float scale = 1.0f,
		const Vector2 texScale = Vector2(1.0f));
};

