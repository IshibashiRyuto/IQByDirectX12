#include "PrimitivePlane.h"

using namespace Math;


PrimitivePlane::PrimitivePlane(std::shared_ptr<Device> device,
	const std::vector<Primitive::Vertex>& vertices,
	const std::vector<short>& indexes,
	const Primitive::Material & material,
	std::shared_ptr<PipelineStateObject> pipelineState,
	std::shared_ptr<PipelineStateObject> shadowPSO,
	std::shared_ptr<RootSignature> rootSignature)
	: PrimitiveModelData( device, vertices, indexes, material, pipelineState, shadowPSO, rootSignature)
{
}

PrimitivePlane::~PrimitivePlane()
{
}

std::shared_ptr<PrimitivePlane> PrimitivePlane::Create(std::shared_ptr<Device> device,
	const Math::Vector2 & size,
	const Primitive::Material & material,
	std::shared_ptr<PipelineStateObject> pipelineState,
	std::shared_ptr<PipelineStateObject> shadowPSO,
	std::shared_ptr<RootSignature> rootSignature)
{
	std::vector<Primitive::Vertex> vertices;
	std::vector<Primitive::Index> indexes;
	ConstructVertices(size, vertices, indexes);
	return std::shared_ptr<PrimitivePlane>(new PrimitivePlane(device, vertices, indexes, material, pipelineState, shadowPSO, rootSignature));
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
