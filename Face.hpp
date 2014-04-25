#ifndef Face_hpp
#define Face_hpp

#include "Vertex.hpp"
#include "HEdge.hpp"
#include <vector>
using namespace std;

class Face {

private:
	Vertex *vertices[3];
	HEdge *edges[3];
	Face *neighborFaces[3];
	Vertex *newVertices[3];
	vector< Vertex* > generatedVertices;
	Face *newFaces[4];
	Face *newNeighborFaces[3][2];

public:
	Face(Vertex *v0, Vertex *v1, Vertex *v2);
    ~Face();
	
	void linkTo(Face *face);
	void setNeighbor(Face *otherFace, unsigned int edgeId);
	HEdge* getCompanionEdge(HEdge *otherEdge);
	void splitFace(unsigned int nextVertexId, unsigned int generation);
	Vec3f computeNormal();
	void saveNewVertex(Vertex *vertex, unsigned int edgeId);
	void saveNewFaces(Face *f0, Face *f1, unsigned int edgeId);
	vector< Vertex* > getGeneratedVertices();
	vector< Face* > getNewFaces();
	Vec<unsigned int, 3> getIndices();
};

#endif
