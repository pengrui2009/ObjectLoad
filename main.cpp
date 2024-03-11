#define STB_IMAGE_IMPLEMENTATION

#include "glew/glew.h"
#include "glfw/glfw3.h"
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/type_ptr.hpp"
#include "stb_image.h"
#include "Utils.h"
#include "ImportedModel.h"
#include "camera.h"
#include <iostream>
#include <fstream>
#include <string>
#include <vector>

using namespace std;

static const int screen_width = 1920;
static const int screen_height = 1080;

static const int numVAOs = 1;
static const int numVBOs = 3;

static const float pai = 3.1415926f;

GLuint renderingProgram = 0;
GLuint vao[numVAOs] = { 0 };
GLuint vbo[numVBOs] = { 0 };

GLuint renderingProgram1 = 0;
GLuint vao1[numVAOs] = { 0 };
GLuint vbo1[numVBOs] = { 0 };

int width = 0;
int height = 0;
float aspect = 0.f;
float objLocX = 0.f, objLocY = 0.f, objLocZ = 0.f;
GLuint mvLoc = 0;
GLuint projLoc = 0;
GLuint modelLoc = 0;

GLuint snowTextureId = 0;
GLuint penguinTextureId = 0;


glm::mat4 mMat(1.f), vMat(1.f), pMat(1.f), mvMat(1.f);

Camera camera(glm::vec3(0.f, 0.f, 3.f));
float cameraX, cameraY, cameraZ;

ImportedModel snow_model("resources/snow/snow.obj");
ImportedModel penguin_model("resources/penguin/penguin.obj");


GLboolean keys[1024] = { GL_FALSE };
GLboolean buttons[GLFW_MOUSE_BUTTON_LAST] = { GL_FALSE };
GLboolean firstMouse = GL_TRUE;
float deltaTime = 0.f;
float lastFrame = 0.f;
float lastLocX = 0.f;
float lastLocY = 0.f;

float toRadians(float degrees)
{
	return (degrees * 2.f * pai) / 360.f;
}

