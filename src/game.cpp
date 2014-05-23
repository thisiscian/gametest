#include "game.h"
using namespace std;
using namespace glm;

Settings *s;

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
