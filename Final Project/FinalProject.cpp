#include "FinalProject.h"

// To do:   P2 Entity (derive it from p1)
//			bullets entity
//			sprites
//			background + music
//			"tweening"
// still have time: particle effects, laser, blocks, konami code


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



Game_Entity::Game_Entity() : done(false), state(main_menu) {
	float texCoords[] = { 0.0f, 0.0f, 0.0f, 1.0f, 1.0f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f, 1.0f, 0.0f };
	float vert[] = { -1.0f, 1.0f, -1.0f, -1.0f, 1.0f, 1.0f, -1.0f, -1.0f, 1.0f, -1.0f, 1.0f, 1.0f };
}

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

	if (player1.active == true)
		player1.update();

	switch (state)
	{
	case main_menu:
		if (keys[SDL_SCANCODE_SPACE])
		{
			state = directions;
		}
		renderP1();
		render_main_menu();
		break;
	case directions:
		render_directions();
		if (keys[SDL_SCANCODE_A])
		{
			state = play;
			player1.active = true;
		}
		break;
	case play:
		render_play();
		if (keys[SDL_SCANCODE_ESCAPE])
		{
			state = pause;
			player1.active = false;
		}
		break;
	case pause:
		render_pause();
		if (keys[SDL_SCANCODE_RETURN])
		{
			state = play;
			player1.active = true;
		}
		else if (keys[SDL_SCANCODE_BACKSPACE])
		{
			state = main_menu;
		}
		break;
	case game_over:
		render_game_over();
		player1.active = false;
		break;
	}
}


void Game_Entity::render_main_menu(){
	program->setModelMatrix(modelMatrix);
	modelMatrix.identity();
	modelMatrix.Translate(-.25, 1.5, 0);
	DrawText(fontID, "Title Screen", .5, -.25, program);
	
	//write title screen
	//space for directions
}


void Game_Entity::render_directions(){
	program->setModelMatrix(modelMatrix);
	modelMatrix.identity();
	modelMatrix.Translate(-.25, 1.5, 0);
	DrawText(fontID, "Directions", .5, -.25, program);

	//write directions here
	//w for 1 player I for 2 player

}


void Game_Entity::render_play(){

	/*program->setModelMatrix(modelMatrix);
	modelMatrix.identity();
	modelMatrix.Translate(-.25, 1.5, 0);
	DrawText(fontID, "Play", .5, -.25, program);*/
	renderP1();

}


void Game_Entity::render_pause(){
	program->setModelMatrix(modelMatrix);
	modelMatrix.identity();
	modelMatrix.Translate(-.25, 1.5, 0);
	DrawText(fontID, "Pause", .5, -.25, program);
}

void Game_Entity::render_game_over(){
	program->setModelMatrix(modelMatrix);
	modelMatrix.identity();
	modelMatrix.Translate(-.25, 1.5, 0);
	DrawText(fontID, "Game Over", .5, -.25, program);
}


void Game_Entity::renderP1(){
	glUseProgram(program->programID);
	glVertexAttribPointer(program->positionAttribute, 2, GL_FLOAT, false, 0, vert);
	glVertexAttribPointer(program->texCoordAttribute, 2, GL_FLOAT, false, 0, texCoords);
	glBindTexture(GL_TEXTURE_2D, player1.textureID);
	glEnableVertexAttribArray(program->positionAttribute);
	glEnableVertexAttribArray(program->texCoordAttribute);
	program->setModelMatrix(modelMatrix);
	glDrawArrays(GL_TRIANGLES, 0, 6);
	modelMatrix.identity();
	//modelMatrix.Scale(.25, .25, 1.0);
	modelMatrix.Translate(player1.position_x, player1.position_y, 0.0f);
	glDisableVertexAttribArray(program->positionAttribute);
	glDisableVertexAttribArray(program->texCoordAttribute);
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
	player1.textureID = LoadTexture("Megaman.png");

	program = new ShaderProgram(RESOURCE_FOLDER"vertex_textured.glsl", RESOURCE_FOLDER"fragment_textured.glsl");
	while (!done){
		process_events();
		update(); //update, then render
 		SDL_GL_SwapWindow(displayWindow);
	}
	SDL_Quit();
}