void setupVertices(void)
{
	vector<glm::vec3> penguin_vert = penguin_model.getVertices();
	vector<glm::vec2> penguin_text = penguin_model.getTextureCoords();
	vector<glm::vec3> penguin_norm = penguin_model.getNormals();

	vector<glm::vec3> snow_vert = snow_model.getVertices();
	vector<glm::vec2> snow_text = snow_model.getTextureCoords();
	vector<glm::vec3> snow_norm = snow_model.getNormals();

	vector<float> snow_pValues;
	vector<float> snow_tValues;
	vector<float> snow_nValues;

	vector<float> penguin_pValues;
	vector<float> penguin_tValues;
	vector<float> penguin_nValues;
	
	/*************************load snow obj info********************************/
	for (int i=0; i< snow_model.getNumVertices(); i++)
	{
		/*pValues.push_back(vert[i * 3 + 0].x);
		pValues.push_back(vert[i * 3 + 1].y);
		pValues.push_back(vert[i * 3 + 2].z);

		tValues.push_back(text[i * 2 + 0].s);
		tValues.push_back(text[i * 2 + 1].t);

		nValues.push_back(norm[i * 3 + 0].x);
		nValues.push_back(norm[i * 3 + 1].y);
		nValues.push_back(norm[i * 3 + 2].z);*/

		snow_pValues.push_back(snow_vert[i].x);
		snow_pValues.push_back(snow_vert[i].y);
		snow_pValues.push_back(snow_vert[i].z);

		snow_tValues.push_back(snow_text[i].s);
		snow_tValues.push_back(snow_text[i].t);

		snow_nValues.push_back(snow_norm[i].x);
		snow_nValues.push_back(snow_norm[i].y);
		snow_nValues.push_back(snow_norm[i].z);
	}

	glGenVertexArrays(numVAOs, vao1);
	glBindVertexArray(vao1[0]);

	glGenBuffers(numVBOs, vbo1);
	glBindBuffer(GL_ARRAY_BUFFER, vbo1[0]);
	glBufferData(GL_ARRAY_BUFFER, snow_pValues.size() * sizeof(float), &(snow_pValues[0]), GL_STATIC_DRAW);
	//glBufferData(GL_ARRAY_BUFFER, penguin_model.getVertices().size() * sizeof(float), &(pVlaues[0]), GL_STATIC_DRAW);

	glBindBuffer(GL_ARRAY_BUFFER, vbo1[1]);
	glBufferData(GL_ARRAY_BUFFER, snow_tValues.size() * sizeof(float), &(snow_tValues[0]), GL_STATIC_DRAW);
	//glBufferData(GL_ARRAY_BUFFER, penguin_model.getTextureCoords().size() * sizeof(float), &(tValues[0]), GL_STATIC_DRAW);

	glBindBuffer(GL_ARRAY_BUFFER, vbo1[2]);
	glBufferData(GL_ARRAY_BUFFER, snow_nValues.size() * sizeof(float), &(snow_nValues[0]), GL_STATIC_DRAW);
	//glBufferData(GL_ARRAY_BUFFER, penguin_model.getNormals().size() * sizeof(float), &(nValues[0]), GL_STATIC_DRAW);

	/*************************load penguin obj info********************************/
	for (int i = 0; i < penguin_model.getNumVertices(); i++)
	{
		/*pValues.push_back(vert[i * 3 + 0].x);
		pValues.push_back(vert[i * 3 + 1].y);
		pValues.push_back(vert[i * 3 + 2].z);

		tValues.push_back(text[i * 2 + 0].s);
		tValues.push_back(text[i * 2 + 1].t);

		nValues.push_back(norm[i * 3 + 0].x);
		nValues.push_back(norm[i * 3 + 1].y);
		nValues.push_back(norm[i * 3 + 2].z);*/

		penguin_pValues.push_back(penguin_vert[i].x);
		penguin_pValues.push_back(penguin_vert[i].y);
		penguin_pValues.push_back(penguin_vert[i].z);

		penguin_tValues.push_back(penguin_text[i].s);
		penguin_tValues.push_back(penguin_text[i].t);

		penguin_nValues.push_back(penguin_norm[i].x);
		penguin_nValues.push_back(penguin_norm[i].y);
		penguin_nValues.push_back(penguin_norm[i].z);
	}

	glGenVertexArrays(numVAOs, vao);
	glBindVertexArray(vao[0]);

	glGenBuffers(numVBOs, vbo);
	glBindBuffer(GL_ARRAY_BUFFER, vbo[0]);
	glBufferData(GL_ARRAY_BUFFER, penguin_pValues.size() * sizeof(float), &(penguin_pValues[0]), GL_STATIC_DRAW);
	//glBufferData(GL_ARRAY_BUFFER, penguin_model.getVertices().size() * sizeof(float), &(pVlaues[0]), GL_STATIC_DRAW);

	glBindBuffer(GL_ARRAY_BUFFER, vbo[1]);
	glBufferData(GL_ARRAY_BUFFER, penguin_tValues.size() * sizeof(float), &(penguin_tValues[0]), GL_STATIC_DRAW);
	//glBufferData(GL_ARRAY_BUFFER, penguin_model.getTextureCoords().size() * sizeof(float), &(tValues[0]), GL_STATIC_DRAW);

	glBindBuffer(GL_ARRAY_BUFFER, vbo[2]);
	glBufferData(GL_ARRAY_BUFFER, penguin_nValues.size() * sizeof(float), &(penguin_nValues[0]), GL_STATIC_DRAW);
	//glBufferData(GL_ARRAY_BUFFER, penguin_model.getNormals().size() * sizeof(float), &(nValues[0]), GL_STATIC_DRAW);



}

std::string texture_penguin_filepath = "resources/penguin/penguin_01.png";
std::string texture_snow_filepath = "resources/snow/snow_01.jpg";

