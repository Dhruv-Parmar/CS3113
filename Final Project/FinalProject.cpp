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



Game_Entity::Game_Entity() : done(false), state(main_menu) {
	float texCoords[] = { 0.0f, 0.0f, 0.0f, 1.0f, 1.0f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f, 1.0f, 0.0f };
	float vert[] = { -1.0f, 1.0f, -1.0f, -1.0f, 1.0f, 1.0f, -1.0f, -1.0f, 1.0f, -1.0f, 1.0f, 1.0f };

	memcpy(this->vert, vert, sizeof(float) * 12);
	memcpy(this->texCoords, texCoords, sizeof(float) * 12);
}


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
	if (player2.active == true)
		player2.update();

	renderBackground();

	switch (state)
	{
	case main_menu:
		if (keys[SDL_SCANCODE_SPACE])
		{
			state = directions;
			player1.position_x = -5.0f;
			player1.position_y = 0.0f;
			player2.position_x = 5.0f;
			player2.position_y = 0.0f;
		}

		for (int i = 0; i < bullets.size(); i++)
			bullets[i].initialize();

		player1.active = false;
		player2.active = false;
		player2.was_active = false;
		score = 0;
		render_main_menu();
		break;
	case directions:
		render_directions();
		if (keys[SDL_SCANCODE_W])
		{
			state = play;
			player1.active = true;
		}
		else if (keys[SDL_SCANCODE_I])
		{
			state = play;
			player1.active = true;
			player2.active = true;
			player2.was_active = true;
		}
		
		break;
	case play:
		render_play();
		for (int i = 0; i < bullets.size(); i++)
			bullets[i].update();
		for (int i = 0; i < bullets.size(); i++){
			if (bullets[i].check_collisions(player1.position_x, player1.position_y)){
				player1.active = false;
				state = game_over;
			}
			if (player2.active)
				if (bullets[i].check_collisions(player2.position_x, player2.position_y)){
					player2.active = false;
					state = game_over;
				}
		}
		if (keys[SDL_SCANCODE_ESCAPE])
		{
			state = pause;
			player1.active = false;
			player2.active = false;
			
		}
		break;
	case pause:
		render_pause();
		if (keys[SDL_SCANCODE_RETURN])
		{
			state = play;
			player1.active = true;
			if (player2.was_active == true)
				player2.active = true;
		}
		else if (keys[SDL_SCANCODE_BACKSPACE])
		{
			state = main_menu;
		}
		break;
	case game_over:
		if (keys[SDL_SCANCODE_RETURN])
		{
			state = main_menu;
		}
		render_game_over();
		break;
	}
}


void Game_Entity::render_main_menu(){
	modelMatrix.identity();
	modelMatrix.Translate(-.5, 1.5, 0);
	program->setModelMatrix(modelMatrix);
	DrawText(fontID, "Don't", .4, -.1, program);

	modelMatrix.identity();
	modelMatrix.Translate(-.25 , .95, 0);
	program->setModelMatrix(modelMatrix);
	DrawText(fontID, "Get", .5, -.1, program);

	modelMatrix.identity();
	modelMatrix.Translate(-1.4, .3, 0);
	program->setModelMatrix(modelMatrix);
	DrawText(fontID, "CHOPPED!", .6, -.1, program);

	modelMatrix.identity();
	modelMatrix.Translate(-2, -1.5, 0);
	program->setModelMatrix(modelMatrix);
	DrawText(fontID, "Press Space to continue..", .2, 0, program);
}


void Game_Entity::render_directions(){
	modelMatrix.identity();
	modelMatrix.Translate(-1.5, 1.5, 0);
	program->setModelMatrix(modelMatrix);
	DrawText(fontID, "Directions", .37, 0, program);

	modelMatrix.identity();
	modelMatrix.Translate(-2.25, .8, 0);
	program->setModelMatrix(modelMatrix);
	DrawText(fontID, "Player 1: W, A, S, and D to move", .2, -.05, program);

	modelMatrix.identity();
	modelMatrix.Translate(-2.25, .3, 0);
	program->setModelMatrix(modelMatrix);
	DrawText(fontID, "Player 2: I, J, K, and L to move", .2, -.05, program);

	modelMatrix.identity();
	modelMatrix.Translate(-3.25, -1.5, 0);
	program->setModelMatrix(modelMatrix);
	DrawText(fontID, "Press W for single player or I for two player", .2, -.05, program);

}


