#ifndef COMMON_H
#define COMMON_H

#define GDEFAULT_GRIDSIZE	7
#define GMIN_GRIDSIZE		3
#define GMAX_GRIDSIZE		14	// TODO set this value based on min(WindowsWidth / tileWidth, WindowsHeight / tileHeight)

// TODO Ideally should be returned by a call back from "option" scene and send to "HelloWorldScene"
//		OR should be in a singleton class and reached by it
//		OR should be written into a file and read from that file when it is needed
//		BUT since it is the only variable that will be controlled, this is the cheapest solution with the lowest opportunity cost
extern int GFIELD_SIZE;

#define GPIXEL_SPACE	1

#define GGEM_NUMBER	5

#define GGEM_SPACE_FROMUP	3
#define GGEM_SPACE_FROMLEFT	6

#define GTILE_FILENAME	"jewel_"
#define GTILE_EXTENSION	".png"

// BECAUSE DEPLOYMENT FOR ANDROID GIVES to_string is not a member of std...
#include <string>
#include <sstream>

using namespace std;

namespace std
{
	template < typename T > std::string to_string(const T& n)
	{
		std::ostringstream stm;
		stm << n;
		return stm.str();
	}
}


#endif
