#include "GameConfig.h"
using namespace std;
using namespace glm;

class Settings {
	public:
		int exit, width, height;
		bool floor;
		Settings() {	
			exit=0;
			width=800;
			height=600;
			floor=false;
		}
};

Settings get_settings(int ac, char* av[]) {	
	Settings set;
	namespace po=boost::program_options;
	int width,height;
	string geometry;
	po::options_description desc("Allowed options");
	desc.add_options()
		("help,h", "produce help message")
		("width,w", po::value<int>(&width)->default_value(800), "set width of display, default: 800")
		("height,h", po::value<int>(&height)->default_value(600), "set height of display, default: 600")
		("geometry,g", po::value<string>(&geometry), "set width and height simultaneously, example: 800x600");
	po::variables_map vm;
	po::store(po::parse_command_line(ac,av,desc),vm);
	po::notify(vm);
	if(vm.count("help")) {
		cout << "Game v" << Game_VERSION_MAJOR << "." << Game_VERSION_MINOR << endl;
		cout << desc << flush;
		set.exit=1;
		return set;
	}
	
	if(vm.count("floor")) {
		set.floor=true;	
	}
	if(vm.count("width")) {
		set.width=width;	
	}
	if(vm.count("height")) {
		set.height=height;	
	}
	if(vm.count("geometry")) {
		int pos=geometry.find("x");
		width=atoi(geometry.substr(0,pos).c_str());
		height=atoi(geometry.substr(pos+1).c_str());
		set.width=width;
		set.height=height;
	}
	return set;
}

GLFWwindow* initialiseWindow(int w, int h) {
	glfwInit();
	glfwWindowHint(GLFW_SAMPLES, 4); // 4x antialiasing
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3); // We want OpenGL 3.3
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE); //We don't want the old OpenGL
	GLFWwindow* window= glfwCreateWindow(w,h,Game_TITLE, NULL, NULL);
	glfwMakeContextCurrent(window);
	glewExperimental=true;
	glewInit();
	glfwSetInputMode(window, GLFW_STICKY_KEYS, GL_TRUE);
	return window;
}

float* myPerspective(float fov, float ratio, float nearClip, float farClip) {
	float* p = new float[12];
	for(int i=0; i<12; i++) {
		p[i]=0;
	}
	p[0]=ratio/tan(fov);
	p[5]=1/tan(fov);
	p[10]=(nearClip+farClip)/(nearClip-farClip);
	p[11]=-1;
	return p;
}

/*
void changeSize(int w, int h) {
	float r=h==0?1.0*w:1.0*w/h;
	float farClip=100;
	float nearClip=1;
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glViewport(0,0,w,h);
	gluPerspective(45,r,nearClip,farClip);
	glMatrixMode(GL_MODELVIEW);
}
void renderScene() {
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glBegin(GL_TRIANGLES);
		glVertex3f(-0.5,0.0,0.0);
		glVertex3f(0.0,1.0,0.0);
		glVertex3f(0.5,0.0,0.0);
	glEnd();
	glutSwapBuffers();
}
*/


void drawTriangle() {
	static const GLfloat g_vertex_buffer_data[] = {
		 -1.0f, -1.0f, 0.0f,
		 1.0f, -1.0f, 0.0f,
		 0.0f,  1.0f, 0.0f,
	};
	GLuint vertexbuffer;
	glGenBuffers(1, &vertexbuffer);
	glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer);
	glBufferData(GL_ARRAY_BUFFER, sizeof(g_vertex_buffer_data), g_vertex_buffer_data, GL_STATIC_DRAW);
	glEnableVertexAttribArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer);
	glVertexAttribPointer(
		 0,                  // attribute 0. No particular reason for 0, but must match the layout in the shader.
		 3,                  // size
		 GL_FLOAT,           // type
		 GL_FALSE,           // normalized?
		 0,                  // stride
		 (void*)0            // array buffer offset
	);
	glDrawArrays(GL_TRIANGLES, 0, 3); // Starting from vertex 0; 3 vertices total -> 1 triangle
	glDisableVertexAttribArray(0);
}

int main(int argc, char* argv[]) {
	Settings settings=get_settings(argc,argv);
	if(settings.exit != 0) { return settings.exit; }
	GLFWwindow* window=initialiseWindow(settings.width, settings.height);

	GLuint VertexArrayID;
	glGenVertexArrays(1, &VertexArrayID);
	glBindVertexArray(VertexArrayID);
	GLuint programID = LoadShaders("shaders/simplevert.sdr", "shaders/simplefrag.sdr");
	do{
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		glUseProgram(programID);
		drawTriangle();
		glfwSwapBuffers(window);
		glfwPollEvents();
	} while( glfwGetKey(window, GLFW_KEY_ESCAPE ) != GLFW_PRESS && glfwWindowShouldClose(window) == 0 );

	return 0;
}
