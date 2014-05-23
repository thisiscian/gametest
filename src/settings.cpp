#include "settings.h"
using namespace std;
using namespace boost::program_options;

Settings::Settings(int ac, char* av[]) {
	string geometry;
	exit=0;
	options_description desc("Allowed options");
	desc.add_options()
		("help,h", "produce help message")
		("width,w", value<int>(&width)->default_value(300), "set width of display, default: 800")
		("height,h", value<int>(&height)->default_value(300), "set height of display, default: 600")
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
		width=atoi(geometry.substr(0,pos).c_str());
		height=atoi(geometry.substr(pos+1).c_str());
	}
}
