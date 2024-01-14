#include "pch.h"
#include "GeometryGenerator.h"
#include "StaticMeshResource.h"

void GeometryGenerator::MakeSquare(GeometryData& out,const float scale /*= 1.0f*/, const Vector2 texScale /*= Vector2(1.0f)*/)
{	
	vector<Vector3> positions;
	vector<Vector3> normals;
	vector<Vector2> texcoords; 

	// 앞면
	positions.push_back(Vector3(-1.0f, 1.0f, 0.0f) * scale);
	positions.push_back(Vector3(1.0f, 1.0f, 0.0f) * scale);
	positions.push_back(Vector3(1.0f, -1.0f, 0.0f) * scale);
	positions.push_back(Vector3(-1.0f, -1.0f, 0.0f) * scale);
	normals.push_back(Vector3(0.0f, 0.0f, -1.0f));
	normals.push_back(Vector3(0.0f, 0.0f, -1.0f));
	normals.push_back(Vector3(0.0f, 0.0f, -1.0f));
	normals.push_back(Vector3(0.0f, 0.0f, -1.0f));

	// Texture Coordinates (Direct3D 9)
	// https://learn.microsoft.com/en-us/windows/win32/direct3d9/texture-coordinates
	texcoords.push_back(Vector2(0.0f, 0.0f));
	texcoords.push_back(Vector2(1.0f, 0.0f));
	texcoords.push_back(Vector2(1.0f, 1.0f));
	texcoords.push_back(Vector2(0.0f, 1.0f));
	
	for (size_t i = 0; i < positions.size(); i++) {
		Vertex v;
		v.Position = positions[i];
		v.Normal = normals[i];
		v.TexCoord = texcoords[i] * texScale;
		v.Tangent = Vector3(1.0f, 0.0f, 0.0f);
		out.vertices.push_back(v);
	}
	out.indices = {
		Face(0, 1, 2),Face(0, 2, 3) // 앞면
	};
}
