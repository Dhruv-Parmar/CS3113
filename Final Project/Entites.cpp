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

P1_Entity::P1_Entity():active(false){}

bool P1_Entity::collide_left(){
	if (position_x - .0025 < -8.0f)
		return true;
	else return false; // set collision if needed
}


bool P1_Entity::collide_right(){
	if (position_x - .0025 > 8.0f)
		return true;
	else return false; // set collision if needed
}

bool P1_Entity::collide_down(){
	if (position_y - .0025 < -6.5f)
		return true;
	else return false; // set collision if needed
}


bool P1_Entity::collide_up(){
	if (position_y - .0025 > 6.5f){
		collision = true;
		return true;
	}
	else {
		collision = false;
		return false;
	}
}


void P1_Entity::check_collisions(){
	if (position_x >= 2.0f || position_x <= -2.0f || position_y >= 2.0f || position_y <= -2.0f)
		collision = 0;
	else collision = 1; // !collision
}


void P1_Entity::update(){
	const Uint8 *keys = SDL_GetKeyboardState(NULL);
	if (keys[SDL_SCANCODE_A])
	{
		if (!collide_left())
			position_x -= .0025f;
	}
	if (keys[SDL_SCANCODE_D])
	{
		if (!collide_right())
			position_x += .0025f;
	}
	if (keys[SDL_SCANCODE_S])
	{
		if (!collide_down())
			position_y -= .0025f;
	}
	if (keys[SDL_SCANCODE_W])
	{
		if (!collide_up())
			position_y += .0025f;
	}
}



// P2 stuff ////////////////////////////////////////////////////////////////////////

P2_Entity::P2_Entity() :active(false), was_active(false){}

bool P2_Entity::collide_left(){
	if (position_x - .0025 < -8.0f)
		return true;
	else return false; // set collision if needed
}


bool P2_Entity::collide_right(){
	if (position_x - .0025 > 8.0f)
		return true;
	else return false; // set collision if needed
}

bool P2_Entity::collide_down(){
	if (position_y - .0025 < -6.5f)
		return true;
	else return false; // set collision if needed
}


bool P2_Entity::collide_up(){
	if (position_y - .0025 > 6.5f){
		collision = true;
		return true;
	}
	else {
		collision = false;
		return false;
	}
}


void P2_Entity::check_collisions(){
	if (position_x >= 2.0f || position_x <= -2.0f || position_y >= 2.0f || position_y <= -2.0f)
		collision = 0;
	else collision = 1; // !collision
}


void P2_Entity::update(){
	const Uint8 *keys = SDL_GetKeyboardState(NULL);
	if (keys[SDL_SCANCODE_J])
	{
		if (!collide_left())
			position_x -= .0025f;
	}
	if (keys[SDL_SCANCODE_L])
	{
		if (!collide_right())
			position_x += .0025f;
	}
	if (keys[SDL_SCANCODE_K])
	{
		if (!collide_down())
			position_y -= .0025f;
	}
	if (keys[SDL_SCANCODE_I])
	{
		if (!collide_up())
			position_y += .0025f;
	}
}


//BULLET /////////////////////////////////////////////////////////////////////////////////

void Bullet_Entity::update(){
	if (direction_x == 0 && direction_y == 0)
		direction_x = 1;
	position_x += speed * direction_x;
	position_y += speed * direction_y;
	
	if (position_x > 15.0f || position_x < -15.0f || position_y < -10.0f || position_y > 10.0f)
		initialize();
}

bool Bullet_Entity::check_collisions(float player_pos_x, float player_pos_y){
	if (position_x - .5 > player_pos_x + .5)
		return false;
	if (position_x + .5 < player_pos_x - .5)
		return false;
	if (position_y - .5 > player_pos_y + .5)
		return false;
	if (position_y + .5 < player_pos_y - .5)
		return false;

	return true;
}

void Bullet_Entity::initialize(){
	//srand(time(NULL));
	speed = ((float(rand()) / float(RAND_MAX)) * (0.0035f + 0.002f)) - 0.002f;
	direction_x = (rand() % 2) - 1; // what if 0?
	direction_y = (rand() % 2) - 1;
	int x_or_y = rand() % 2;
	int side = rand() % 2;

	if (x_or_y == 0) // start on either left or right wall
	{
		if (side == 0)	// left wall
			position_x = -15.0f;
		else			// right wall
			position_x = 15.0f;

		position_y = ((float(rand()) / float(RAND_MAX)) * (20.0f))-10.0f;
	}

	else // start on either top or bottom wall
	{
		if (side == 0)	// bottom wall
			position_y = -10.0f;
		else            // top wall
			position_y = 10.0f;

		position_x = (float(rand()) / float(RAND_MAX)) * (30.0f) -15.0f;
	}
}