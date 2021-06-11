#pragma once
#include <SDL.h>
#include <SDL_Image.h>
#include <iostream>
#include <windows.h>

const float PI = 3.1415;

/*
*	Textures are expected to have same pixel format
*/

//Source texture gets a wavy effect
inline void horizontalOscillation(SDL_Texture* src, SDL_Texture* dst, float timerOffset, float revDivider, int amplitude)
{
	int w, h;
	SDL_QueryTexture(src, NULL, NULL, &w, &h);

	void* pix_tex1, * pix_tex2;
	int pitch1, pitch2;

	SDL_LockTexture(src, NULL, (void**)&pix_tex1, &pitch1);
	SDL_LockTexture(dst, NULL, (void**)&pix_tex2, &pitch2);

	int pixel_size = pitch1 / w;

	for (int i = 0; i < h; ++i)
	{
		int moveby = int(sin(i * PI / revDivider + timerOffset) * amplitude) * pixel_size;
		int offset = (pitch1 * i);
		memcpy((void*)((int)pix_tex2 + offset + max(moveby, 0)), (void*)((int)pix_tex1 + offset - min(moveby, 0)), pitch1);
	}

	SDL_UnlockTexture(src);
	SDL_UnlockTexture(dst);
}

//Same as previous, but all even and odd lines goes in opposite directions
inline void horizontalOscillation2dir(SDL_Texture* src, SDL_Texture* dst, float timerOffset, float revDivider, int amplitude)
{
	int w, h;
	SDL_QueryTexture(src, NULL, NULL, &w, &h);

	void* pix_tex1, * pix_tex2;
	int pitch1, pitch2;

	SDL_LockTexture(src, NULL, (void**)&pix_tex1, &pitch1);
	SDL_LockTexture(dst, NULL, (void**)&pix_tex2, &pitch2);

	int pixel_size = pitch1 / w;

	for (int i = 0; i < h; ++i)
	{
		if (i % 2)
		{
			int moveby = int(sin(i * PI / revDivider + timerOffset) * amplitude) * pixel_size;
			int offset = (pitch1 * i);
			memcpy((void*)((int)pix_tex2 + offset + max(moveby, 0)), (void*)((int)pix_tex1 + offset - min(moveby, 0)), pitch1);
		}
		else
		{
			int moveby = -int(sin(i * PI / revDivider + timerOffset) * amplitude) * pixel_size;
			int offset = (pitch1 * i);
			memcpy((void*)((int)pix_tex2 + offset + max(moveby, 0)), (void*)((int)pix_tex1 + offset - min(moveby, 0)), pitch1);
		}
	}

	SDL_UnlockTexture(src);
	SDL_UnlockTexture(dst);
}

//Source texture gets a vertical wave
inline void verticalOscillation(SDL_Texture* src, SDL_Texture* dst, float timerOffset, float revDivider, int amplitude)
{
	int w, h;
	SDL_QueryTexture(src, NULL, NULL, &w, &h);

	void* pix_tex1, * pix_tex2;
	int pitch1, pitch2;

	SDL_LockTexture(src, NULL, (void**)&pix_tex1, &pitch1);
	SDL_LockTexture(dst, NULL, (void**)&pix_tex2, &pitch2);

	int pixel_size = pitch1 / w;

	for (int i = 0; i < h; ++i)
	{
		int lineOffset = int(sin(i * PI / revDivider + timerOffset) * amplitude);
		if (i + lineOffset < 0)
			lineOffset = -i;
		if (i + lineOffset > 719)
			lineOffset = 719 - i;
		memcpy((void*)((int)pix_tex2 + i * pitch1), (void*)((int)pix_tex1 + (i + lineOffset) * pitch1), pitch1);
	}

	SDL_UnlockTexture(src);
	SDL_UnlockTexture(dst);
}