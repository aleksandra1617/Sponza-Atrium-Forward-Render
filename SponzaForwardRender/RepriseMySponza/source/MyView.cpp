#include "MyView.hpp"

#include <sponza/sponza.hpp>
#include <tygra/FileHelper.hpp>

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <iostream>
#include <cassert>
#include <string>

MyView::MyView()
{
}

MyView::~MyView() 
{
}

void MyView::setScene(const sponza::Context * sponza)
{
    scene_ = sponza;
}

void MyView::AccumulateMeshData(sponza::Mesh mesh)
{
	//Creates a new MeshGL
	MeshGL newMesh;
	newMesh.first_element_index = 0;
	newMesh.first_vertex_index = 0;

	//Gets the geometry data for the current mesh from the builder 
	const auto& newNormals = mesh.getNormalArray();
	const auto& newPositions = mesh.getPositionArray();
	const auto& newElements = mesh.getElementArray();
	const auto& newTexCoordinates = mesh.getTextureCoordinateArray();

	//Record the first element index, element_count and first_vertex_index
	if (!meshMap.empty())
	{
		//Get the alwready accumulated verts and el size
		newMesh.first_element_index = elementData.size(); // Will be set to 0 in the begining if working with the first mesh.
		newMesh.first_vertex_index = posData.size(); // Will be set to 0 in the begining if working with the first mesh.
	}

	//The element count is always the size of the elements in the current 'mesh'.
	newMesh.element_count = newElements.size();

	//Accumulate the data into one vector array. Using 'insert' here because it's faster than 'push_back'.
	posData.insert(std::end(posData), std::begin(newPositions), std::end(newPositions));
	normData.insert(std::end(normData), std::begin(newNormals), std::end(newNormals));
	texCoordData.insert(std::end(texCoordData), std::begin(newTexCoordinates), std::end(newTexCoordinates));
	elementData.insert(std::end(elementData), std::begin(newElements), std::end(newElements));

	meshMap.insert(std::make_pair(mesh.getId(), newMesh));
}

//Swapping the std::vector's type to glm::vec3 when passing to function so that this function could be reused for any type of light
void MyView::AccumulateLightData(std::vector<glm::vec3> lightPos)
{

}

void MyView::Interleave()
{
	for (int c = 0; c < posData.size(); c++)
	{
		//Build the vertex
		Vertex newVert;
		newVert.pos = GLMVec3Cast(posData[c]);
		newVert.norm = GLMVec3Cast(normData[c]);

		if (c <= 26) //TODO: remove the magic num 26
		{
			glm::vec2 coord = glm::vec2(texCoordData[c].x, texCoordData[c].y); // Friends dont have tex coord put 0
			newVert.texCoord = (coord);
		}
		else
		{
			newVert.texCoord = (glm::vec2(0, 0));
		}

		//Add it onto the vector of vers
		vertexArrayData.push_back(newVert);
	}
}

glm::vec3 MyView::GLMVec3Cast(sponza::Vector3 s_Vec3)
{
	glm::vec3 glm_Vec3 = glm::vec3(s_Vec3.x, s_Vec3.y, s_Vec3.z);

	return glm_Vec3;
}

std::vector <GLuint> MyView::SendMatDataToShader(const sponza::Material mat)
{
	std::vector <GLuint> materialLocs;

	//------ GETTING MATERIAL DATA ------//	
	float shininess = mat.getShininess();
	
	glm::vec3 ambientColour = GLMVec3Cast(mat.getDiffuseColour());
	glm::vec3 diffusedColour = GLMVec3Cast(mat.getDiffuseColour());
	glm::vec3 specularColour = GLMVec3Cast(mat.getSpecularColour());

	//------PASSING MATERIAL DATA TO SHADER------//	
	//SHININESS
	GLuint shiniID = glGetUniformLocation(shade_handler_.program_, "shininess");
	glUniform1f(shiniID, shininess);

	//AMBIENT COLOUR
	GLuint ambID = glGetUniformLocation(shade_handler_.program_, "ambientColour");
	glUniform3fv(ambID, 1, glm::value_ptr(ambientColour));

	//DIFFUSED COLOUR
	GLuint diffID = glGetUniformLocation(shade_handler_.program_, "diffusedColour");
	glUniform3fv(diffID, 1, glm::value_ptr(diffusedColour));

	//SPECULAR COLOUR
	GLuint specID = glGetUniformLocation(shade_handler_.program_, "specularColour");
	glUniform3fv(specID, 1, glm::value_ptr(specularColour));

	//TODO: Use insert instead
	materialLocs.push_back(ambID);
	materialLocs.push_back(diffID);
	materialLocs.push_back(specID);

	return materialLocs;
}

