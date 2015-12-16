#include "Entites.h"



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

	float texCoords[12]; //testing
	float vert[12];

	int score_ticks;
	int score;

	P1_Entity player1;
	P2_Entity player2;
	Bullet_Entity bullet;

	std::vector<Bullet_Entity> bullets;
	
	Game_Entity();
	void process_events(); //for SDL events
	void update();
	void render_main_menu();
	void render_directions();
	void render_play();
	void render_pause();
	void render_game_over();

	void renderP1();
	void renderP2();
	void renderBullets();
	void run();

};