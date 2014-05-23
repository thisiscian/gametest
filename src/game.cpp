#include "game.h"
using namespace std;
using namespace glm;

int main(int argc, char* argv[]) {
	Settings s(argc, argv);
	Drawer d(100, s.width, s.height);
	if(s.exit != 0) { return s.exit; }
	cout << "initiating main loop" << endl;
	do{
		duo<float> posRelW = px2a(d.mSize/2-d.wPos, d.wSize);
		vec2 pos(posRelW.x, posRelW.y);
		duo<float> crossHair=px2a(d.wSize/2-20, d.wSize);
//		d.addSquare(pos,30.0f/d.wSize.x, 30.0f/d.wSize.y, vec3(0.5,0.1,0.1));

//		d.addSquare(pos,1.0f/d.wSize.x, 2.0f*d.mSize.y/d.wSize.y, vec3(1,1,1));
//		d.addSquare(pos,2.0f*d.mSize.x/d.wSize.x, 1.0f/d.wSize.y, vec3(1,1,1));
		d.addSquare(vec2(0,crossHair.y), 3.0f/d.wSize.x, 30.0f/d.wSize.y, vec3(0,1,0));
		d.addSquare(vec2(0,-crossHair.y), 3.0f/d.wSize.x, 30.0f/d.wSize.y, vec3(0,1,0));
		d.addSquare(vec2(crossHair.x,0), 30.f/d.wSize.x, 3.f/d.wSize.y, vec3(0,1,0));
		d.addSquare(vec2(-crossHair.x,0), 30.f/d.wSize.x, 3.f/d.wSize.y, vec3(0,1,0));

		d.draw();
	} while(d.escape());
	cout << "exiting... " << endl;
	return 0;
}
