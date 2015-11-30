#include "FinalProject.h"

void DrawText(int fontTexture, std::string text, float size, float spacing, ShaderProgram *program) {
	float texture_size = 1.0 / 16.0f;
	std::vector<float> vertexData;
	std::vector<float> texCoordData;
	for (int i = 0; i < text.size(); i++) {
		float texture_x = (float)(((int)text[i]) % 16) / 16.0f;
		float texture_y = (float)(((int)text[i]) / 16) / 16.0f;
		vertexData.insert(vertexData.end(), {
			((size + spacing) * i) + (-0.5f * size), 0.5f * size,
			((size + spacing) * i) + (-0.5f * size), -0.5f * size,
			((size + spacing) * i) + (0.5f * size), 0.5f * size,
			((size + spacing) * i) + (0.5f * size), -0.5f * size,
			((size + spacing) * i) + (0.5f * size), 0.5f * size,
			((size + spacing) * i) + (-0.5f * size), -0.5f * size,
		});
		texCoordData.insert(texCoordData.end(), {
			texture_x, texture_y,
			texture_x, texture_y + texture_size,
			texture_x + texture_size, texture_y,
			texture_x + texture_size, texture_y + texture_size,
			texture_x + texture_size, texture_y,
			texture_x, texture_y + texture_size,
		});
	}
	glUseProgram(program->programID);
	glBindTexture(GL_TEXTURE_2D, fontTexture);
	glVertexAttribPointer(program->positionAttribute, 2, GL_FLOAT, false, 0, vertexData.data());
	glEnableVertexAttribArray(program->positionAttribute);
	glVertexAttribPointer(program->texCoordAttribute, 2, GL_FLOAT, false, 0, texCoordData.data());
	glEnableVertexAttribArray(program->texCoordAttribute);
	glDrawArrays(GL_TRIANGLES, 0, text.size() * 6);
	glDisableVertexAttribArray(program->positionAttribute);
	glDisableVertexAttribArray(program->texCoordAttribute);
}


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


Game_Entity::Game_Entity() : done(false), state(main_menu) {}

//void Game_Entity::init(){
//	
//}


void Game_Entity::process_events(){
	while (SDL_PollEvent(&event)){
		if (event.type == SDL_QUIT || event.type == SDL_WINDOWEVENT_CLOSE) {
			done = true;
		}
	}
}


void Game_Entity::update(){
	program->setProjectionMatrix(projectionMatrix);
	program->setViewMatrix(viewMatrix);

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	glUseProgram(program->programID);

	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT);

	keys = SDL_GetKeyboardState(NULL);

	switch (state)
	{
	case main_menu:
		if (keys[SDL_SCANCODE_SPACE])
		{
			state = directions;
		}
		render_main_menu();
		break;
	case directions:
		render_directions();
		if (keys[SDL_SCANCODE_A])
		{
			done = true;
		}
		break;
	case play:
		//render_play();
		break;
	case pause:
		//render_pause();
		break;
	case game_over:
		//render_game_over();
		break;
	}
}


void Game_Entity::render_main_menu(){
	program->setModelMatrix(modelMatrix);
	modelMatrix.identity();
	modelMatrix.Translate(-.25, 1.5, 0);
	DrawText(fontID, "Title Screen", .5, -.25, program);
}


void Game_Entity::render_directions(){
	program->setModelMatrix(modelMatrix);
	modelMatrix.identity();
	modelMatrix.Translate(-.25, 1.5, 0);
	DrawText(fontID, "Directions", .5, -.25, program);

	//write directions here
	//w for 1 player I for 2 player

}

void Game_Entity::run(){
	SDL_Init(SDL_INIT_VIDEO);
	displayWindow = SDL_CreateWindow("Dont Die", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 640, 360, SDL_WINDOW_OPENGL);
	SDL_GLContext context = SDL_GL_CreateContext(displayWindow);
	SDL_GL_MakeCurrent(displayWindow, context);
#ifdef _WINDOWS
	glewInit();
#endif
	glViewport(0, 0, 640, 360);
	projectionMatrix.setOrthoProjection(-3.55f, 3.55f, -2.0f, 2.0f, -1.0f, 1.0f);
	fontID = LoadTexture("font1.png");
	program = new ShaderProgram(RESOURCE_FOLDER"vertex_textured.glsl", RESOURCE_FOLDER"fragment_textured.glsl");
	while (!done){
		process_events();
		update(); //update, then render
		SDL_GL_SwapWindow(displayWindow);
	}
	SDL_Quit();
}