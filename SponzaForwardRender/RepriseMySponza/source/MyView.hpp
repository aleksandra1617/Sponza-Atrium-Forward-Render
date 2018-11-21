#pragma once

#include <sponza/sponza_fwd.hpp>
#include <tygra/WindowViewDelegate.hpp>
#include <tgl/tgl.h>
#include <glm/glm.hpp>

#include <vector>
#include <map>
#include <memory>

#include "ShaderHandler.h"

class MyView : public tygra::WindowViewDelegate
{
public:

    MyView();
    ~MyView();

    void setScene(const sponza::Context * sponza);

private:
	ShaderHandler shade_handler_;

	/*
	*	Define integer constants to improve readability. Any integers
	*	found in the code should be counting numbers, not ids.
	*/
	const static GLuint kNullId = 0;

	struct Vertex		//previousfloatcount * sizeof(float) = offset
	{
		glm::vec3 pos;		// 0 * 4 = 0
		glm::vec3 norm;		// 3 * 4 = 12
		glm::vec2 texCoord; //6 * 4 = 24
		// if next one exists: 8 * 4 = 32
	};

	struct InstanceData
	{
		glm::mat4 modelMatrix;
		glm::vec3 ambColour;
		glm::vec3 diffColur;
		glm::vec3 specColour;
	};

	struct Light 
	{
		glm::vec3 lightPos;
		float padding; 
	};

	//Same as the order at which the attributes are passed to the vertex shader
	enum VertexAttribIndices 
	{ //PER MESH
		eVertPos = 0, eVertNorm = 1, eTexCoord = 2, 

	  //PER INSTANCE
		eModelRow1 = 3, eModelRow2 = 4, kModelRow3 = 5, kModelRow4 = 6, //Model Matrix
		eAmbCol = 7, eDiffCol = 8, eSpecCol = 9  //Light Material
	};

	enum InstanceIndices { eModelMatrix, eAmbMat, eDiffMat, eSpecMat };
	enum FragmentDataIndices { eFragColour = 0 };

	//VERTEX ARRAY
	std::vector <Vertex> vertexArrayData;

	//VERTEX DATA
	std::vector <sponza::Vector3> normData, posData;
	std::vector <sponza::Vector2> texCoordData;

	//ELEMENT DATA 
	std::vector <unsigned int> elementData;

	GLuint vertex_vbo_, element_vbo_, instance_vbo_, ubo_, vao_;

	//UBO data
	GLuint uboIndex;
	GLint uboSize;
	GLuint uboID;
	GLbyte * uniBufferData;

	struct MeshGL
	{
		unsigned int first_element_index;
		unsigned int element_count;
		unsigned int first_vertex_index;
	};

	std::map<sponza::MeshId, MeshGL> meshMap;


    void windowViewWillStart(tygra::Window * window) override;

    void windowViewDidReset(tygra::Window * window,
                            int width,
                            int height) override;

    void windowViewDidStop(tygra::Window * window) override;

    void windowViewRender(tygra::Window * window) override;

    const sponza::Context * scene_;

	//------- REFACTURED FUNCTIONS ---------// 
	/*
	*	Creates OpenGL buffers to hold the geometry data. This is
	*   how we pass our geometry data to OpenGL so it can draw it.
	*/
	void AccumulateMeshData(sponza::Mesh mesh);
	//void AccumulateUboData(const int loc);
	void AccumulateLightData(std::vector<glm::vec3> lightPositions);
	void Interleave();
	void CreateVBOs();

	//Casts a sponza::Vector3 to glm::vec3
	glm::vec3 GLMVec3Cast(sponza::Vector3 s_Vec3);
	std::vector <GLuint> SendMatDataToShader(const sponza::Material mat);
	glm::mat4 ComputeProjectionViewMatrix();

};


//https://www.opengl.org/discussion_boards/showthread.php/182312-Uniform-Buffer-Objects-dynamic-sized-arrays-and-lights