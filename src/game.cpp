#include "GameConfig.h"
using namespace std;

class Settings {
	public:
		int exit;
		bool floor;
		Settings() {	
			exit=0;
			floor=false;
		}
};

Settings get_settings(int ac, char* av[]) {	
	Settings set;
	namespace po=boost::program_options;
	po::options_description desc("Allowed options");
	desc.add_options()
		("help,h", "produce help message")
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
		return set;
	}
	return set;
}


int main(int argc, char* argv[]) {
	Settings set=get_settings(argc,argv);
	if(set.exit != 0) { return set.exit; }
	double input=atof(argv[1]), output=sqrt(input);
	if(set.floor) { output=floor(output); }
	cout << "sqrt: " << output << endl;
	return 0;
}
