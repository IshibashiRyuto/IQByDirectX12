#include "PrimitiveData.h"

Primitive::Vertex::Vertex()
	: pos(Math::Vector3())
	, normal(Math::Vector3(0.f,1.f,0.f))
	, uv(Math::Vector2())
{
}

Primitive::Vertex::Vertex(const Math::Vector3 & _pos, const Math::Vector3 & _normal, const Math::Vector2 & _uv)
	: pos(_pos)
	, normal(_normal)
	, uv(_uv)
{
}

Primitive::Material::Material()
	: diffuse(Math::Vector4(0.8f, 0.8f, 0.8f, 1.f))
	, specular(Math::Vector3(1.f, 1.f, 1.f))
	, ambient(Math::Vector3(0.2f, 0.2f, 0.2f))
	, specularity(1.f)
{
}

Primitive::Material::Material(const Math::Vector4 & _diffuse, const Math::Vector3 & _ambient, const Math::Vector3 & _specular, float _specularity)
	: diffuse(_diffuse)
	, ambient(_ambient)
	, specular(_specular)
	, specularity(_specularity)
{
}
