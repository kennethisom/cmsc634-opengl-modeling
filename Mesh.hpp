// terrain data and drawing
#ifndef Mesh_hpp
#define Mesh_hpp

#include "Face.hpp"
#include "Vertex.hpp"
#include "Vec.hpp"
#include <vector>
using namespace std;

class Mesh {

private:
	unsigned int generation;
	unsigned int maxGeneration;
	
    float mapSize;              // size of terrain in world space

    unsigned int numvert;       // total vertices
    Vec3f *vert;                // per-vertex position
    Vec3f *norm;                // per-vertex normal
    
    unsigned int numtri;        // total triangles
	unsigned int triOffset;
    vector< Vec<unsigned int, 3> > indices; // 3 vertex indices per triangle

	vector< Face* > faces;
	vector< Vertex* > vertices;

public:
    Mesh();
    ~Mesh();

	void increaseGeneration();
	void decreaseGeneration();
	void saveIndices();
	unsigned int getNumVert();
	Vec3f* getVertices();
	Vec3f* getNormals();
	unsigned int getNumTri();
	unsigned int getNumTriForGeneration(unsigned int i);
	unsigned int getTriOffset();
	Vec<unsigned int, 3>* getIndices();
};

#endif
