#ifndef HEdge_hpp
#define HEdge_hpp

#include "Vertex.hpp"

class HEdge {

private:
	unsigned int id;
	Vertex *vertices[2];
	HEdge *otherEdge;

public:
    HEdge(Vertex *v1, Vertex *v2, unsigned int identifier);
    ~HEdge();

	unsigned int getId();
	void setLink(HEdge *edge);
	bool matchesEdge(HEdge *otherEdge);
	Vertex* getVertex(unsigned int index);
	HEdge* getOtherEdge();
};

#endif