void get_OpenGL_info()
{
	// OpenGL information
	const GLubyte* name = glGetString(GL_VENDOR);
	const GLubyte* renderer = glGetString(GL_RENDERER);
	const GLubyte* glversion = glGetString(GL_VERSION);
	std::cout << "OpenGL company: " << name << std::endl;
	std::cout << "Renderer name: " << renderer << std::endl;
	std::cout << "OpenGL version: " << glversion << std::endl;
}

void initializedGL(GLFWwindow* window)
{

	get_OpenGL_info();

	renderingProgram = Utils::createShaderProgram("vertShader.glsl", "fragShader.glsl");

	//glfwGetWindowSize(window, &width, &height);
	cameraX = 0.0f; cameraY = 0.0f; cameraZ = 2.f;
	objLocX = 0.0f; objLocY = 0.0f; objLocZ = 0.00f;

	glfwGetFramebufferSize(window, &width, &height);
	aspect = (float)width / (float)height;
	pMat = glm::perspective(toRadians(45.f), aspect, 0.01f, 1000.f);
	
	setupVertices();
	
	/*lastLocX = (float)screen_width / 2.f;
	lastLocY = (float)screen_height / 2.f;*/
}


void window_size_callback(GLFWwindow* window, int newWidth, int newHeight)
{
	//��Ļ����ʹ��ڵ�֡����
	/*GLFW���������������ĵ��е���������ϵ��
		�����֮����������������ڼ������� / �򴰿ڵģ������Բ�һ����Ӧ����ʵ��Ļ���ص����쵥Ԫ������ ��DPI���ű�����ʱ�����磬�ڴ�������Ĥ��ʾ����Mac�ϣ������������ˡ�
		�봰��������ȣ�֡�������Ĵ�С��������أ��Ա���glViewport OpenGLsҪ����ƥ�䡣
		��ע�⣬��ĳЩϵͳ�ϣ�����������������������ͬ�����ⲻһ������ȷ�ġ�*/
	aspect = (float)newWidth / (float)newHeight;
	glViewport(0, 0, newWidth, newHeight);

	cameraX = 0.0f; cameraY = 0.0f; cameraZ =4.f;
	objLocX = 0.0f; objLocY = 0.0f; objLocZ = 0.0f;
	mMat = glm::translate(glm::mat4(1.f), glm::vec3(objLocX, objLocY, objLocZ));
	//pMat = glm::perspective(glm::radians(45.f), aspect, 0.001f, 1000.f);
	pMat = glm::perspective(camera.Zoom, aspect, 0.001f, 1000.f);
}

float left_right_step = 0.0;
float up_down_step = 0.0;
float light_step = 0.7;

void do_movement(void)
{
	if (keys[GLFW_KEY_1])
	{
		texture_penguin_filepath = "resources/penguin/penguin_01.png";
	}

	if (keys[GLFW_KEY_2])
	{
		texture_penguin_filepath = "resources/penguin/penguin_02.png";
	}

	if (keys[GLFW_KEY_3])
	{
		texture_snow_filepath = "resources/snow/snow_01.jpg";
	}

	if (keys[GLFW_KEY_4])
	{
		texture_snow_filepath = "resources/snow/snow_02.jpg";
	}

	if (keys[GLFW_KEY_W])
	{
		// camera.ProcessKeyboard(FORWARD, deltaTime);
		light_step += 0.005;
	}
	if (keys[GLFW_KEY_S])
	{
		// camera.ProcessKeyboard(BACKWARD, deltaTime);
		light_step -= 0.005;
	}
#if 0
	if (keys[GLFW_KEY_A])
	{
		camera.ProcessKeyboard(LEFT, deltaTime);
	}
	if (keys[GLFW_KEY_D])
	{
		camera.ProcessKeyboard(RIGHT, deltaTime);
	}
#endif
	if (keys[GLFW_KEY_UP])
	{
		up_down_step += 0.2;
		// camera.ProcessKeyboard(FORWARD, deltaTime);		
	}
	if (keys[GLFW_KEY_DOWN])
	{
		up_down_step -= 0.2;
		// camera.ProcessKeyboard(BACKWARD, deltaTime);
	}
	if (keys[GLFW_KEY_RIGHT])
	{
		// camera.ProcessKeyboard(LEFT, deltaTime);
		left_right_step += 0.3;
	}
	if (keys[GLFW_KEY_LEFT])
	{
		// camera.ProcessKeyboard(RIGHT, deltaTime);
		left_right_step -= 0.3;
	}
}

