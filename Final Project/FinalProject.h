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

#ifdef _WINDOWS
#define RESOURCE_FOLDER ""
#else
#define RESOURCE_FOLDER "NYUCodebase.app/Contents/Resources/"
#endif

enum states {main_menu, directions, play, pause, game_over};


class Game_Entity
{
public:
	SDL_Window* displayWindow;
	Matrix modelMatrix;
	Matrix projectionMatrix;
	Matrix viewMatrix;
	SDL_Event event;
	ShaderProgram* program;
	bool done;
	states state;
	const Uint8 *keys;
	unsigned int fontID;
	
	Game_Entity();
	void process_events(); //for SDL events
	//void init();
	void update();
	//void render();
	void render_main_menu();
	void render_directions();
	void run();
};