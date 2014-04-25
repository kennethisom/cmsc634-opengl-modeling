#include "Vertex.hpp"
#include "Vec.inl";
#include <stdio.h>

Vertex::Vertex(Vec3f vertex, unsigned int identifier)
{
	id = identifier;
	vert = vertex;
}

Vertex::~Vertex()
{

}

void Vertex::saveNormal(Vec3f normal)
{
	norms.push_back(normal);
}

void Vertex::computeNormal()
{
	norm = vec3<float>(0.f, 0.f, 0.f);
	for (unsigned int i=0; i < norms.size(); i++)
	{
		norm = norm + norms[i];
	}
	norm = normalize(norm);
}

unsigned int Vertex::getId()
{
	return id;
}

Vec3f Vertex::getVertex()
{
	return vert;
}

Vec3f Vertex::getNormal()
{
	return norm;
}