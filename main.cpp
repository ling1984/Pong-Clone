#include "screen.h"
#include <iostream>
#include <string>

// too many magic numbers
// probably should split into classes
// 400 is screen height and 500 width

constexpr int FPS = 200;// frames per second;

struct ball
{
	float xVelocity;
	float yVelocity;
	int radius;
	float xPos;
	float yPos;
	float defaultX = 250;
	float defaultY = 200;
	float defaultXvel = 200;
	float defaultYvel = 0;
	bool direction;

	ball(bool startDirection = true, float defaultX = 250, float defaultY = 200, float defaultXvel = 150, float defaultYvel = 0, int r = 20) : direction(startDirection), xVelocity(defaultXvel), yVelocity(defaultYvel), xPos(defaultX), yPos(defaultY), radius(r) {}
};

struct paddle
{
	float velocity;
	int width;
	int height;
	float xPos;
	float yPos;

	paddle(float v, int width = 30, int height = 100, float xP = 0, float yP = 0) : velocity(v), xPos(xP), yPos(yP), width(width), height(height) {}
};

void drawPaddle(Screen& screen, paddle* paddle)
{
	for (int i = 0; i < paddle->width; i++)
	{
		// horizontal lines
		screen.pixel(paddle->xPos + i, paddle->yPos);
		screen.pixel(paddle->xPos + i, paddle->yPos + paddle->height);
	}
	for (int i = 0; i < paddle->height; i++)
	{
		// vertical lines
		screen.pixel(paddle->xPos, paddle->yPos + i);
		screen.pixel(paddle->xPos+paddle->width, paddle->yPos + i);
	}
	screen.pixel(paddle->xPos + paddle->width, paddle->yPos + paddle->height);
}

void drawBall(Screen& screen, ball* ball)
{
	for (int i = 0; i < ball->radius; i++)
	{
		// horizontal lines
		screen.pixel(ball->xPos + i, ball->yPos);
		screen.pixel(ball->xPos + i, ball->yPos + ball->radius);
		// vertical lines
		screen.pixel(ball->xPos, ball->yPos + i);
		screen.pixel(ball->xPos + ball->radius, ball->yPos + i);
	}
	// bottom right corner
	screen.pixel(ball->xPos + ball->radius, ball->yPos + ball->radius);
}


void checkWallCollision(ball* ball)
{
	// checking if it hits the ceiling
	if (ball->yPos <= 0 || (ball->yPos + ball->radius) >= 400)
	{
		ball->yVelocity = -(ball->yVelocity);
	}
}

bool hasP1Scored(ball* ball, int& player1score)
{
	if ((ball->xPos + ball->radius) >= 500)
	{
		player1score++;
		ball->direction = true;
		return true;
	}
	return false;
}

bool hasP2Scored(ball* ball, int& player2score)
{
	if (ball->xPos <= 0)
	{
		player2score++;
		ball->direction = false;
		return true;
	}
	return false;
}

bool checkBallPaddleCollision(ball* ball, paddle* paddle1, paddle* paddle2) // refactor all this
{
	
	// paddle1 check
	
	// making a box width half of the paddle at the front
	if (ball->xPos <= paddle1->xPos + paddle1->width && 
		ball->xPos >= paddle1->xPos + (paddle1->width) / 2)
	{
		// with the same height as the paddle
		if (ball->yPos >= paddle1->yPos &&
			ball->yPos <= (paddle1->yPos + paddle1->height) ||
			ball->yPos + ball->radius >= paddle1->yPos &&
			ball->yPos + ball->radius <= (paddle1->yPos + paddle1->height))
		{
			ball->xVelocity = -(ball->xVelocity);
			ball->xPos = paddle1->xPos + paddle1->width;
			ball->yVelocity = ball->xVelocity * -((paddle1->yPos + (paddle1->height / 2) - (ball->yPos + ball->radius / 2)) / (paddle1->height / 2));
			return true;
		}
	}
	// now doing the tops and bottoms
	if (ball->xPos <= paddle1->xPos + (paddle1->width / 2) &&
		ball->xPos >= paddle1->xPos + paddle1->width)
	{
		// with the same height as the paddle
		if (ball->yPos >= paddle1->yPos &&
			ball->yPos <= (paddle1->yPos + paddle1->height))
		{
			ball->yVelocity = -(ball->yVelocity);
			ball->yPos = paddle1->yPos + paddle1->height;
		}
		if
			(ball->yPos + ball->radius >= paddle1->yPos &&
			ball->yPos + ball->radius <= (paddle1->yPos + paddle1->height))
		{
			ball->yVelocity = -(ball->yVelocity);
			ball->yPos = paddle1->yPos;
		}
	}

	// paddle2 check
	if (ball->xPos+ball->radius >= paddle2->xPos 
		&& ball->xPos+ball->radius <= paddle2->xPos + (paddle2->width / 2))
	{
		if ((ball->yPos >= paddle2->yPos &&
			ball->yPos <= (paddle2->yPos + paddle2->height)) ||
			ball->yPos + ball->radius >= paddle2->yPos &&
			ball->yPos + ball->radius <= (paddle2->yPos + paddle2->height))
		{
			ball->xVelocity = -(ball->xVelocity);
			ball->xPos = paddle2->xPos - ball->radius;
			ball->yVelocity = ball->xVelocity * -((paddle2->yPos + (paddle2->height / 2) - (ball->yPos + ball->radius / 2)) / (paddle2->height / 2));
			return true;
		}
	}
	// top and bottom check
	if (ball->xPos + ball->radius >= paddle2->xPos + (paddle2->width / 2)
		&& ball->xPos + ball->radius <= paddle2->xPos + paddle2->width)
	{
		if ((ball->yPos >= paddle2->yPos &&
			ball->yPos <= (paddle2->yPos + paddle2->height)))
		{
			ball->yVelocity = -(ball->yVelocity);
			ball->yPos = paddle2->yPos + paddle2->height;
		}
		if (ball->yPos + ball->radius >= paddle2->yPos &&
			ball->yPos + ball->radius <= (paddle2->yPos + paddle2->height))
		{
			ball->yVelocity = -(ball->yVelocity);
			ball->yPos = paddle2->yPos;
		}
	}
	return false;
}