void paintGL(GLFWwindow* window, double currentTime)
{
	glClearColor(1.0f, 1.0f, 1.0f, 0.5f); //specify the background color, this is just an example
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	if (!texture_snow_filepath.empty()) {
		snowTextureId = Utils::loadTexture(texture_snow_filepath.c_str());
		texture_snow_filepath.clear();
	}

	if (!texture_penguin_filepath.empty()) {
		penguinTextureId = Utils::loadTexture(texture_penguin_filepath.c_str());
		texture_penguin_filepath.clear();
	}
	
	//������ɫ������,��GPU�ϰ�װGLSL����,�ⲻ��������ɫ������
	glUseProgram(renderingProgram);

	deltaTime = currentTime - lastFrame;
	lastFrame = currentTime;
#if 1
	// �ƹ�λ��
	glm::vec3 lampPos(0.3f, -0.5f, 0.0f);
	// ���ù�Դ���� ���Դ
	GLint lightAmbientLoc = glGetUniformLocation(renderingProgram, "light.ambient");
	GLint lightDiffuseLoc = glGetUniformLocation(renderingProgram, "light.diffuse");
	GLint lightSpecularLoc = glGetUniformLocation(renderingProgram, "light.specular");
	GLint lightPosLoc = glGetUniformLocation(renderingProgram, "light.position");
	GLint attConstant = glGetUniformLocation(renderingProgram, "light.constant");
	GLint attLinear = glGetUniformLocation(renderingProgram, "light.linear");
	GLint attQuadratic = glGetUniformLocation(renderingProgram, "light.quadratic");
	glUniform3f(lightAmbientLoc, 0.2f, 0.2f, 0.2f);
	glUniform3f(lightDiffuseLoc, 0.5f, 0.5f, 0.5f);
	glUniform3f(lightSpecularLoc, 1.0f, 1.0f, 1.0f);
	glUniform3f(lightPosLoc, lampPos.x, lampPos.y, lampPos.z);
	// ����˥��ϵ��
	glUniform1f(attConstant, light_step);
	glUniform1f(attLinear, 0.09f);
	glUniform1f(attQuadratic, 0.032f);
	// ���ò��Ϲ�������

	// ����diffuseMap
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, snowTextureId);

	// ����specularMap
	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, snowTextureId);

	GLint objectShininessLoc = glGetUniformLocation(renderingProgram, "material.shininess");
	glUniform1f(objectShininessLoc, 32.0f);
	// ���ù۲���λ��
	GLint viewPosLoc = glGetUniformLocation(renderingProgram, "viewPos");
	glUniform3f(viewPosLoc, cameraX, cameraY, cameraZ);
