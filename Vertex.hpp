// terrain data and drawing
#ifndef Vertex_hpp
#define Vertex_hpp

#include "Vec.hpp"
#include <vector>
using namespace std;


class Vertex {

private:
	unsigned int id;
    Vec3f vert;
    Vec3f norm;
	vector< Vec3f > norms;

public:
    Vertex(Vec3f vertex, unsigned int identifier);
    ~Vertex();

	void saveNormal(Vec3f normal);
	void computeNormal();

	unsigned int getId();
	Vec3f getVertex();
	Vec3f getNormal();
};

#endif
