#ifdef _WINDOWS
#include <GL/glew.h>
#endif
#include <SDL.h>
#include <SDL_opengl.h>
#include <SDL_image.h>
#include "ShaderProgram.h"
#include "Matrix.h"

#ifdef _WINDOWS
#define RESOURCE_FOLDER ""
#else
#define RESOURCE_FOLDER "NYUCodebase.app/Contents/Resources/"
#endif

SDL_Window* displayWindow;

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

int main(int argc, char *argv[])
{
	SDL_Init(SDL_INIT_VIDEO);
	displayWindow = SDL_CreateWindow("Assignment 3, 2, 1", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 640, 360, SDL_WINDOW_OPENGL);
	SDL_GLContext context = SDL_GL_CreateContext(displayWindow);
	SDL_GL_MakeCurrent(displayWindow, context);
#ifdef _WINDOWS
	glewInit();
#endif

	glViewport(0, 0, 640, 360);
	ShaderProgram program(RESOURCE_FOLDER"vertex_textured.glsl", RESOURCE_FOLDER"fragment_textured.glsl");
	Matrix projectionMatrix;
	Matrix modelMatrix1;
	Matrix modelMatrix2;
	Matrix modelMatrix3;
	Matrix viewMatrix;

	projectionMatrix.setOrthoProjection(-3.55f, 3.55f, -2.0f, 2.0f, -1.0f, 1.0f);

	unsigned int FireTextureID = LoadTexture("fire.png");
	unsigned int Sparkle2TextureID = LoadTexture("sparkle.png");
	unsigned int Fire2TextureID = LoadTexture("fire2.png");
	float lastFrameTicks = 0.0f;
	float angle = 0.0f;

	float positionP1 = 0.0f;
	float p1Top = positionP1 + 1;
	float p1Bottom = positionP1 - 1;
	float positionP2 = 0.0f;
	float p2Top = positionP2 + 1;
	float p2Bottom = positionP2 - 1;
	float ballPosX = 0.0f;
	float ballPosY = 0.0f;
	bool gamestarted = false;
	float ballDirecY = 1.0f;
	float ballDirecX = 1.0f;
	float ballVel = .00025f;

	SDL_Event event;
	bool done = false;
	while (!done) {
		while (SDL_PollEvent(&event)) {
			if (event.type == SDL_QUIT || event.type == SDL_WINDOWEVENT_CLOSE) {
				done = true;
			}
		}
		const Uint8 *keys = SDL_GetKeyboardState(NULL);

		program.setModelMatrix(modelMatrix1);
		program.setProjectionMatrix(projectionMatrix);
		program.setViewMatrix(viewMatrix);

		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);


		glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);

		

		float ticks = (float)SDL_GetTicks() / 1000.0f;
		float elapsed = ticks - lastFrameTicks;
		lastFrameTicks = ticks;


		//the 0-1 UV coords -- units NOT pixels
		float texCoords[] = { 0.0f, 0.0f, 0.0f, 1.0f, 1.0f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f, 1.0f, 0.0f };
		float vert[] = { -1.0f, 1.0f, -1.0f, -1.0f, 1.0f, 1.0f, -1.0f, -1.0f, 1.0f, -1.0f, 1.0f, 1.0f };

		glUseProgram(program.programID);
		glBindTexture(GL_TEXTURE_2D, FireTextureID);

		//P1
		glVertexAttribPointer(program.positionAttribute, 2, GL_FLOAT, false, 0, vert);
		glVertexAttribPointer(program.texCoordAttribute, 2, GL_FLOAT, false, 0, texCoords);

		glEnableVertexAttribArray(program.positionAttribute);
		glEnableVertexAttribArray(program.texCoordAttribute);

		glDrawArrays(GL_TRIANGLES, 0, 6);
		modelMatrix1.identity();
		if (keys[SDL_SCANCODE_A])
		{
			positionP1 -= .00025f;
		}
		else if (keys[SDL_SCANCODE_Q])
		{
			positionP1 += .00025f;
		}
		modelMatrix1.Translate(-3.0, positionP1, 0.0);

		glDisableVertexAttribArray(program.positionAttribute);
		glDisableVertexAttribArray(program.texCoordAttribute);

		//P2
		glBindTexture(GL_TEXTURE_2D, Fire2TextureID);
		program.setModelMatrix(modelMatrix2);
		modelMatrix2.identity();

		glEnableVertexAttribArray(program.positionAttribute);
		glEnableVertexAttribArray(program.texCoordAttribute);
		glDrawArrays(GL_TRIANGLES, 0, 6);
		modelMatrix2.identity();
		if (keys[SDL_SCANCODE_L])
		{
			positionP2 -= .00025f;
		}
		else if (keys[SDL_SCANCODE_P])
		{
			positionP2 += .00025f;
		}
		modelMatrix2.Translate(3.0, positionP2, 0.0);
		glDisableVertexAttribArray(program.positionAttribute);
		glDisableVertexAttribArray(program.texCoordAttribute);

		//ball
		glBindTexture(GL_TEXTURE_2D, Sparkle2TextureID);
		program.setModelMatrix(modelMatrix3);
		glEnableVertexAttribArray(program.positionAttribute);
		glEnableVertexAttribArray(program.texCoordAttribute);
		glDrawArrays(GL_TRIANGLES, 0, 6);
		modelMatrix3.identity();
		modelMatrix3.Translate(ballPosX, ballPosY, 0.0);
		glDisableVertexAttribArray(program.positionAttribute);
		glDisableVertexAttribArray(program.texCoordAttribute);
		
		//bouncing off top/bottom "walls"
		if (ballPosY + 0.02f >= 2.0f || ballPosY - 0.02f <= -2.0f)
		{
			ballDirecY *= -1;
			ballPosX += ballVel * ballDirecX;
			ballPosY += ballVel * ballDirecY;
		}
		//game over stuff goes here
		else if (ballPosX >= 4.0f || ballPosX <= -4.0f){
			done = true;
		}
		//paddle collision
		else if ((ballPosX >= 2.8f && (ballPosY <= p2Top && ballPosY >= p2Bottom)) || (ballPosX <= -2.8f && (ballPosY >= p1Bottom && ballPosY <= p1Top))){
		//else if ( ballPosX >= 2.8f || ballPosX <= -2.8f){
			ballDirecX *= -1;
			ballPosX += ballVel * ballDirecX;
			ballPosY += ballVel * ballDirecY;
		}
		//ball movement without collision
		else {
			ballPosX += ballVel * ballDirecX;
			ballPosY += ballVel * ballDirecY;
		}

		SDL_GL_SwapWindow(displayWindow);
	}

	SDL_Quit();
	return 0;
}
