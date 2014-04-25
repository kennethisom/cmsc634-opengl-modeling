// terrain data and drawing
#ifndef Terrain_hpp
#define Terrain_hpp

#include "Mesh.hpp"
#include "Vec.hpp"
#include "Shader.hpp"

// terrain data and rendering methods
class Terrain {
// private data
private:
	Mesh *mesh;

    unsigned int numvert;       // total vertices
    Vec3f *vert;                // per-vertex position
    Vec3f *norm;                // per-vertex normal
    
    unsigned int numtri;        // total triangles
    Vec<unsigned int, 3> *indices; // 3 vertex indices per triangle

    // GL vertex array object ID
    unsigned int varrayID;

    // GL buffer object IDs
    enum {POSITION_BUFFER, NORMAL_BUFFER, INDEX_BUFFER, NUM_BUFFERS};
    unsigned int bufferIDs[NUM_BUFFERS];

    // GL shaders
    unsigned int shaderID;      // ID for shader program
    ShaderInfo shaderParts[2];  // vertex & fragment shader info

	// get new terrain values to pass to GPU
	void updateTerrainData();

	// Send Triangle data to GPU
	void sendTrianglesToGPU();

// public methods
public:
    // load terrain, given elevation image and surface texture
    Terrain(const char *elevationPPM);

    // clean up allocated memory
    ~Terrain();

	// increase terrain generation
	void increaseDetail();

	// decrease terrain generation
	void decreaseDetail();

    // load/reload shaders
    void updateShaders();

    // draw this terrain object
    void draw() const;
};

#endif
