#include <CVK_2/CVK_Framework.h>
#include <CVK_AnSim/CVK_AS_MassPoint.h>
#include <CVK_AnSim/CVK_AS_ShaderSpace.h>

#include <iostream>
#include "Skybox.h"
#include "ShaderSkybox.h"

#include <sstream>


const int width = 1080;
const int height = 720;

GLFWwindow* window;

//define Camera (Trackball)
CVK::Perspective projection(60.0f, width / (float) height, 0.1f, 60.0f);
CVK::Trackball camera( width, height, &projection);

//*************************************************************************************************************
// space ship
CVK::MassPoint spaceShipMassPoint;
float spaceShipEngineForce 	= 10.0f;
float spaceShipRotAngle		= 0.0f;  // angle in degree

//*************************************************************************************************************

void keyCallback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
    if(action != GLFW_PRESS)
    	return;

    switch(key)
    {
    	case GLFW_KEY_ESCAPE:
    		glfwSetWindowShouldClose(window, GL_TRUE);
    		break;
    	case GLFW_KEY_S:
			spaceShipMassPoint.applyForce(spaceShipEngineForce * glm::vec3(glm::cos(glm::radians(spaceShipRotAngle)), 0.0f, -glm::sin(glm::radians(spaceShipRotAngle))));
			break;
    	case GLFW_KEY_LEFT:
    		spaceShipRotAngle += 10;
    		break;
    	case GLFW_KEY_RIGHT:
    		spaceShipRotAngle -= 10;
    		break;
    }
        
}

double calculateFPS(double interval = 1.0 , std::string title = "NONE"){
	static double tZero = glfwGetTime();
	static double fps = 0.0;

	static double frames = -1.0;

	frames ++;

	if (interval < 0.0)
		interval = 0.0;
	else
		if (interval > 10.0)
			interval = 10;

	double timeElapsed = glfwGetTime() - tZero;

	if (timeElapsed > interval){
		fps = frames / timeElapsed;
		if (title != "NONE"){
			std::ostringstream stream;
			stream << fps;
			std::string fpsToString = stream.str();

			title += " || Frames per second: " + fpsToString;

			const char* pszConstString = title.c_str();
			glfwSetWindowTitle(window, pszConstString);
		}
		else {
			// std::cout << "Frames per second: " + glm::to_string(fps) << endl;
		}
		frames = 0.0;
		tZero = glfwGetTime();
	}

	return fps;
}


void resizeCallback( GLFWwindow *window, int w, int h)
{
	camera.setWidthHeight( w, h);
	camera.getProjection()->updateRatio( w / (float) h);
	glViewport(0, 0, w, h);
}

int main() 
{
	// Init GLFW and GLEW
	glfwInit();
	CVK::useOpenGL33CoreProfile();
	window = glfwCreateWindow(width, height, "AnSim Exercise 3", 0, 0);
	glfwSetWindowPos(window, 100, 50);
	glfwMakeContextCurrent(window);
	glfwSetWindowSizeCallback(window, resizeCallback);
	glfwSetKeyCallback(window, keyCallback);
	glewInit();

	CVK::State::getInstance()->setBackgroundColor( white);
	glm::vec3 BgCol = CVK::State::getInstance()->getBackgroundColor();
	glClearColor( BgCol.r, BgCol.g, BgCol.b, 0.0);
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_BLEND);         
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);  

	//load, compile and link Shader
	const char *shadernames[2] = { SHADERS_PATH "/Toon.vert", SHADERS_PATH "/Toon.frag" };
	CVK::ShaderSpace spaceShader(VERTEX_SHADER_BIT | FRAGMENT_SHADER_BIT, shadernames);

    const char *skyboxShadernames[2] = {SHADERS_PATH "/Minimal.vert", SHADERS_PATH "/SimpleTexture.frag"};
    ShaderSkybox skyboxShader(VERTEX_SHADER_BIT | FRAGMENT_SHADER_BIT, skyboxShadernames);
    

	//Camera
	glm::vec3 center(0.0f, 0.0f, 0.0f);
	glm:: vec3 spaceShipPos(spaceShipMassPoint.getPosition());
	camera.setCenter(&spaceShipPos);
	camera.setRadius(6.0f);
	CVK::State::getInstance()->setCamera( &camera);

	//Light Sources
	CVK::Light light = CVK::Light(glm::vec4(0.0, 10.0, 0.0, 1.0), grey, glm::vec3(0, 0, 0), 1.0f, 0.0f);
	CVK::State::getInstance()->addLight(&light);
	CVK::State::getInstance()->updateSceneSettings(darkgrey, 0, white, 1, 10, 1);

	// Create skybox
    Skybox* skybox = new Skybox(15.0f);

	//Init scene nodes and mass points
	CVK::Node spaceship("Spaceship", RESOURCES_PATH "/sphere.obj");
	//First mass point for the spaceship
	spaceShipMassPoint = CVK::MassPoint(glm::vec3(0.0f, 2.3f, 0.0f),  glm::vec3(0.0f, 0.0f, 0.0f), 1.0);

	float deltaTime = 0.0f;
	float oldTime = glfwGetTime();
	float t = 0.0f;

	while(!glfwWindowShouldClose( window))
	{	
		float currentTime = glfwGetTime();
		deltaTime = currentTime - oldTime;
		oldTime = currentTime;
		calculateFPS(1.0, "OpenGL Window");

		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		
		//Update Camera
		camera.update(window);

		//Use the skybox shader program
        CVK::State::getInstance()->setShader(&skyboxShader);
        CVK::State::getInstance()->getShader()->update();
        skybox->render();

		//Use phong shader to render the scene
		CVK::State::getInstance()->setShader(&spaceShader);
		CVK::State::getInstance()->setLight(0, &light);
		spaceShader.update();

		//Update and render spaceship
		spaceShipMassPoint.numericIntegration(deltaTime);
		glm::mat4 modelmatrix = glm::rotate(glm::mat4(1.0f), spaceShipRotAngle, glm::vec3(0.0f,1.0f,0.0f));
		modelmatrix = glm::translate(modelmatrix, spaceShipMassPoint.getPosition());
		modelmatrix = glm::scale(modelmatrix, glm::vec3(1.5,1.5,1.5));
		spaceship.setModelMatrix(modelmatrix);
		glm:: vec3 spaceShipPos(spaceShipMassPoint.getPosition());
		camera.setCenter(&spaceShipPos);
		spaceship.render();
		
		t += deltaTime * 0.01f; //speed = 0.01
		if(t >= 1.0)
			t = 0.0f;

		glfwSwapBuffers( window);
		glfwPollEvents();
	}

	glfwDestroyWindow( window);
	glfwTerminate();


	return 0;
}

