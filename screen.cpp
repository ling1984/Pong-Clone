// screen.cpp
#include "screen.h"

// credits to TheBuilder on YouTube this file is adapted from his code
// youtube.com/c/TheBuilder/
Screen::Screen()
{
	SDL_Init(SDL_INIT_VIDEO);
	SDL_CreateWindowAndRenderer(500 * 2, 400 * 2, 0, &window, &renderer);
	SDL_RenderSetScale(renderer, 2, 2);
}

void Screen::show()
{
	SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
	SDL_RenderClear(renderer);
	SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
	SDL_RenderCopy(renderer, TextTexture, NULL, &TextRect);
	//loop drawing the points
	for (auto& point : points)
	{
		SDL_RenderDrawPointF(renderer, point.x, point.y);
	}
	SDL_RenderPresent(renderer);
}

// adds an x and a y as a point to the vector of points
void Screen::pixel(float x, float y)
{
	SDL_FPoint point;
	point.x = x;
	point.y = y;
	points.emplace_back(point);
}

// clears the vector of points
void Screen::clear()
{
	points.clear();
	SDL_DestroyTexture(TextTexture);
}

// handles input and returns corresponding values
int Screen::input()
{
	while (SDL_PollEvent(&e))
	{
		if (e.type == SDL_QUIT)
		{
			SDL_Quit();
			exit(0);
		}
		else if (e.type == SDL_KEYDOWN)
		{
			switch (e.key.keysym.sym)
			{
			case SDLK_LEFT:
				return 1;

			case SDLK_RIGHT:
				return 3;
			}
		}
		else if (e.type == SDL_KEYUP)
		{
			switch (e.key.keysym.sym)
			{
			case SDLK_LEFT:
				return 2;
			case SDLK_RIGHT:
				return 4;
			}
		}
	}
	return 0;
}

// text functions

void Screen::createText(const char* message, int xPos, int yPos)
{
	TTF_Init();
	TTF_Font* font = TTF_OpenFont("fonts/lazy.ttf", 40);
	if (!font)
		std::cout << "Couldn't find/init open ttf font." << std::endl;

	TextSurface = TTF_RenderText_Solid(font, message, TextColor);
	TextTexture = SDL_CreateTextureFromSurface(renderer, TextSurface);
	TextRect.x = xPos; 
	TextRect.y = yPos;
	TextRect.w = TextSurface->w;
	TextRect.h = TextSurface->h;
	SDL_FreeSurface(TextSurface);
	TTF_Quit();
}