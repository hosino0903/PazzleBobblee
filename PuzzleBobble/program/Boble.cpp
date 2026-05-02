#include"Boble.h"
#include"Main.h"
#include"Float2.h"

int Bobble;
float Bobble_x;
float Bobble_y;
float Bobble_speed;

void BobleInit()
{
		Bobble_x = 400.0f;
		Bobble_y = 400.0f;
		Bobble_speed = 2.0f;

}

void BobleUpdate()
{
	if (PushHitKey(KEY_INPUT_SPACE))
	{
		Bobble_y--;
	}

}

void BobleRender()
{
	DrawCircle(Bobble_x, Bobble_y, 20, 0xFF0000FF);

}

void BobleExit()
{


}