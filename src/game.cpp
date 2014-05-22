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

class tri {
	public:
	vec2 v[3];
	tri(vec2 a, vec2 b, vec2 c) {v[0]=a; v[1]=b; v[2]=c;}
};

class Drawer {
	private:
		GLuint vertexBuffer, colourBuffer;
		GLuint programID, vertexArrayID;
		GLfloat *vertexData, *colourData;
		int promise, count;
		GLFWwindow* window;
	public:
		Drawer(GLFWwindow* w, int p) {
			promise=p;
			window=w;
			glGenVertexArrays(1, &vertexArrayID);
			glBindVertexArray(vertexArrayID);
			programID=LoadShaders("shaders/simplevert.sdr", "shaders/simplefrag.sdr");

			vertexData=new GLfloat[promise*9];
			glGenBuffers(1, &vertexBuffer);
			glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer);
			glBufferData(GL_ARRAY_BUFFER, sizeof(vertexData), vertexData, GL_STATIC_DRAW);

			colourData=new GLfloat[promise*9];
			glGenBuffers(1, &colourBuffer);
			glBindBuffer(GL_ARRAY_BUFFER, colourBuffer);
			glBufferData(GL_ARRAY_BUFFER, sizeof(colourData), colourData, GL_STATIC_DRAW);
		}
		
		void addTriangle(vec2 a, vec2 b, vec2 c, vec3 col) {
			if(count >= promise) { return; }
			vertexData[9*count+0]=a[0]; vertexData[9*count+1]=a[1]; vertexData[9*count+2]=0;
			vertexData[9*count+3]=b[0]; vertexData[9*count+4]=b[1]; vertexData[9*count+5]=0;
			vertexData[9*count+6]=c[0]; vertexData[9*count+7]=c[1]; vertexData[9*count+8]=0;
			for(int i=0; i<3; i++) {
				for(int j=0; j<3; j++) {
					colourData[9*count+3*i+j]=col[j];
					cout << 9*count+3*i+j << ": " << vertexData[9*count+3*i+j] << "," << colourData[9*count+3*i+j] << endl;
				}
			}
			count++;
		}
		void addSquare(vec2 C, float w, float h, vec3 col) {
			vec2 a= vec2(C[0]+w/2, C[1]+h/2);
			vec2 b= vec2(C[0]+w/2, C[1]-h/2);
			vec2 c= vec2(C[0]-w/2, C[1]-h/2);
			vec2 d= vec2(C[0]-w/2, C[1]+h/2);
			addTriangle(a,d,b,col);
			addTriangle(a,c,d,col);
		}
		void draw() {
			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
			glUseProgram(programID);

			glEnableVertexAttribArray(0);
			glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer);
			glVertexAttribPointer( 0, 3, GL_FLOAT, GL_FALSE, 0, (void*) 0 );

			glEnableVertexAttribArray(1);
			glBindBuffer(GL_ARRAY_BUFFER, colourBuffer);
			glVertexAttribPointer( 1, 3, GL_FLOAT, GL_FALSE, 0, (void*) 0 );
	
			glDrawArrays(GL_TRIANGLES, 0, count*3);

			glDisableVertexAttribArray(0);
			glDisableVertexAttribArray(1);

			glfwSwapBuffers(window);
			glfwPollEvents();
			count=0;
		}
		bool escape() {
			return glfwGetKey(window, GLFW_KEY_ESCAPE ) != GLFW_PRESS && glfwWindowShouldClose(window) == 0;
		};
};

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
	Drawer d(s->window, 100);
	cout << "initiating main loop" << endl;
	do{
		s->update();

		duo<float> posRelW = px2a(s->mSize/2-s->wPos, s->wSize);
		vec2 pos(posRelW.x, posRelW.y);
		duo<float> crossHair=px2a(s->wSize/2-20, s->wSize);

//		d.addSquare(pos,1.0f/s->wSize.x, 2.0f*s->mSize.y/s->wSize.y, vec3(1,1,1));
//		d.addSquare(pos,2.0f*s->mSize.x/s->wSize.x, 1.0f/s->wSize.y, vec3(1,1,1));
		d.addSquare(pos,30.0f/s->wSize.x, 30.0f/s->wSize.y, vec3(0.5,0.1,0.1));

//		d.addSquare(vec2(0,crossHair.y), 3.0f/s->wSize.x, 30.0f/s->wSize.y, vec3(0,1,0));
//		d.addSquare(vec2(0,-crossHair.y), 3.0f/s->wSize.x, 30.0f/s->wSize.y, vec3(0,1,0));
//		d.addSquare(vec2(crossHair.x,0), 30.f/s->wSize.x, 3.f/s->wSize.y, vec3(0,1,0));
//		d.addSquare(vec2(-crossHair.x,0), 30.f/s->wSize.x, 3.f/s->wSize.y, vec3(0,1,0));

		d.draw();
	} while(d.escape());
	cout << "exiting... " << endl;
	return 0;
}
