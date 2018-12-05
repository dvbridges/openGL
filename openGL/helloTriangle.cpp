#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <iostream>

void generateVBO(GLFWwindow* window, float * vertices);
void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void processInput(GLFWwindow *window);
void draw(GLFWwindow* window, unsigned int * shaderProgram, unsigned int * VAO);
void flip(GLFWwindow* window);
void initializeWin();
int testWindowCreation(GLFWwindow* window);
int initGLAD();
unsigned int compileShader(const char * vertexShaderSource, char shaderType[10]);
void checkShaderSuccess(unsigned int * shader);
unsigned int shaderProgramBuild(unsigned int * vertexShader, unsigned int * fragShader);
void linkVertexAttribute(unsigned int * shaderProgram, unsigned int * VBO, unsigned int * VAO);

// settings
const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 600;

// Vertex objects
unsigned int VAO;
unsigned int VBO;

// Shaders
char vertexType[10] = "vertex";
char fragType[10] = "fragment";
unsigned int vertexShader;
unsigned int fragShader;
unsigned int shaderProgram;

// Create vertex array
float vertices[] = {
	-0.5f, -0.5f, 0.0f,
	 0.5f, -0.5f, 0.0f,
	 0.0f,  0.5f, 0.0f
};

// Write shader 

const char * vertexShaderSource = "#version 450 core\n"
"layout (location = 0) in vec3 aPos;\n"
"void main()\n"
"{\n"
"	gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0f);\n"
"}\n\0";

const char * fragShaderSource = "#version 330 core\n"
"out vec4 FragColor;\n"
"void main()\n"
"{\n"
"	FragColor = vec4(1.0f, 0.5f, 0.2f, 1.0f);\n"
"}\n\0";
	
int main()
{
	initializeWin();
	// glfw window creation
	GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "LearnOpenGL", NULL, NULL);
	testWindowCreation(window);
	glfwMakeContextCurrent(window);
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
	initGLAD();	 // Get OPENGL function pointers

	// Generate vertex buffer object
	generateVBO(window, vertices);
	
	vertexShader = compileShader(vertexShaderSource, vertexType);
	fragShader = compileShader(fragShaderSource, fragType);
	shaderProgram = shaderProgramBuild(&vertexShader, &fragShader);
	linkVertexAttribute(&shaderProgram, &VBO, &VAO);
	draw(window, &shaderProgram, &VAO);

	return 0;
}

void initializeWin()
{
	// glfw: initialize and configure
	// ------------------------------
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 5);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

#ifdef __APPLE__
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE); // uncomment this statement to fix compilation on OS X
#endif
}

int testWindowCreation(GLFWwindow* window)
{
	if (window == NULL)
	{
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
		return -1;
	}
}

int initGLAD()
{
	// glad: load all OpenGL function pointers
	// ---------------------------------------
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		std::cout << "Failed to initialize GLAD" << std::endl;
		return -1;
	}
}

void generateVBO(GLFWwindow* window, float * vertices)
{
	// Create vertex buffer object
	glGenBuffers(1, &VBO);  // Generate vertex buffer object
	glBindBuffer(GL_ARRAY_BUFFER, VBO);  // Bind VBO to buffer type target
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);  // copy vertex data to buffers memory
}

void processInput(GLFWwindow *window)
// process all input: query GLFW whether relevant keys are pressed/released this frame and react accordingly
// ---------------------------------------------------------------------------------------------------------
{
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		glfwSetWindowShouldClose(window, true);
}

// glfw: whenever the window size changed (by OS or user resize) this callback function executes
// ---------------------------------------------------------------------------------------------
void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
	// make sure the viewport matches the new window dimensions; note that width and 
	// height will be significantly larger than specified on retina displays.
	glViewport(0, 0, width, height);
}

void draw(GLFWwindow* window, unsigned int * shaderProgram, unsigned int * VAO)
{
	// render loop
	// -----------
	while (!glfwWindowShouldClose(window))
	{
		// input
		// -----
		processInput(window);

		// render
		// ------
		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);
		glUseProgram(*shaderProgram);
		glBindVertexArray(*VAO);
		glDrawArrays(GL_TRIANGLES, 0, 3);
		
		// swap buffers
		// ------------
		flip(window);  
	}

	// glfw: terminate, clearing all previously allocated GLFW resources.
	// ------------------------------------------------------------------
	glfwTerminate();
}

void flip(GLFWwindow* window)
{
	// glfw: swap buffers and poll IO events (keys pressed/released, mouse moved etc.)
		// -------------------------------------------------------------------------------
	glfwSwapBuffers(window);
	glfwPollEvents();
}

unsigned int compileShader(const char * shaderSource, char shaderType[])
{
	
	unsigned int shader;

	if (shaderType == vertexType) {
		shader = glCreateShader(GL_VERTEX_SHADER);
	}
	else if (shaderType == fragType) {
		shader = glCreateShader(GL_FRAGMENT_SHADER);
	}	

	glShaderSource(shader, 1, &shaderSource, NULL);
	glCompileShader(shader);

	// Check successful creation of vertex shader object
	checkShaderSuccess(&shader);
	return shader;
}

void checkShaderSuccess(unsigned int *shader)
{
	// Check successful creation of vertex shader object
	int  success;
	char infoLog[512];
	glGetShaderiv(*shader, GL_COMPILE_STATUS, &success);
	if (!success)
	{
		glGetShaderInfoLog(*shader, 512, NULL, infoLog);
		std::cout << "ERROR::SHADER::" << *shader << "::COMPILATION_FAILED\n" << infoLog << std::endl;
	}
}

unsigned int shaderProgramBuild(unsigned int * vertexShader, unsigned int * fragShader)
{
	
	shaderProgram = glCreateProgram();
	glAttachShader(shaderProgram, *vertexShader);
	glAttachShader(shaderProgram, *fragShader);
	glLinkProgram(shaderProgram);

	int  success;
	char infoLog[512];
	glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
	if (!success) {
		glGetProgramInfoLog(shaderProgram, 512, NULL, infoLog);
		
	}
	glUseProgram(shaderProgram);
	glDeleteShader(*vertexShader);
	glDeleteShader(*fragShader);

	return shaderProgram;
}

void linkVertexAttribute(unsigned int * shaderProgram, unsigned int * VBO, unsigned int * VAO)
{	
	
	glGenVertexArrays(1, VAO);

	// 1. bind Vertex Array Object
	glBindVertexArray(*VAO);
	// 2. copy our vertices array in a buffer for OpenGL to use
	glBindBuffer(GL_ARRAY_BUFFER, *VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
	// 3. then set our vertex attributes pointers
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	// 4. draw the object
	glUseProgram(*shaderProgram);
	glBindVertexArray(*VAO);
	
}