#endif

	do_movement();

	//��ȡuniform��������ɫ�������е�λ����ţ�ͨ������ſ�������һ�±�����ֵ�����û�иñ����򷵻�-1
	mvLoc = glGetUniformLocation(renderingProgram, "mv_matrix");
	modelLoc = glGetUniformLocation(renderingProgram, "model_matrix");
	projLoc = glGetUniformLocation(renderingProgram, "proj_matrix");

	mMat = glm::translate(glm::mat4(1.f), glm::vec3(objLocX, objLocY, objLocZ));

	//vMat = glm::translate(glm::mat4(1.0f), glm::vec3(-cameraX, -cameraY, -cameraZ));
	vMat = camera.GetViewMatrix();


	mMat = glm::rotate(mMat, toRadians(0.f), glm::vec3(1.f, 0.f, 0.f));
	mMat = glm::rotate(mMat, toRadians(0.f), glm::vec3(0.f, 1.f, 0.f));
	mMat = glm::rotate(mMat, toRadians(0.0f), glm::vec3(0.f, 0.f, 1.f));

	//������Ҫ�У���������н�ʧЧ
	pMat = glm::perspective(camera.Zoom, aspect, 0.01f, 1000.f);

	mvMat = vMat * mMat;

	//����һ��uniform��������������ֵ��Ҫ���ĵ�uniform������λ����locationָ����location��ֵӦ����glGetUniformLocation��������
	// ��͸�Ӿ����MV �����Ƹ���Ӧ��ͳһ����
	/*ͨ��һ�±�����uniform���εı��������ý�һ�±���ֵ������Ⱦ���ߡ�
		location : uniform��λ�á�
		count : ��Ҫ�������ݵ�����Ԫ�ص�����������Ҫ�޸ĵľ����������
		transpose : ָ��������������(column major)����GL_FALSE������������(row major)����GL_TRUE����
		value : ָ����count��Ԫ�ص������ָ�롣
	*/
	glUniformMatrix4fv(mvLoc, 1, GL_FALSE, glm::value_ptr(mvMat));
	glUniformMatrix4fv(projLoc, 1, GL_FALSE, glm::value_ptr(pMat));  //GL_FALSE �������ܴ�����޷���ʾobjģ��


	/*************************draw snow obj********************************/ 
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, snowTextureId);

	glBindVertexArray(vao1[0]);
	glBindBuffer(GL_ARRAY_BUFFER, vbo1[0]);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
	//ָ������Ⱦʱ����ֵΪ index �Ķ���������������ݸ�ʽ��λ��
	/*Parameters
	index
		ָ��Ҫ�޸ĵĶ������Ե�����ֵ

		size
		ָ��ÿ���������Ե��������������Ϊ1��2��3����4����ʼֵΪ4������ά����position����3����x, y, z����ɣ�����ɫ��4����r, g, b, a����

		type
		ָ��������ÿ��������������͡����õķ��ų�����GL_BYTE, GL_UNSIGNED_BYTE, GL_SHORT, GL_UNSIGNED_SHORT, GL_FIXED, �� GL_FLOAT����ʼֵΪGL_FLOAT��

		normalized
		ָ����������ʱ���̶�������ֵ�Ƿ�Ӧ�ñ���һ����GL_TRUE������ֱ��ת��Ϊ�̶���ֵ��GL_FALSE����

		stride
		ָ��������������֮���ƫ���������Ϊ0����ô�������Իᱻ���Ϊ�������ǽ���������һ��ġ���ʼֵΪ0��

		pointer
		ָ��һ��ָ�룬ָ�������е�һ���������Եĵ�һ���������ʼֵΪ0��
		*/
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
	//������ã�������ͨ�ö������������е�ֵ�����ڵ��ö������������glDrawArrays��glDrawElements��ʱ���ڳ��֡�
	//��Ӧ������ɫ����layout(location=0) in vec3 position;
	glEnableVertexAttribArray(0);

	glBindBuffer(GL_ARRAY_BUFFER, vbo1[1]);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(1);

	glBindBuffer(GL_ARRAY_BUFFER, vbo1[2]);
	glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(2);

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, snowTextureId);

	glEnable(GL_DEPTH_TEST);
	//ָ��������Ȼ���Ƚ�ֵ��
	glDepthFunc(GL_LEQUAL);

	glm::mat4 modelMat1 = glm::mat4(1.0f);

	glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(modelMat1));

	glDrawArrays(GL_TRIANGLES, 0, penguin_model.getNumVertices() + snow_model.getNumVertices());
	
	/*************************draw penguin obj********************************/
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, penguinTextureId);

	glBindVertexArray(vao[0]);
	glBindBuffer(GL_ARRAY_BUFFER, vbo[0]);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(0);

	glBindBuffer(GL_ARRAY_BUFFER, vbo[1]);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(1);

	glBindBuffer(GL_ARRAY_BUFFER, vbo[2]);
	glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(2);

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, penguinTextureId);

	glEnable(GL_DEPTH_TEST);
	//ָ��������Ȼ���Ƚ�ֵ��
	glDepthFunc(GL_LEQUAL);

