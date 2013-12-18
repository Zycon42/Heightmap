/**
* @file HeightMapLoader.cpp
*
* @author Jan Dušek <xdusek17@stud.fit.vutbr.cz>
* @date 2013
*/

#include "HeightMapLoader.h"
#include "Application.h"

#include <SDL.h>
#include <SDL_image.h>

#include <cstdint>

const char* HeightMapLoader::DEFAULT_DIR = "../data/heightmaps/";

HeightMapLoader::HeightMapLoader() : m_dir(DEFAULT_DIR) {
}

void HeightMapLoader::setCustomDirectory(const char* path) {
	m_dir = path;
}

void HeightMapLoader::setDefaultDirectory() {
	m_dir = DEFAULT_DIR;
}

HeightMap HeightMapLoader::load(const char* name) {
	SDL_Surface* rawImg = IMG_Load((m_dir + name).c_str());
	if (!rawImg)
		throw SDLException("IMG_Load failed");

	// use unique_ptr for exception safety
	std::unique_ptr<SDL_Surface, void (*)(SDL_Surface*)> image{ rawImg, SDL_FreeSurface };

	if (SDL_PIXELTYPE(image->format->format) != SDL_PIXELTYPE_INDEX8)
		throw Exception("Heightmap must be 8bit grayscale image!");

	HeightMap map = { image->w, image->h };

	auto bpp = image->format->BytesPerPixel;
	uint8_t* pixel = nullptr;
	for (int y = 0; y < image->h; y++) {
		for (int x = 0; x < image->w; x++) {
			// pointer to pixel
			pixel = reinterpret_cast<uint8_t*>(image->pixels) + y * image->pitch + x * bpp;
			map.heights.push_back(*pixel);
		}
	}

	return map;
}
