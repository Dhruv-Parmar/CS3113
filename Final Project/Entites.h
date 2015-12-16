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
	//float velocity_x;
	//float velocity_y;
	//float acceleration_x;
	//float acceleration_y;
	//float direction_x;
	//float direction_y;

	bool collision = 0; // !collision actually

	bool collide_left();
	bool collide_right(); // set collision if needed
	bool collide_up();
	bool collide_down();

	//P1_Entity(float x, float y, float height, float width, unsigned int textureID);
	P1_Entity();


	void check_collisions();
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
	//float velocity_x;
	//float velocity_y;
	//float acceleration_x;
	//float acceleration_y;
	//float direction_x;
	//float direction_y;

	bool collision = 0; // !collision actually

	bool collide_left();
	bool collide_right(); // set collision if needed
	bool collide_up();
	bool collide_down();

	P2_Entity();


	void check_collisions();
	void update();

};


class Bullet_Entity{
public:
	GLuint textureID;
	float position_x;
	float position_y;
	float speed;
	bool active = false;
	bool alive = false;

	void update(); // check for bullet death in here
	bool check_collisions(float player_pos_x, float player_pos_y);
	void initialize_location();
};