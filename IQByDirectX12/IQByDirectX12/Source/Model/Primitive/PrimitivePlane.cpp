#include "PrimitivePlane.h"

using namespace Math;


PrimitivePlane::PrimitivePlane(std::shared_ptr<Device> device, const std::vector<Primitive::Vertex>& vertices, const std::vector<short>& indexes, const Primitive::Material & material, std::shared_ptr<PipelineStateObject> pipelineState)
	: PrimitiveModelData( device, vertices, indexes, material, pipelineState)
{
}

PrimitivePlane::~PrimitivePlane()
{
}

std::shared_ptr<PrimitivePlane> PrimitivePlane::Create(std::shared_ptr<Device> device, const Math::Vector2 & size, const Primitive::Material & material, std::shared_ptr<PipelineStateObject> pipelineState)
{
	return std::shared_ptr<PrimitivePlane>();
}

void PrimitivePlane::ConstructVertices(const Math::Vector2 & size, std::vector<Primitive::Vertex>& vertices, std::vector<short>& indexes)
{
	Vector3 up(0.f, 1.f, 0.f);
	vertices.clear();
	vertices.resize(4);
	for (int i = 0; i < 4; ++i)
	{
		vertices[i].pos = Vector3(powf(-1, i % 2) * size.x / 2.f, 0.f, powf(-1, i / 2) *  size.y / 2.f);
		vertices[i].normal = up;
		vertices[i].uv = Vector2(static_cast<float>(i % 2), static_cast<float>(i / 2));
	}
	indexes.clear();
	indexes.resize(6);

	indexes[0] = 0;
	indexes[1] = 1;
	indexes[2] = 2;
	indexes[3] = 1;
	indexes[4] = 3;
	indexes[5] = 2;
}
