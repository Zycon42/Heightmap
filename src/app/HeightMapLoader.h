/**
* @file HeightMapLoader.h
*
* @author Jan Dušek <xdusek17@stud.fit.vutbr.cz>
* @date 2013
*/

#ifndef HEIGHT_MAP_LOADER
#define HEIGHT_MAP_LOADER

#include "Terrain.h"

#include <vector>
#include <string>

class HeightMapLoader
{
public:
	HeightMapLoader();

	void setCustomDirectory(const char* path);
	void setDefaultDirectory();

	HeightMap load(const char* name);
private:
	static const char* DEFAULT_DIR;

	std::string m_dir;
};


#endif // !HEIGHT_MAP_LOADER
