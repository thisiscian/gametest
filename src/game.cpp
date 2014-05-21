#include "GameConfig.h"
using namespace std;
using namespace glm;

class Settings {
	public:
		int exit, width, height, maxWidth, maxHeight;
		bool floor;
		Settings() {	
			exit=0;
			width=800;
			height=600;
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
	glClearColor(0.0f,0.0f,0.0f,0.0f);
	return window;
}

void square(vec2 a, float s, vec3 col) {
	GLfloat g_vertex_buffer_data[] = {
		a[0]+s/2, a[1]+s/2, 0,
			a[0]-s/2, a[1]+s/2, 0,
			a[0]+s/2, a[1]-s/2, 0,
		a[0]-s/2, a[1]-s/2, 0,
			a[0]-s/2, a[1]+s/2, 0,
			a[0]+s/2, a[1]-s/2, 0
	};
	GLfloat g_color_buffer_data[] = {
		col[0],col[1],col[2],
			col[0],col[1],col[2],
			col[0],col[1],col[2],
		col[0],col[1],col[2],
			col[0],col[1],col[2],
			col[0],col[1],col[2]
	};
	GLuint vertexbuffer;
	glGenBuffers(1, &vertexbuffer);
	glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer);
	glBufferData(GL_ARRAY_BUFFER, sizeof(g_vertex_buffer_data), g_vertex_buffer_data, GL_STATIC_DRAW);

  GLuint colorbuffer;
  glGenBuffers(1, &colorbuffer);
  glBindBuffer(GL_ARRAY_BUFFER, colorbuffer);
  glBufferData(GL_ARRAY_BUFFER, sizeof(g_color_buffer_data), g_color_buffer_data, GL_STATIC_DRAW);

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
	glEnableVertexAttribArray(1);
	glBindBuffer(GL_ARRAY_BUFFER, colorbuffer);
	glVertexAttribPointer(
		1,                                // attribute. No particular reason for 1, but must match the layout in the shader.
		3,                                // size
		GL_FLOAT,                         // type
		GL_FALSE,                         // normalized?
		0,                                // stride
		(void*)0                          // array buffer offset
	);

	glDrawArrays(GL_TRIANGLES, 0, 6); // Starting from vertex 0; 3 vertices total -> 1 triangle
	glDisableVertexAttribArray(0);
}

void drawTriangle(vec2 a, vec2 b, vec2 c) {
	GLfloat g_vertex_buffer_data[] = {
		a[0], a[1], 0,
		b[0], b[1], 0, 
		c[0], c[1], 0
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

	GLuint programID = LoadShaders("shaders/simplevert.sdr", "shaders/simplefrag.sdr");
	GLuint VertexArrayID;
	GLuint MatrixID=glGetUniformLocation(programID, "MVP");
	GLFWmonitor* monitor=glfwGetPrimaryMonitor();
	int xOffset, yOffset;
	glfwGetMonitorPos(monitor, &xOffset, &yOffset);
	const GLFWvidmode* vid=glfwGetVideoMode(monitor);
	settings.maxWidth=vid->width;
	settings.maxHeight=vid->height;



	mat4 Projection = perspective(0.25f*pi<float>(), (float) settings.width/settings.height, 0.1f, 100.0f);
	mat4 View = lookAt(vec3(0,0,1), vec3(0,0,0), vec3(0,1,0));
	mat4 Model = mat4(1.0f);
	mat4 MVP = Projection*View*Model;
	glGenVertexArrays(1, &VertexArrayID);
	glBindVertexArray(VertexArrayID);
	do{
		int x,y;
		glfwGetWindowSize(window, &(settings.width), &(settings.height));
		glfwGetWindowPos(window, &x, &y);
		x-=xOffset;
		y-=yOffset;
		Projection = perspective(0.25f*pi<float>(), (float) settings.width/settings.height, 0.1f, 100.0f);
		//MVP= Projection*View*Model;
		MVP = Model;
	
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		glUseProgram(programID);
		glUniformMatrix4fv(MatrixID, 1, GL_FALSE, &MVP[0][0]);
		//vec2 pos(4.0f*x/settings.maxWidth-1, 4.0f*y/settings.maxHeight-1);
		vec2 pos(2.0f*x/(settings.maxWidth-settings.width)-1,2.0f*y/(settings.maxHeight-settings.height)-1);


		cout << x << "|" << settings.maxWidth << ":" << y << "|" << settings.maxHeight << "\t" << pos[0] << "x" << pos[1] << endl;
		square(pos,0.05, vec3(0.2,0.5,0.1));
		glfwSwapBuffers(window);
		glfwPollEvents();
	} while( glfwGetKey(window, GLFW_KEY_ESCAPE ) != GLFW_PRESS && glfwWindowShouldClose(window) == 0 );
	return 0;
}