void Game_Entity::render_play(){
	renderP1();
	if (player2.active == true)
		renderP2();

	renderBullets();

	score_ticks++;
	if (score_ticks == 1000){
		score_ticks = 0;
		score += 1;
	}
	modelMatrix.identity();
	modelMatrix.Translate(2, 1.8, 0);
	program->setModelMatrix(modelMatrix);
	DrawText(fontID, "Score: " + std::to_string(score), .15, 0, program);
}


void Game_Entity::render_pause(){
	modelMatrix.identity();
	modelMatrix.Translate(-1, 1.5, 0);
	program->setModelMatrix(modelMatrix);
	DrawText(fontID, "Pause", .5, 0, program);

	modelMatrix.identity();
	modelMatrix.Translate(-2.9, .3, 0);
	program->setModelMatrix(modelMatrix);
	DrawText(fontID, "Press ENTER to continue playing", .2, 0, program);

	modelMatrix.identity();
	modelMatrix.Translate(-2.9, -.8, 0);
	program->setModelMatrix(modelMatrix);
	DrawText(fontID, "Press BACKSPACE to return to the", .2, 0, program);

	modelMatrix.identity();
	modelMatrix.Translate(-1, -1.1, 0);
	program->setModelMatrix(modelMatrix);
	DrawText(fontID, "main menu", .2, 0, program);
}


void Game_Entity::render_game_over(){
	modelMatrix.identity();
	modelMatrix.Translate(-1.75, 1.5, 0);
	program->setModelMatrix(modelMatrix);
	DrawText(fontID, "Game Over", .5, 0, program);

	if (player2.was_active){
		if (player1.active){
			modelMatrix.identity();
			modelMatrix.Translate(-1.5, .7, 0);
			program->setModelMatrix(modelMatrix);
			DrawText(fontID, "Player 1 Wins!", .25, 0, program);
		}
		else {
			modelMatrix.identity();
			modelMatrix.Translate(-1.5, .7, 0);
			program->setModelMatrix(modelMatrix);
			DrawText(fontID, "Player 2 Wins!", .25, 0, program);
		}

	}

	if (score >= high_score){
		modelMatrix.identity();
		modelMatrix.Translate(-2, 0, 0);
		program->setModelMatrix(modelMatrix);
		DrawText(fontID, "New High Score!", .35, 0, program);

		high_score = score;
	}

	modelMatrix.identity();
	modelMatrix.Translate(-.75, -1, 0);
	program->setModelMatrix(modelMatrix);
	DrawText(fontID, "Score: " + std::to_string(score), .25, 0, program);

	modelMatrix.identity();
	modelMatrix.Translate(-1.5, -1.8, 0);
	program->setModelMatrix(modelMatrix);
	DrawText(fontID, "Press ENTER to continue..", .15, 0, program);
	
}


void Game_Entity::renderP1(){
	glUseProgram(program->programID);
	glVertexAttribPointer(program->positionAttribute, 2, GL_FLOAT, false, 0, vert);
	glVertexAttribPointer(program->texCoordAttribute, 2, GL_FLOAT, false, 0, texCoords);
	glBindTexture(GL_TEXTURE_2D, player1.textureID);
	glEnableVertexAttribArray(program->positionAttribute);
	glEnableVertexAttribArray(program->texCoordAttribute);

	modelMatrix.identity();
	modelMatrix.Scale(.25, .25, 1.0);
	modelMatrix.Translate(player1.position_x, player1.position_y, 0.0f);

	program->setModelMatrix(modelMatrix);
	glDrawArrays(GL_TRIANGLES, 0, 6);

	glDisableVertexAttribArray(program->positionAttribute);
	glDisableVertexAttribArray(program->texCoordAttribute);
}