glm::mat4 MyView::ComputeProjectionViewMatrix()
{
	//Compute aspect ratio 
	GLint viewportSize[4];
	glGetIntegerv(GL_VIEWPORT, viewportSize);
	const float aspectRatio = viewportSize[2] / (float)viewportSize[3];

	//Projection matrix
	glm::mat4 projectionXform = glm::perspective(
									glm::radians(scene_->getCamera().getVerticalFieldOfViewInDegrees()), 
									aspectRatio, 
									1.f, 
									1000.f);

	//View matrix
	glm::vec3 camPos = GLMVec3Cast(scene_->getCamera().getPosition());
	glm::vec3 camDir = GLMVec3Cast(scene_->getCamera().getDirection());

	glm::vec3 center = camPos + camDir;//look pos
	glm::vec3 upDir = GLMVec3Cast(scene_->getUpDirection());
	glm::mat4 viewXform = glm::lookAt(camPos, center, upDir);

	//Multiply the matrices together
	glm::mat4 projectionViewXform = projectionXform * viewXform;

	return projectionViewXform;
}

void MyView::CreateVBOs() 
{
	//ELEMENT VBO
	glGenBuffers(1, &element_vbo_);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, element_vbo_);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, elementData.size() * sizeof(unsigned int), elementData.data(), GL_STATIC_DRAW);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, kNullId);

	//VERTEX VBO
	glGenBuffers(1, &vertex_vbo_);
	glBindBuffer(GL_ARRAY_BUFFER, vertex_vbo_);
	glBufferData(GL_ARRAY_BUFFER, vertexArrayData.size() * sizeof(Vertex), vertexArrayData.data(), GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, kNullId);

	//INSTANCE VBO GENERATED (Holds Instance IDS) Should this be an interleaved VBO (materials, shininess??, model)
	int maxInstances = 10;
	glGenBuffers(1, &instance_vbo_);
	glBindBuffer(GL_ARRAY_BUFFER, instance_vbo_);
	glBufferData(GL_ARRAY_BUFFER, maxInstances * sizeof(InstanceData), NULL, GL_STREAM_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, kNullId);

	//----------UNIFORM BUFFER OBJECT-----------//
	//Find the UBO index 
	uboIndex = glGetUniformBlockIndex(shade_handler_.program_, "per_frame_ublock");

	//Determine block size 
	glGetActiveUniformBlockiv(shade_handler_.program_, uboIndex, GL_UNIFORM_BLOCK_DATA_SIZE, &uboSize);

	glGenBuffers(1, &ubo_);
	glBindBuffer(GL_UNIFORM_BUFFER, ubo_);
	glBufferData(GL_UNIFORM_BUFFER, uboSize, NULL, GL_STREAM_DRAW);
	glBindBufferBase(GL_UNIFORM_BUFFER, uboIndex, ubo_);
	glBindBuffer(GL_UNIFORM_BUFFER, kNullId);

	//VAO
	//BIND Element data
	glGenVertexArrays(1, &vao_);
	glBindVertexArray(vao_);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, element_vbo_);

	//--------------VERTICES--------------//
	glBindBuffer(GL_ARRAY_BUFFER, vertex_vbo_); //1. Bind Pos to VAO (only one at a time), it can't work with/bind more than one buffer at once.
	glEnableVertexAttribArray(eVertPos); //2. Enable Pos  //kVertexPosition = 0
	glVertexAttribPointer(eVertPos, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), 0); //sizeof(glm::vec3)

	glBindBuffer(GL_ARRAY_BUFFER, vertex_vbo_);
	glEnableVertexAttribArray(eVertNorm);
	glVertexAttribPointer(eVertNorm, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)(12)); //0*4 + 3*4 bytes

																						  //Handle TexCoord data
	glBindBuffer(GL_ARRAY_BUFFER, vertex_vbo_);
	glEnableVertexAttribArray(eTexCoord);
	glVertexAttribPointer(eTexCoord, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)(24)); //0*4 + 3*4 + 3*4 bytes
	int accOffset = 0;

	//------------ INSTANCE DATA ------------//
	glBindBuffer(GL_ARRAY_BUFFER, instance_vbo_);

	for (int i = 0; i < 4; i++)
	{
		// Set up the vertex attribute
		glEnableVertexAttribArray(eModelRow1 + i);
		glVertexAttribPointer(eModelRow1 + i, 4, GL_FLOAT, GL_FALSE, sizeof(InstanceData), (void *)(sizeof(GLfloat) * 4 * i));  //TODO: remember to multiply offset by sizeof(GLfloat)

																																// Make it instanced
		glVertexAttribDivisor(eModelRow1 + i, 1);
		accOffset = 4 * i;
	}
	accOffset = sizeof(glm::mat4);

	glBindBuffer(GL_ARRAY_BUFFER, instance_vbo_);
	glEnableVertexAttribArray(eAmbCol);
	glVertexAttribPointer(eAmbCol, 3, GL_FLOAT, GL_FALSE, sizeof(InstanceData), (void*)(accOffset)); // 16*4 byte offset
	glVertexAttribDivisor(eAmbCol, 1);
	accOffset += (3 * sizeof(float));

	glBindBuffer(GL_ARRAY_BUFFER, instance_vbo_);
	glEnableVertexAttribArray(eDiffCol);
	glVertexAttribPointer(eDiffCol, 3, GL_FLOAT, GL_FALSE, sizeof(InstanceData), (void*)(accOffset)); // 64 + 3*4
	glVertexAttribDivisor(eDiffCol, 1);
	accOffset += (3 * sizeof(float));

	glBindBuffer(GL_ARRAY_BUFFER, instance_vbo_);
	glEnableVertexAttribArray(eSpecCol);
	glVertexAttribPointer(eSpecCol, 3, GL_FLOAT, GL_FALSE, sizeof(InstanceData), (void*)(accOffset)); // 64 + 12 + 3*4
	glVertexAttribDivisor(eSpecCol, 1);

}

