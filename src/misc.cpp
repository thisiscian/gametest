#include "misc.h"

float px2a(int px, int size) {
	return px/(size/2.0f) - 1; 
}

duo<float> px2a(duo<int> px, duo<int> size) {
	duo<float> a;
	a.x= px.x/(size.x/2.0f)-1;
	a.y= 1-px.y/(size.y/2.0f);
	return a;
}


