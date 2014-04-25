// draw a simple terrain height field

#include "Terrain.hpp"
#include "AppContext.hpp"
#include "ImagePPM.hpp"
#include "Vec.inl"

// using core modern OpenGL
#include <GL/glew.h>
#include <GLFW/glfw3.h>


//
// load the terrain data
//
Terrain::Terrain(const char *elevationPPM)
{
    // buffer objects to be used later
    glGenBuffers(NUM_BUFFERS, bufferIDs);
    glGenVertexArrays(1, &varrayID);

    // load terrain heights
    ImagePPM elevation(elevationPPM);
    unsigned int w = elevation.width, h = elevation.height;

	// Create new mesh
	mesh = new Mesh();

	// get numvert, vert, norm, numtri, indices
	updateTerrainData();

    // load vertex and index array to GPU
	sendTrianglesToGPU();

    // initial shader load
    shaderParts[0].id = glCreateShader(GL_VERTEX_SHADER);
    shaderParts[0].file = "terrain.vert";
    shaderParts[1].id = glCreateShader(GL_FRAGMENT_SHADER);
    shaderParts[1].file = "terrain.frag";
    shaderID = glCreateProgram();
    updateShaders();
}

//
// Delete terrain data
//
Terrain::~Terrain()
{
    glDeleteShader(shaderParts[0].id);
    glDeleteShader(shaderParts[1].id);
    glDeleteProgram(shaderID);
    glDeleteBuffers(NUM_BUFFERS, bufferIDs);
    glDeleteVertexArrays(1, &varrayID);

	delete mesh;
}

void Terrain::updateTerrainData()
{
	// get numvert, vert, norm, numtri, indices
	numvert = mesh->getNumVert();
	vert = mesh->getVertices();
	norm = mesh->getNormals();
	numtri = mesh->getNumTri();
	indices = mesh->getIndices();
}

void Terrain::sendTrianglesToGPU()
{
    // load vertex and index array to GPU
    glBindBuffer(GL_ARRAY_BUFFER, bufferIDs[POSITION_BUFFER]);
    glBufferData(GL_ARRAY_BUFFER, numvert*sizeof(Vec3f), vert, GL_STATIC_DRAW);

    glBindBuffer(GL_ARRAY_BUFFER, bufferIDs[NORMAL_BUFFER]);
    glBufferData(GL_ARRAY_BUFFER, numvert*sizeof(Vec3f), norm, GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, bufferIDs[INDEX_BUFFER]);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, 
                 numtri*sizeof(unsigned int[3]), indices, GL_STATIC_DRAW);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}

void Terrain::increaseDetail()
{
	// Tell mesh to increase generation
	mesh->increaseGeneration();

	// get numvert, vert, norm, numtri, indices
	updateTerrainData();

    // load vertex and index array to GPU
	sendTrianglesToGPU();
    updateShaders();
}

void Terrain::decreaseDetail()
{
	// Tell mesh to increase generation
	mesh->decreaseGeneration();

	// get numvert, vert, norm, numtri, indices
	updateTerrainData();

	// load vertex and index array to GPU
	sendTrianglesToGPU();
	updateShaders();
}

//
// load (or replace) terrain shaders
//
void Terrain::updateShaders()
{
    loadShaders(shaderID, sizeof(shaderParts)/sizeof(*shaderParts), 
                shaderParts);
    glUseProgram(shaderID);

    // (re)connect view and projection matrices
    glUniformBlockBinding(shaderID, 
                          glGetUniformBlockIndex(shaderID,"SceneData"),
                          AppContext::SCENE_UNIFORMS);

    // re-connect attribute arrays
    glBindVertexArray(varrayID);

    GLint positionAttrib = glGetAttribLocation(shaderID, "vPosition");
    glBindBuffer(GL_ARRAY_BUFFER, bufferIDs[POSITION_BUFFER]);
    glVertexAttribPointer(positionAttrib, 3, GL_FLOAT, GL_FALSE, 0, 0);
    glEnableVertexAttribArray(positionAttrib);

    GLint normalAttrib = glGetAttribLocation(shaderID, "vNormal");
    glBindBuffer(GL_ARRAY_BUFFER, bufferIDs[NORMAL_BUFFER]);
    glVertexAttribPointer(normalAttrib, 3, GL_FLOAT, GL_FALSE, 0, 0);
    glEnableVertexAttribArray(normalAttrib);

    // turn off everything we enabled
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
    glUseProgram(0);
}

//
// this is called every time the terrain needs to be redrawn 
//
void Terrain::draw() const
{
    // enable shaders
    glUseProgram(shaderID);

    // draw the triangles for each three indices
    glBindVertexArray(varrayID);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, bufferIDs[INDEX_BUFFER]);
    glDrawElements(GL_TRIANGLES, 3*numtri, GL_UNSIGNED_INT, 0);

    // turn off whatever we turned on
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
    glUseProgram(0);
}