void MyView::windowViewWillStart(tygra::Window * window)
{
    assert(scene_ != nullptr);

	//CREATES A VERTEX SHADER
	GLuint vertex_shader = shade_handler_.CreateShader(GL_VERTEX_SHADER, "resource:///reprise_vs.glsl");

	//CREATES A FRAGMENT/PIXEL SHADER
	GLuint fragment_shader = shade_handler_.CreateShader(GL_FRAGMENT_SHADER, "resource:///reprise_fs.glsl");

	// ---------------------------------------------------------------> Do I need to Delete these GLuints?
	std::vector <GLuint> shaders = { vertex_shader, fragment_shader };

	// BIND Shaders to Program
	shade_handler_.LinkShaders(shaders);

	// ------- READ geometry data ------- //
	//Holds geometry data (vertex and element data) of every mesh in the scene
	sponza::GeometryBuilder builder;

	//Fill the map of meshes with the data from builder
	for (auto mesh : builder.getAllMeshes()) //Happens 27 times
	{
		AccumulateMeshData(mesh); // setup mesh struct and accumulates the mesh data
	}

	//Populates the Vertex struct and adds all the verices to an std::vector
	Interleave();

	CreateVBOs();
}

void MyView::windowViewDidReset(tygra::Window * window, int width, int height)
{
    glViewport(0, 0, width, height);
}

void MyView::windowViewDidStop(tygra::Window * window)
{
	glDeleteBuffers(1, &(vertex_vbo_));
	glDeleteBuffers(1, &(element_vbo_));
	glDeleteBuffers(1, &(instance_vbo_));
	glDeleteVertexArrays(1, &(vao_));
}

