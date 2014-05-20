#include "GameConfig.h"
using namespace std;

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
	po::options_description desc("Allowed options");
	desc.add_options()
		("help,h", "produce help message")
		("width,w", "set width of display");
		("height,h", "set height of display");
		("floor,f", "floor it");
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
		set.width=vm["width"].as<int>();	
	}
	if(vm.count("height")) {
		set.height=vm["width"].as<int>();	
	}
	return set;
}

int initialiseWindow(int* argc, char* argv[], int w, int h) {
	glutInit(argc,argv);
	glutInitWindowPosition(-1,-1); // allow wm to decide where window opens
	glutInitWindowSize(w,h);
	glutInitDisplayMode(GLUT_DEPTH | GLUT_DOUBLE | GLUT_RGB);
	glutCreateWindow(Game_TITLE);
	return 0;
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

int main(int argc, char* argv[]) {
	Settings settings=get_settings(argc,argv);
	if(settings.exit != 0) { return settings.exit; }

	initialiseWindow(&argc, argv, settings.width, settings.height);
	glutDisplayFunc(renderScene);
	glutReshapeFunc(changeSize);
	glutMainLoop();

	double input=atof(argv[1]), output=sqrt(input);

	if(settings.floor) { output=floor(output); }
	cout << "sqrt: " << output << endl;
	return 0;
}