#if 0
	glm::mat4 model;
	glm::vec3 cubePositions[] = {
		glm::vec3(0.0f, 0.0f, 0.0f),
		glm::vec3(2.0f, 5.0f, -15.0f),
		glm::vec3(-1.5f, -2.2f, -2.5f),
		glm::vec3(-3.8f, -2.0f, -12.3f),
		glm::vec3(2.4f, -0.4f, -3.5f),
		glm::vec3(-1.7f, 3.0f, -7.5f),
		glm::vec3(1.3f, -2.0f, -2.5f),
		glm::vec3(1.5f, 2.0f, -2.5f),
		glm::vec3(1.5f, 0.2f, -1.5f),
		glm::vec3(-1.3f, 1.0f, -1.5f)
	};

	model = glm::translate(model, cubePositions[0]);
	float angle = 20.0f * step;
	model = glm::rotate(model, glm::radians(angle), glm::vec3(1.0f, 0.3f, 0.5f));
	// shader.setMat4("model", glm::value_ptr(model));
	// projModel = glGetUniformLocation(renderingProgram, "model_matrix");
	glUniformMatrix4fv(glGetUniformLocation(renderingProgram, "model_matrix"), 1, GL_FALSE, glm::value_ptr(model));
#endif

	glm::mat4 modelMat = glm::mat4(1.0f);
	float step = 1.0;
	modelMat = glm::translate(modelMat, glm::vec3(0.01f, -1.0f, 0.0f + up_down_step));
	float angle = 1.0f + left_right_step;// 2.0f * left_right_step;
	modelMat = glm::rotate(modelMat, glm::radians(angle), glm::vec3(0.0f, 1.0f, 0.0f));

	glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(modelMat));

	glDrawArrays(GL_TRIANGLES, 0, penguin_model.getNumVertices());

}


void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode)
{
	if ((key == GLFW_KEY_ESCAPE) && (action == GLFW_PRESS))
	{
		glfwSetWindowShouldClose(window, GLFW_TRUE);
	}
	if (action == GLFW_PRESS)
	{
		keys[key] = GLFW_TRUE;
	}
	else if (action == GLFW_RELEASE)
	{
		keys[key] = GLFW_FALSE;
	}
}

void cursor_position_callback(GLFWwindow* window, double xPos, double yPos)
{
	if (buttons[GLFW_MOUSE_BUTTON_LEFT] == GLFW_TRUE)
	{
		if (firstMouse)
		{
			lastLocX = xPos;
			lastLocY = yPos;
			firstMouse = GL_FALSE;
		}

		double offsetLocX = xPos - lastLocX;
		double offsetLocY = lastLocY - yPos;

		lastLocX = xPos;
		lastLocY = yPos;

		camera.ProcessMouseMovement(offsetLocX, offsetLocY);
	}
}

void scroll_callback(GLFWwindow* window, double xPos, double yPos)
{
	camera.ProcessMouseScroll(yPos/ 20.0);
}

void mouse_button_callback(GLFWwindow* window, int button, int action, int mods)
{
	// Sets the mouse-button callback for the current window.
	if (action == GLFW_PRESS)
	{
		buttons[button] = GLFW_TRUE;
	}
	else if (action == GLFW_RELEASE)
	{
		buttons[button] = GLFW_FALSE;
	}
}

