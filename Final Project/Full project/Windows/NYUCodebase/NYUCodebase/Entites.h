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

	int collision = 0; // !collision actually

	//P1_Entity(float x, float y, float height, float width, unsigned int textureID);
	P1_Entity();


	void check_collisions();
	void update();

};