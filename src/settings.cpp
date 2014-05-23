#include "settings.h"
using namespace std;
using namespace boost::program_options;

Settings::Settings(int ac, char* av[]) {
	string geometry;
	exit=0;
	options_description desc("Allowed options");
	desc.add_options()
		("help,h", "produce help message")
		("width,w", value<int>(&(wSize.x))->default_value(300), "set width of display, default: 800")
		("height,h", value<int>(&(wSize.y))->default_value(300), "set height of display, default: 600")
		("geometry,g", value<string>(&geometry), "set width and height simultaneously, example: 800x600");
	variables_map vm;
	store(parse_command_line(ac,av,desc),vm);
	notify(vm);
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

void Settings::update() {
	glfwGetWindowSize(window, (int*) &(wSize.x), (int*) &(wSize.y));
	glfwGetWindowPos(window, (int*) &(wPos.x), (int*) &(wPos.y));
	wPos.x-offset.x;
	wPos.y-offset.y;
}