int main(int argc, char** argv)
{
	int glfwState = glfwInit();
	if (GLFW_FALSE == glfwState)
	{
		cout << "GLFW initialize failed,invoke glfwInit()......Error file:" << __FILE__ << "......Error line:" << __LINE__ << endl;
		glfwTerminate();
		exit(EXIT_FAILURE);
	}

	/*��Ϊ����Ҫʹ��OpenGL 4.6���������ǰ�GLFW_CONTEXT_VERSION_MAJOR��GLFW_CONTEXT_VERSION_MINOR��Ӧ��hint������Ϊ4��6��
	��Ϊ����Ҫʹ��OpenGL����ģʽ�����������ᵽ���ࣩ���������ǰ�GLFW_OPENGL_PROFILE��Ӧ��hint����ΪGLFW_OPENGL_CORE_PROFILE��
	��ʾʹ��OpenGL����ģʽ����󣬰�GLFW_RESIZABLE��Ӧ��hint����ΪGLFW_FALSE����ʾ���ڲ������û�������С��
	֮��������������Ϊ��������û�������С����С�����仯�󣬴��ڵĻ�������Ĭ�ϲ��䣨��Ȼ��ԭ�����ڵ����򣩣�
	Ҳ����˵�����ϻ��Ƶ�ͼ��Ĵ�С��λ�ò��ᷢ���ı䡣Ϊ�˱������������������Ǿͼ򵥵ز����û��������ڴ�С
	����ȻҲ�и��õķ�����������GLFW����һ�����ڴ�С�Ļص��������������Ƚϼ򵥣���*/
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
	glfwWindowHint(GLFW_OPENGL_CORE_PROFILE, GLFW_OPENGL_PROFILE);
	glfwWindowHint(GLFW_RESIZABLE, GL_TRUE);

	GLFWwindow* window = glfwCreateWindow(screen_width, screen_height, "Assignment 2", nullptr, nullptr);
	if (!window)
	{
		cout << "GLFW create window failed,invoke glfwCreateWindow()......Error line:" << __FILE__ << "......Error line:" << __LINE__ << endl;
		glfwTerminate();
		exit(EXIT_FAILURE);
	}

	/*�˺���ʹ�����߳��ϵ�ָ�����ڵ� OpenGL �� OpenGL ES �����ĳ�Ϊ��ǰ�����ġ�
	  һ��ֻ���ڵ����߳���ʹ�����ĳ�Ϊ��ǰ�����ģ�����ÿ���߳�һ��ֻ����һ����ǰ�����ġ�
	  ���߳�֮���ƶ�������ʱ��������ʹ���ھ��߳��ϱ�Ϊ�ǵ�ǰ״̬��Ȼ���������߳��ϱ�Ϊ��ǰ״̬��
	*/
	glfwMakeContextCurrent(window);
	glfwSetKeyCallback(window, key_callback);
	glfwSetCursorPosCallback(window, cursor_position_callback);
	glfwSetScrollCallback(window, scroll_callback);
	glfwSetMouseButtonCallback(window, mouse_button_callback);
	glfwSetWindowSizeCallback(window, window_size_callback);

	//�������ģʽ
	//glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);

	int glewState = glewInit();
	if (GLEW_OK != glewState)
	{
		cout << "GLEW initialize failed,invoke glewInit()......Error file:" << __FILE__ << "......Error line:" << __LINE__ << endl;
		glfwTerminate();
		exit(EXIT_FAILURE);
	}

	/*�˺������õ�ǰ OpenGL �� OpenGL ES �����ĵĽ�����������ӵ���glfwSwapBuffers��ʼ�ȴ�����Ļ���´�����
	  Ȼ���ٽ��������������ء�����ʱ��Ϊ��ֱͬ������ֱ��ɨͬ�����vsync��
	  ֧��WGL_EXT_swap_control_tear��GLX_EXT_swap_control_tear��չ��������Ҳ���ܸ����������������������������������
	  ��ʹ֡�����е���������ʹ��glfwExtensionSupported�����Щ��չ��
	  �����ı����ڵ����߳��������µġ���û�е�ǰ�����ĵ�����µ��ô˺���������GLFW_NO_CURRENT_CONTEXT����
	  �˹��ܲ������� Vulkan�������ʹ�� Vulkan ������Ⱦ�����Ϊ�鿴�������ĵ�ǰģʽ��
	*/
	glfwSwapInterval(1);

	printf("%s\n", glGetString(GL_SHADING_LANGUAGE_VERSION));//��ʼ��ʼ������
	const GLubyte* renderer = glGetString(GL_RENDERER);
	const GLubyte* vendor = glGetString(GL_VENDOR);
	const GLubyte* version = glGetString(GL_VERSION);
	const GLubyte* glslVersion = glGetString(GL_SHADING_LANGUAGE_VERSION);
	GLint major, minor;
	glGetIntegerv(GL_MAJOR_VERSION, &major);
	glGetIntegerv(GL_MINOR_VERSION, &minor);
	printf("GL Vendor : %s\n", vendor);
	printf("GL Renderer : %s\n", renderer);
	printf("GL Version (string) : %s\n", version);
	printf("GL Version (integer) : %d.%d\n", major, minor);
	printf("GLSL Version : %s\n", glslVersion);

	glGetError(); // Debug GLEW bug fix


	/*��Ϊ����Ҫʹ��OpenGL 4.6���������ǰ�GLFW_CONTEXT_VERSION_MAJOR��GLFW_CONTEXT_VERSION_MINOR��Ӧ��hint������Ϊ4��6��
	��Ϊ����Ҫʹ��OpenGL����ģʽ�����������ᵽ���ࣩ���������ǰ�GLFW_OPENGL_PROFILE��Ӧ��hint����ΪGLFW_OPENGL_CORE_PROFILE��
	��ʾʹ��OpenGL����ģʽ����󣬰�GLFW_RESIZABLE��Ӧ��hint����ΪGLFW_FALSE����ʾ���ڲ������û�������С��
	֮��������������Ϊ��������û�������С����С�����仯�󣬴��ڵĻ�������Ĭ�ϲ��䣨��Ȼ��ԭ�����ڵ����򣩣�
	Ҳ����˵�����ϻ��Ƶ�ͼ��Ĵ�С��λ�ò��ᷢ���ı䡣Ϊ�˱������������������Ǿͼ򵥵ز����û��������ڴ�С
	����ȻҲ�и��õķ�����������GLFW����һ�����ڴ�С�Ļص��������������Ƚϼ򵥣���*/



	/*Ĭ������£��������ܿ��ǣ����ж�����ɫ�������ԣ�Attribute���������ǹرյģ�
	  ��ζ����������ɫ�����ǲ��ɼ��ģ����������Ѿ��ϴ���GPU����glEnableVertexAttribArray����ָ�����ԣ�
	  �ſ��ڶ�����ɫ���з����𶥵���������ݡ�glVertexAttribPointer��VBOֻ�ǽ���CPU��GPU֮����߼����ӣ�
	  �Ӷ�ʵ����CPU�����ϴ���GPU�����ǣ�������GPU���Ƿ�ɼ���������ɫ���ܷ��ȡ�����ݣ����Ƿ������˶�Ӧ�����Ծ�����
	  �����glEnableVertexAttribArray�Ĺ��ܣ���������ɫ����ȡGPU���������ˣ����ݡ�
	 */
	int nrAttributes;
	glGetIntegerv(GL_MAX_VERTEX_ATTRIBS, &nrAttributes);
	std::cout << "Maximum nr of vertex attributes supported: " << nrAttributes << std::endl;

	initializedGL(window);

	while (!glfwWindowShouldClose(window))
	{
		/* Render here */
		paintGL(window, (float)glfwGetTime());

		glfwSwapBuffers(window);
		
		glfwPollEvents();
	}

	glfwDestroyWindow(window);
	glfwTerminate();
	exit(EXIT_SUCCESS);

	return 0;
}