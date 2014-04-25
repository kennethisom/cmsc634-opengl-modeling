#include "Mesh.hpp"
#include "Vec.inl"
#include <math.h>
#include <stdlib.h>
#include <time.h>


Mesh::Mesh():
	generation(0), maxGeneration(0)
{
	srand(time(NULL));

    // world dimensions
    mapSize = 256.f;

	numvert = getNumVert();
	numtri = getNumTri();
	triOffset = getTriOffset();

	Vertex *vertex;
	
	vertex = new Vertex(vec3<float>(0.f, 0.f, 0.5f) * mapSize, vertices.size());
	vertex->saveNormal(vec3<float>(0.f, 0.f, 1.f));
	vertex->computeNormal();
	vertices.push_back(vertex);

	vertex = new Vertex(vec3<float>(0.f, 0.5f, 0.f) * mapSize, vertices.size());
	vertex->saveNormal(vec3<float>(0.f, 1.f, 0.f));
	vertex->computeNormal();
	vertices.push_back(vertex);

	vertex = new Vertex(vec3<float>(-0.5f, 0.f, 0.f) * mapSize, vertices.size());
	vertex->saveNormal(vec3<float>(-1.f, 0.f, 0.f));
	vertex->computeNormal();
	vertices.push_back(vertex);

	vertex = new Vertex(vec3<float>(0.f, -0.5f, 0.f) * mapSize, vertices.size());
	vertex->saveNormal(vec3<float>(0.f, -1.f, 0.f));
	vertex->computeNormal();
	vertices.push_back(vertex);

	vertex = new Vertex(vec3<float>(0.5f, 0.f, 0.f) * mapSize, vertices.size());
	vertex->saveNormal(vec3<float>(1.f, 0.f, 0.f));
	vertex->computeNormal();
	vertices.push_back(vertex);

	vertex = new Vertex(vec3<float>(0.f, 0.f, -0.5f) * mapSize, vertices.size());
	vertex->saveNormal(vec3<float>(0.f, 0.f, -1.f));
	vertex->computeNormal();
	vertices.push_back(vertex);

	faces.push_back(new Face(vertices[0], vertices[1], vertices[2]));
	faces.push_back(new Face(vertices[0], vertices[2], vertices[3]));
	faces.push_back(new Face(vertices[0], vertices[3], vertices[4]));
	faces.push_back(new Face(vertices[0], vertices[4], vertices[1]));
	faces.push_back(new Face(vertices[5], vertices[2], vertices[1]));
	faces.push_back(new Face(vertices[5], vertices[3], vertices[2]));
	faces.push_back(new Face(vertices[5], vertices[4], vertices[3]));
	faces.push_back(new Face(vertices[5], vertices[1], vertices[4]));
	
	faces[0]->linkTo(faces[1]);
	faces[1]->linkTo(faces[2]);
	faces[2]->linkTo(faces[3]);
	faces[3]->linkTo(faces[0]);
	
	faces[4]->linkTo(faces[5]);
	faces[5]->linkTo(faces[6]);
	faces[6]->linkTo(faces[7]);
	faces[7]->linkTo(faces[4]);

	faces[0]->linkTo(faces[4]);
	faces[1]->linkTo(faces[5]);
	faces[2]->linkTo(faces[6]);
	faces[3]->linkTo(faces[7]);
	saveIndices();
}

Mesh::~Mesh()
{
	indices.empty();
    delete[] norm;
    delete[] vert;
	/*for(unsigned int i=0; i < faces.size(); i++)
	{
		delete faces[i];
	}
	for(unsigned int i=0; i < vertices.size(); i++)
	{
		delete vertices[i];
	}*/
}

void Mesh::increaseGeneration()
{
	generation++;
	if (generation > maxGeneration)
	{
		maxGeneration = generation;

		vector< Face* > newFaces;
		vector< Vertex* > newVertices;
		for (unsigned int i=0; i < faces.size(); i++)
		{
			faces[i]->splitFace(vertices.size(), generation);
			vector< Vertex* > newGeneratedVertices = faces[i]->getGeneratedVertices();
			for (unsigned int i=0; i < newGeneratedVertices.size(); i++)
			{
				newVertices.push_back(newGeneratedVertices[i]);
				vertices.push_back(newGeneratedVertices[i]);
			}
		}
		for (unsigned int i=0; i < faces.size(); i++)
		{
			vector< Face* > tempFaces = faces[i]->getNewFaces();
			for (unsigned int i=0; i < tempFaces.size(); i++)
			{
				newFaces.push_back(tempFaces[i]);
			}
		}
		for (unsigned int i=0; i < newVertices.size(); i++)
		{
			newVertices[i]->computeNormal();
		}
		for (unsigned int i=0; i < faces.size(); i++)
		{
			delete faces[i];
		}
		faces.empty();
		faces = newFaces;
		newFaces.empty();
		newVertices.empty();
		saveIndices();
	}
	numvert = getNumVert();
	numtri = getNumTri();
	triOffset = getTriOffset();
}

void Mesh::decreaseGeneration()
{
	if (generation > 0)
	{
		generation--;
		numvert = getNumVert();
		numtri = getNumTri();
		triOffset = getTriOffset();
	}
}

void Mesh::saveIndices()
{
	for (unsigned int i=0; i < faces.size(); i++)
	{
		indices.push_back(faces[i]->getIndices());
	}
}

unsigned int Mesh::getNumVert()
{
	return (4 * (int) pow(4.f, (float) generation)) + 2;
}

Vec3f* Mesh::getVertices()
{
	// iterate over vertices array from 0  while less than getNumVert()
	vert = new Vec3f[numvert];
	for (unsigned int i=0; i < numvert; i++)
	{
		vert[i] = vertices[i]->getVertex();
	}
	return vert;
}

Vec3f* Mesh::getNormals()
{
	// iterate over vertices array from 0  while less than getNumVert()
	norm = new Vec3f[numvert];
	for (unsigned int i=0; i < numvert; i++)
	{
		norm[i] = vertices[i]->getNormal();
	}
	return norm;
}

unsigned int Mesh::getNumTri()
{
	return 8 * (int) pow(4.f, (float) generation);
}

unsigned int Mesh::getNumTriForGeneration(unsigned int i)
{
	return 8 * (int) pow(4.f, (float) i);
}

unsigned int Mesh::getTriOffset()
{
	unsigned int offset = 0;
	for (unsigned int i=0; i < generation; i++)
	{
		offset += getNumTriForGeneration(i);
	}
	return offset;
}

Vec<unsigned int, 3>* Mesh::getIndices()
{
	// iterate over faces array from 0(???) while less than getNumTri()(???)
	Vec<unsigned int, 3>* currIndices;
	currIndices = new Vec<unsigned int, 3>[numtri];
	for (unsigned int i=0; i < numtri; i++)
	{
		currIndices[i] = indices[triOffset+i];
	}
	return currIndices;
}