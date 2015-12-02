#include "Entites.h"


GLuint LoadTexture(const char *image_path){
	SDL_Surface *surface = IMG_Load(image_path);
	GLuint textureID;
	glGenTextures(1, &textureID);
	glBindTexture(GL_TEXTURE_2D, textureID);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, surface->w, surface->h, 0, GL_RGBA, GL_UNSIGNED_BYTE, surface->pixels);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	SDL_FreeSurface(surface);
	return textureID;
}


//P1_Entity::P1_Entity(float x, float y, float height, float width, unsigned int textureID) :position_x(x), position_y(y), height(height), width(width), textureID(textureID){}

P1_Entity::P1_Entity():active(false){}

void P1_Entity::check_collisions(){
	if (position_x >= 2 || position_x <= -2 || position_y >= 2 || position_y <= -2)
		collision = 0;
	else collision = 1; // !collision
}


void P1_Entity::update(){
	const Uint8 *keys = SDL_GetKeyboardState(NULL);
	if (keys[SDL_SCANCODE_A])
	{
		position_x -= .00025f * collision;
	}
	if (keys[SDL_SCANCODE_D])
	{
		position_x += .00025f * collision;
	}
	if (keys[SDL_SCANCODE_S])
	{
		position_y -= .00025f * collision;
	}
	if (keys[SDL_SCANCODE_W])
	{
		position_y += .00025f * collision;
	}
}