void Game_Entity::renderP2(){

	glUseProgram(program->programID);
	glVertexAttribPointer(program->positionAttribute, 2, GL_FLOAT, false, 0, vert);
	glVertexAttribPointer(program->texCoordAttribute, 2, GL_FLOAT, false, 0, texCoords);
	glBindTexture(GL_TEXTURE_2D, player2.textureID);

	glEnableVertexAttribArray(program->positionAttribute);
	glEnableVertexAttribArray(program->texCoordAttribute);

	modelMatrix.identity();
	modelMatrix.Scale(.25, .25, 1.0);
	modelMatrix.Translate(player2.position_x, player2.position_y, 0.0f);

	program->setModelMatrix(modelMatrix);
	glDrawArrays(GL_TRIANGLES, 0, 6);

	glDisableVertexAttribArray(program->positionAttribute);
	glDisableVertexAttribArray(program->texCoordAttribute);
}

void Game_Entity::renderBullets(){
	for (int i = 0; i < bullets.size(); i++)
	{
		glUseProgram(program->programID);
		glVertexAttribPointer(program->positionAttribute, 2, GL_FLOAT, false, 0, vert);
		glVertexAttribPointer(program->texCoordAttribute, 2, GL_FLOAT, false, 0, texCoords);
		glBindTexture(GL_TEXTURE_2D, bullets[i].textureID);

		glEnableVertexAttribArray(program->positionAttribute);
		glEnableVertexAttribArray(program->texCoordAttribute);

		float ticks = (float)SDL_GetTicks() / 1000.0f;
		float elapsed = ticks - bullets[i].lastframeticks;
		bullets[i].angle += elapsed * 3.14 * bullets[i].rand_rotate_value * bullets[i].rand_rotate_direction;
		bullets[i].lastframeticks = ticks;

		modelMatrix.identity();
		//modelMatrix.Scale(bullets[i].scale * .25, bullets[i].scale * .25, 1.0);
		modelMatrix.Scale(.25, .25, 1.0);

		modelMatrix.Translate(bullets[i].position_x, bullets[i].position_y, 0.0f);
		modelMatrix.Rotate(bullets[i].angle);
		program->setModelMatrix(modelMatrix);
		glDrawArrays(GL_TRIANGLES, 0, 6);

		glDisableVertexAttribArray(program->positionAttribute);
		glDisableVertexAttribArray(program->texCoordAttribute);
	}

}

void Game_Entity::renderBackground(){
	glUseProgram(program->programID);
	glVertexAttribPointer(program->positionAttribute, 2, GL_FLOAT, false, 0, vert);
	glVertexAttribPointer(program->texCoordAttribute, 2, GL_FLOAT, false, 0, texCoords);
	glBindTexture(GL_TEXTURE_2D, background_tex_id);
	glEnableVertexAttribArray(program->positionAttribute);
	glEnableVertexAttribArray(program->texCoordAttribute);

	modelMatrix.identity();
	modelMatrix.Scale(4.2f, 4.2f, 1.0f);
	program->setModelMatrix(modelMatrix);
	glDrawArrays(GL_TRIANGLES, 0, 6);

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
	fontID = LoadTexture("font2.png");
	background_tex_id = LoadTexture("Background.png");
	player2.textureID = LoadTexture("pumpkin.png");
	player1.textureID = LoadTexture("tomato.png"); 
	std::vector<std::string> images;
	images.push_back("knife.png");
	images.push_back("spatula.png");
	images.push_back("knife2.png");
	images.push_back("spoon.png");
	images.push_back("fork.png");

	int temp;
	for (int i = 0; i < 30; i++){

		temp = rand() % images.size();
		Bullet_Entity bullet;
		bullet.textureID = LoadTexture(images[temp].c_str());
		//bullet.initialize();
		bullets.push_back(bullet);
	}

	score_ticks = 0;

	program = new ShaderProgram(RESOURCE_FOLDER"vertex_textured.glsl", RESOURCE_FOLDER"fragment_textured.glsl");
	while (!done){
		process_events();
		update(); //update, then render
 		SDL_GL_SwapWindow(displayWindow);
	}
	SDL_Quit();
}