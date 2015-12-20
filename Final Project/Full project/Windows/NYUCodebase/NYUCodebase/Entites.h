#ifdef _WINDOWS
#include <GL/glew.h>
#endif
#include <SDL.h>
#include <SDL_opengl.h>
#include <SDL_image.h>
#include "ShaderProgram.h"
#include "Matrix.h"
#include <SDL_mixer.h>
#include <vector>
#include <string>
#include <time.h>


GLuint LoadTexture(const char *image_path);

class P1_Entity{
public:
	GLuint textureID;
	float position_x;
	float position_y;
	float width;
	float height;
	bool active;

	bool collide_left();
	bool collide_right();
	bool collide_up();
	bool collide_down();

	P1_Entity();

	void update();

};

//probably should have made another base entity class

class P2_Entity{
public:
	GLuint textureID;
	float position_x;
	float position_y;
	float width;
	float height;
	bool active;
	bool was_active;

	bool collide_left();
	bool collide_right(); 
	bool collide_up();
	bool collide_down();

	P2_Entity();

	void update();

};


class Bullet_Entity{
public:
	GLuint textureID;
	float position_x;
	float position_y;
	int direction_x;
	int direction_y;
	//float variation_x;
	//float variation_y;
	//int variation_direction_x;
	//int variation_direction_y;
	float speed;
	bool active = false;
	bool initial_rotate;
	float lastframeticks;
	float angle;
	float rand_rotate_value;
	float rand_rotate_direction;
	float scale;
	//bool alive = false;

	void update();
	bool check_collisions(float player_pos_x, float player_pos_y);
	void initialize();
};

