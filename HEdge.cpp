#include "HEdge.hpp"

HEdge::HEdge(Vertex *v0, Vertex *v1, unsigned int identifier)
{
	id = identifier;
	vertices[0] = v0;
	vertices[1] = v1;
	otherEdge = NULL;
}

HEdge::~HEdge()
{
}

unsigned int HEdge::getId()
{
	return id;
}

void HEdge::setLink(HEdge *edge)
{
	otherEdge = edge;
}

bool HEdge::matchesEdge(HEdge *otherEdge)
{
	if (otherEdge->getVertex(0) == vertices[0] && otherEdge->getVertex(1) == vertices[1])
	{
		return true;
	}
	else if (otherEdge->getVertex(0) == vertices[1] && otherEdge->getVertex(1) == vertices[0])
	{
		return true;
	}
	return false;
}

Vertex* HEdge::getVertex(unsigned int index)
{
	return vertices[index];
}

HEdge* HEdge::getOtherEdge()
{
	return otherEdge;
}