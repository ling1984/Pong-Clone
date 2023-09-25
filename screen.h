#pragma once
// screen.h

#ifndef SCREEN_H
#define SCREEN_H

#include <SDL.h>
#include <vector>
#include <SDL_ttf.h>
#include <iostream>

// Function declarations
class Screen
{
private:
	SDL_Event e;
	SDL_Window* window = nullptr;
	SDL_Renderer* renderer = nullptr;
	std::vector<SDL_FPoint> points;

	SDL_Color TextColor = { 255, 255, 255, 255 }; // Red SDL color.
	TTF_Font* Font = nullptr; // The font to be loaded from the ttf file.
	SDL_Surface* TextSurface = nullptr; // The surface necessary to create the font texture.
	SDL_Texture* TextTexture = nullptr; // The font texture prepared for render.
	SDL_Rect TextRect; // Text rectangle area with the position for the texture text.
public:
	Screen();
	void show();
	void pixel(float x, float y);
	void clear();
	int input();
	void createText(const char* message, int xPos, int yPos);
};
#endif