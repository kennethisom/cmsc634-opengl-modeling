#include "Face.hpp"
#include "Vec.inl"
#include <stdlib.h>
#include <math.h>

Face::Face(Vertex *v0, Vertex *v1, Vertex *v2)
{
	vertices[0] = v0;
	vertices[1] = v1;
	vertices[2] = v2;
	edges[0] = new HEdge(v0, v1, 0);
	edges[1] = new HEdge(v1, v2, 1);
	edges[2] = new HEdge(v2, v0, 2);
	newVertices[0] = NULL;
	newVertices[1] = NULL;
	newVertices[2] = NULL;
	newNeighborFaces[0][0] = NULL;
	newNeighborFaces[0][1] = NULL;
	newNeighborFaces[1][0] = NULL;
	newNeighborFaces[1][1] = NULL;
	newNeighborFaces[2][0] = NULL;
	newNeighborFaces[2][1] = NULL;
}

Face::~Face()
{
	delete edges[0];
	delete edges[1];
	delete edges[2];
}

void Face::linkTo(Face *otherFace)
{
	HEdge *otherEdge = NULL;
	for (unsigned int i=0; i < 3; i++)
	{
		otherEdge = otherFace->getCompanionEdge(edges[i]);
		if (otherEdge != NULL)
		{
			edges[i]->setLink(otherEdge);
			otherEdge->setLink(edges[i]);
			setNeighbor(otherFace, i);
			otherFace->setNeighbor(this, otherEdge->getId());
			break;
		}
	}
	
}

void Face::setNeighbor(Face *otherFace, unsigned int edgeId)
{
	neighborFaces[edgeId] = otherFace;
}

HEdge* Face::getCompanionEdge(HEdge *otherEdge)
{
	HEdge *matchedEdge = NULL;
	for (unsigned int i=0; i < 3; i++)
	{
		if (edges[i]->matchesEdge(otherEdge))
		{
			matchedEdge = edges[i];
		}
		if (matchedEdge != NULL) { break; }
	}
	return matchedEdge;
}

void Face::splitFace(unsigned int nextVertexId, unsigned int generation)
{
	float displacementEffect = pow(0.5f, (float) generation-1);
	for (unsigned int i=0; i < 3; i++)
	{
		if (newVertices[i] == NULL)
		{
			Vec3f v0 = edges[i]->getVertex(0)->getVertex();
			Vec3f v1 = edges[i]->getVertex(1)->getVertex();
			float normalizedRadialHeight = (length(v0) + length(v1)) / 2.f;
			float random = (float) (rand() % 64) - 32.f;
			float displacement = random * displacementEffect;
			float finalRadialHeight = normalizedRadialHeight + displacement;
			Vec3f newVertex = (v1 + v0) * 0.5f;
			newVertex = normalize(newVertex) * finalRadialHeight;
			newVertices[i] = new Vertex(newVertex, nextVertexId);
			nextVertexId++;
			generatedVertices.push_back(newVertices[i]);
		}
	}
	
	newFaces[0] = new Face(vertices[0], newVertices[0], newVertices[2]);
	newFaces[1] = new Face(newVertices[0], vertices[1], newVertices[1]);
	newFaces[2] = new Face(newVertices[2], newVertices[1], vertices[2]);
	newFaces[3] = new Face(newVertices[0], newVertices[1], newVertices[2]);

	newFaces[0]->linkTo(newFaces[3]);
	newFaces[1]->linkTo(newFaces[3]);
	newFaces[2]->linkTo(newFaces[3]);
	
	int count0 = 0;
	int count1 = 0;
	int count2 = 0;
	for (unsigned int i=0; i < 4; i++)
	{
		Vec3f normal = newFaces[i]->computeNormal();
		switch (i)
		{
			case 0:
				count0++;
				count2++;
				newVertices[0]->saveNormal(normal);
				newVertices[2]->saveNormal(normal);
				break;
			case 1:
				count0++;
				count1++;
				newVertices[0]->saveNormal(normal);
				newVertices[1]->saveNormal(normal);
				break;
			case 2:
				count1++;
				count2++;
				newVertices[1]->saveNormal(normal);
				newVertices[2]->saveNormal(normal);
				break;
			case 3:
				count0++;
				count1++;
				count2++;
				newVertices[0]->saveNormal(normal);
				newVertices[1]->saveNormal(normal);
				newVertices[2]->saveNormal(normal);
				break;
		}
	}

	for (unsigned int i=0; i < 3; i++)
	{
		if (newNeighborFaces[i][0] == NULL)
		{
			neighborFaces[i]->saveNewVertex(newVertices[i], edges[i]->getOtherEdge()->getId());
			neighborFaces[i]->saveNewFaces(newFaces[i], newFaces[(i+1)%3], edges[i]->getOtherEdge()->getId());
		}
		else
		{
			newFaces[i]->linkTo(newNeighborFaces[i][0]);
			newFaces[(i+1)%3]->linkTo(newNeighborFaces[i][1]);
		}
	}
}

Vec3f Face::computeNormal()
{
	Vec3f v0 = vertices[1]->getVertex() - vertices[0]->getVertex();
	Vec3f v1 = vertices[2]->getVertex() - vertices[0]->getVertex();
	return normalize(v0 ^ v1);
}

void Face::saveNewVertex(Vertex *vertex, unsigned int edgeId)
{
	newVertices[edgeId] = vertex;
}

void Face::saveNewFaces(Face *f0, Face *f1, unsigned int edgeId)
{
	newNeighborFaces[edgeId][0] = f1;
	newNeighborFaces[edgeId][1] = f0;
}

vector< Vertex* > Face::getGeneratedVertices()
{
	return generatedVertices;
}

vector< Face* > Face::getNewFaces()
{
	vector< Face* > faces;
	for (unsigned int i=0; i < 4; i++)
	{
		faces.push_back(newFaces[i]);
	}
	return faces;
}

Vec<unsigned int, 3> Face::getIndices()
{
	Vec<unsigned int, 3> indices;
	for (unsigned int i=0; i < 3; i++)
	{
		indices[i] = vertices[i]->getId();
	}
	return indices;
}