void drawLines(Screen& screen)
{
	for (float i = 55; i <= 325; i++)
	{
		if (i <= 80 || i>105 && i <=130 || i > 155 && i <= 180 || i > 205 && i <= 230 || i > 255 && i <= 280 || i > 300)
		{
			for (float j = 250; j < 251; j++)
			{
				screen.pixel(j, i);
			}
		}
	}

}

void drawEverything(Screen& screen, paddle p1, paddle p2, ball ball, std::string message)
{
	drawBall(screen, &ball);
	drawPaddle(screen, &p1);
	drawPaddle(screen, &p2);
	drawLines(screen);
}

void moveAI(ball ball, paddle* p2)
{
	if(p2->yPos+(p2->height/2) + 5 < ball.yPos+(ball.radius/2)){ p2->yPos += (p2->velocity / FPS); }
	else if (p2->yPos + (p2->height / 2) - 5 > ball.yPos + (ball.radius / 2)){ p2->yPos -= (p2->velocity / FPS); }
}

// make draw everything and move everything functions
int main(int argc, char* args[])
{
	Screen screen;
	ball ball(false);
	paddle p1(240, 10, 100, 10, 200);
	paddle p2(240, 10, 100, 480, 200);
	int player1score = 0;
	int player2score = 0;

	enum inputs
	{
		leftDown = 1,
		leftUp = 2,
		rightDown = 3,
		rightUp = 4,
		pause = 5
	};

	bool leftPressed = false;
	bool rightPressed = false;
	std::string message = " Score: " + std::to_string(player1score) + "       Score: " + std::to_string(player2score);

	// main game loop
	while (true)
	{
		if (rightPressed)
		{
			p1.yPos += (p1.velocity / FPS);
		}
		else if (leftPressed)
		{
			p1.yPos -= (p1.velocity / FPS);
		}
		moveAI(ball, &p2);
		ball.xPos += ball.xVelocity / FPS;
		ball.yPos += ball.yVelocity / FPS;
		// move paddles and ball

		drawEverything(screen, p1, p2, ball, message);

		if (hasP1Scored(&ball, player1score) || hasP2Scored(&ball, player2score))
		{

			std::cout << "Player 1 score : " << player1score << "\n";
			std::cout << "Player 2 score : " << player2score << "\n";
			message = " Score: " + std::to_string(player1score) + "       Score: " + std::to_string(player2score);

			ball.yPos = ball.defaultY;
			if (ball.direction) {ball.xVelocity = ball.defaultXvel; ball.xPos = ball.defaultX;}
			else { ball.xVelocity = -(ball.defaultXvel); ball.xPos = ball.defaultX - ball.radius;}
			ball.yVelocity = ball.defaultYvel;

			screen.clear();
			screen.createText(message.c_str(), 0, 0);
			drawEverything(screen, p1, p2, ball, message);
			screen.show();
			SDL_Delay(1000);

			if (player1score >= 3) {
				screen.clear();
				screen.createText("Player 1 won", 120, 200);
				screen.show();
				SDL_Delay(2000);
				break;
			}
			else if (player2score >= 3) // end game condition
			{
				screen.clear();
				screen.createText("Player 2 won", 120, 200);
				screen.show();
				SDL_Delay(2000);
				break;
			}
		}
		checkWallCollision(&ball);
		if (checkBallPaddleCollision(&ball, &p1, &p2)) { ball.xVelocity *= 1.1; std::cout << ball.xVelocity << " is x vel" << std::endl; }

		screen.show();
		SDL_Delay(1000/FPS); 
		// 5 ms delay

		switch (screen.input())
		{
		case leftDown:
			leftPressed = true;
			break;
		case leftUp:
			leftPressed = false;
			break;
		case rightDown:
			rightPressed = true;
			break;
		case rightUp:
			rightPressed = false;
			break;
		case pause:
			std::cout << "pause" << std::endl;
		}
		screen.clear();
	}
	screen.createText("Game Over!", 150, 100);
	screen.show();
	SDL_Delay(3000);
	screen.clear();

	return 0;
}