void MyView::windowViewRender(tygra::Window * window)
{
    assert(scene_ != nullptr);

	//Configuring OpenGL pipeline settings before drawing.
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LEQUAL);
    glClearColor(0.f, 0.f, 0.25f, 0.f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	
	glUseProgram(shade_handler_.program_);

	//LIGHTS
	const auto& dLights = scene_->getAllDirectionalLights();
	const auto& sLights = scene_->getAllSpotLights(); //5
	const auto& pLights = scene_->getAllPointLights();
	
	std::vector<Light> spotLights;
	std::vector<Light> dirLights;
	std::vector<Light> pointLights;

	for (auto light : sLights)
	{
		Light newLight;
		newLight.lightPos = GLMVec3Cast(light.getPosition());
		newLight.padding = 0;

		spotLights.push_back(newLight);
	}

	for (auto light : pLights)
	{
		Light newLight;
		newLight.lightPos = GLMVec3Cast(light.getPosition());
		newLight.padding = 0;

		pointLights.push_back(newLight);
	}

	//Compute projection-view
	glm::mat4 projectionViewXform = ComputeProjectionViewMatrix();

	//Get Camera Pos 
	glm::vec3 camPos = GLMVec3Cast(scene_->getCamera().getPosition());

	//------UBO-------//
	//Allocate space to buffer
	uniBufferData = (GLbyte*)malloc(uboSize);

	if (uniBufferData == NULL)
	{
		std::cerr << "Unable to allocate memmory to the uniform buffer! \n" << std::endl;
		exit(EXIT_FAILURE);
	}
	else
	{
		enum { eProjectionView, eSpotLight, ePointLight, eCameraPos, eNumUniforms };

		const char * names[eNumUniforms] = {
			"projection_view_xform",
			"spotLights",
			"pointLights", 
			"camPos"
		};

		//Determine where to write the data
		GLuint indices[eNumUniforms];
		GLint size[eNumUniforms];
		GLint offset[eNumUniforms];
		GLint type[eNumUniforms];

		//projection-view
		glGetUniformIndices(shade_handler_.program_, eNumUniforms, names, indices);
		glGetActiveUniformsiv(shade_handler_.program_, eNumUniforms, indices, GL_UNIFORM_OFFSET, offset);
		glGetActiveUniformsiv(shade_handler_.program_, eNumUniforms, indices, GL_UNIFORM_SIZE, size);
		glGetActiveUniformsiv(shade_handler_.program_, eNumUniforms, indices, GL_UNIFORM_TYPE, type);

		//Copy the uniform data into the buffer
		memcpy(uniBufferData, &projectionViewXform, size[eProjectionView] * 16 * sizeof(float));
		memcpy(uniBufferData + offset[eSpotLight], spotLights.data(), size[eSpotLight] * 4 * sizeof(float)); // 4 floats because added 1 float padding
		memcpy(uniBufferData + offset[ePointLight], pointLights.data(), size[ePointLight] * 4 * sizeof(float));
		memcpy((GLbyte*)uniBufferData + offset[eCameraPos], &camPos, size[eCameraPos] * 3 * sizeof(float));
	}

	std::vector<InstanceData> instanceDataArray;

	//Itterates through each mesh in the scene
	for (std::map<unsigned int, MeshGL>::iterator it = meshMap.begin(); it != meshMap.end(); ++it)
	{
		//Get the list of instances that need drawing (holds all instance IDS)
		std::vector<unsigned int> instances = scene_->getInstancesByMeshId(it->first);

		//Itterates through each instance of the current mesh
		for (auto instanceID : instances)
		{
			//Get the instance
			sponza::Instance instance = scene_->getInstanceById(instanceID);

			//Get the model matrix
			sponza::Matrix4x3 m_xform = instance.getTransformationMatrix();

			//Converts from sponza Mat4X3 to glm mat4x3
			glm::mat4x3 xform = glm::mat4x3(m_xform.m00, m_xform.m01, m_xform.m02,
				m_xform.m10, m_xform.m11, m_xform.m12,
				m_xform.m20, m_xform.m21, m_xform.m22,
				m_xform.m30, m_xform.m31, m_xform.m32);

			//Cast to mat4
			glm::mat4 model_xform = glm::mat4(xform);

			//Get the material per instance
			const sponza::Material mat = scene_->getMaterialById(instance.getMaterialId());

			//------ GETTING MATERIAL DATA ------//	
			float shininess = mat.getShininess();

			glm::vec3 ambientColour = GLMVec3Cast(mat.getDiffuseColour());
			glm::vec3 diffusedColour = GLMVec3Cast(mat.getDiffuseColour());
			glm::vec3 specularColour = GLMVec3Cast(mat.getSpecularColour());

			//SHININESS
			GLuint shiniID = glGetUniformLocation(shade_handler_.program_, "shininess"); //Strange ID value
			glUniform1f(shiniID, shininess);

			//Set this instance's data 
			InstanceData instStruct = {model_xform, ambientColour, diffusedColour, specularColour};

			instanceDataArray.push_back(instStruct);
		}
	
		glBindBuffer(GL_ARRAY_BUFFER, instance_vbo_);
		glBufferSubData(GL_ARRAY_BUFFER, 0, instanceDataArray.size() * sizeof(InstanceData), instanceDataArray.data());
		glBindBuffer(GL_ARRAY_BUFFER, kNullId);
		
		glBindBuffer(GL_UNIFORM_BUFFER, ubo_);
		glBufferSubData(GL_UNIFORM_BUFFER, 0, uboSize, uniBufferData);
		glBindBuffer(GL_UNIFORM_BUFFER, kNullId);

		//Only null the VAO after everything is added
		glBindBuffer(GL_ARRAY_BUFFER, kNullId);
		glBindVertexArray(kNullId);

		//Bind (attach to the pipeline) our geometry 27 times as there are 27 meshes
		glBindVertexArray(vao_);

		unsigned int elCount = it->second.element_count;
		unsigned int firstElInd = it->second.first_element_index * sizeof(int);
		unsigned int baseVert = it->second.first_vertex_index;

		//Is this supposed to draw all instances ofthe current mesh in one go?
		glDrawElementsInstancedBaseVertex(GL_TRIANGLES, elCount, GL_UNSIGNED_INT, (void *)(firstElInd), instances.size(), baseVert);

		//Clears the space so that it can start drawing the next mesh and it's instances. 
		instanceDataArray.clear();
	}
}