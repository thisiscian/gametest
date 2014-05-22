#include "GameConfig.h"
using namespace std;
using namespace glm;

template <class TYPE> class duo {
	public:
		TYPE x, y;
		duo() { x=0.0; y=0.0; }
		duo(TYPE a, TYPE b) { x=a; y=b; }
		template <class TYPE2>
		duo<TYPE2> operator-(TYPE2 a) { return duo<TYPE2>(x-a, y-a); }
		template <class TYPE2>
		duo<TYPE2> operator/(TYPE2 a) { return duo<TYPE2>(x/a, y/a); }
		template <class TYPE2>
		duo<TYPE2> operator/(duo<TYPE2> a) { return duo<TYPE2>(x/a.x, y/a.x); }
		duo<TYPE> operator-(duo a) { return duo(x-a.x, y-a.y); }
};

class Settings {
	public:
		int exit;
		GLFWwindow* window;
		GLFWmonitor* monitor;
		duo<int> wSize, wPos, mSize, offset;
		bool floor;
		Settings(int ac, char*av[] ) {	
			exit=0;
			namespace po=boost::program_options;
			string geometry;
			po::options_description desc("Allowed options");
			desc.add_options()
				("help,h", "produce help message")
				("width,w", po::value<int>(&(wSize.x))->default_value(300), "set width of display, default: 800")
				("height,h", po::value<int>(&(wSize.y))->default_value(300), "set height of display, default: 600")
				("geometry,g", po::value<string>(&geometry), "set width and height simultaneously, example: 800x600");
			po::variables_map vm;
			po::store(po::parse_command_line(ac,av,desc),vm);
			po::notify(vm);
			if(vm.count("help")) {
				cout << "Game v" << Game_VERSION_MAJOR << "." << Game_VERSION_MINOR << endl;
				cout << desc << flush;
				exit=1;
			}
			if(vm.count("geometry")) {
				int pos=geometry.find("x");
				wSize.x=atoi(geometry.substr(0,pos).c_str());
				wSize.y=atoi(geometry.substr(pos+1).c_str());
			}
			glfwInit();
			glfwWindowHint(GLFW_SAMPLES, 4); // 4x antialiasing
			glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3); // We want OpenGL 3.3
			glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
			glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE); //We don't want the old OpenGL
			window= glfwCreateWindow(wSize.x,wSize.y,Game_TITLE, NULL, NULL);
			glfwMakeContextCurrent(window);
			glewExperimental=true;
			glewInit();
			glfwSetInputMode(window, GLFW_STICKY_KEYS, GL_TRUE);
			glClearColor(0.0f,0.0f,0.0f,0.0f);
			monitor=glfwGetPrimaryMonitor();
			glfwGetMonitorPos(monitor, (int*) &(offset.x), (int*) &(offset.y));
			const GLFWvidmode* vid=glfwGetVideoMode(monitor);
			mSize.x=vid->width;
			mSize.y=vid->height;
		}	
		void update() {
			glfwGetWindowSize(window, (int*) &(wSize.x), (int*) &(wSize.y));
			glfwGetWindowPos(window, (int*) &(wPos.x), (int*) &(wPos.y));
			wPos.x-offset.x;
			wPos.y-offset.y;
		}
} *s;

void square(vec2 a, float w, float h, vec3 col) {
	GLfloat g_vertex_buffer_data[] = {
		a[0]+w/2, a[1]+h/2, 0,
			a[0]-w/2, a[1]+h/2, 0,
			a[0]+w/2, a[1]-h/2, 0,
		a[0]-w/2, a[1]-h/2, 0,
			a[0]-w/2, a[1]+h/2, 0,
			a[0]+w/2, a[1]-h/2, 0
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

float px2a(int px, int size) {
	return px/(size/2.0f) - 1; 
}

duo<float> px2a(duo<int> px, duo<int> size) {
	duo<float> a;
	a.x= px.x/(size.x/2.0f)-1;
	a.y= 1-px.y/(size.y/2.0f);
	return a;
}

int main(int argc, char* argv[]) {
	s=new Settings(argc, argv);
	if(s->exit != 0) { return s->exit; }
	GLuint programID = LoadShaders("shaders/simplevert.sdr", "shaders/simplefrag.sdr");
	GLuint VertexArrayID;
	glGenVertexArrays(1, &VertexArrayID);
	glBindVertexArray(VertexArrayID);
	cout << "initiating main loop" << endl;
	do{
		s->update();
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		glUseProgram(programID);

		duo<float> posRelW = px2a(s->mSize/2-s->wPos, s->wSize);
		vec2 pos(posRelW.x, posRelW.y);
		duo<float> crossHair=px2a(s->wSize/2-20, s->wSize);
		
		square(pos,1.0f/s->wSize.x, 2.0f*s->mSize.y/s->wSize.y, vec3(1,1,1));
		square(pos,2.0f*s->mSize.x/s->wSize.x, 1.0f/s->wSize.y, vec3(1,1,1));
		square(pos,30.0f/s->wSize.x, 30.0f/s->wSize.y, vec3(0.5,0.1,0.1));
		square(vec2(0,crossHair.y), 3.0f/s->wSize.x, 30.0f/s->wSize.y, vec3(0,1,0));
		square(vec2(0,-crossHair.y), 3.0f/s->wSize.x, 30.0f/s->wSize.y, vec3(0,1,0));
		square(vec2(crossHair.x,0), 30.f/s->wSize.x, 3.f/s->wSize.y, vec3(0,1,0));
		square(vec2(-crossHair.x,0), 30.f/s->wSize.x, 3.f/s->wSize.y, vec3(0,1,0));
		glfwSwapBuffers(s->window);
		glfwPollEvents();
	} while( glfwGetKey(s->window, GLFW_KEY_ESCAPE ) != GLFW_PRESS && glfwWindowShouldClose(s->window) == 0 );
	cout << "exiting... " << endl;
	return 0;
}
