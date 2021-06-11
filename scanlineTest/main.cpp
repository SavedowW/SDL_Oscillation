#include "Oscillation.h"

using namespace std;

const int SCREEN_WIDTH = 1280;
const int SCREEN_HEIGHT = 720;

const char* WINDOW_NAME = "TEST";

SDL_Renderer* ren_main;
SDL_Window* win_main;

bool init_system();
SDL_Texture* loadTexture(const char* file);
SDL_Texture* loadStreamingTexture(const char* file);
void renderTexture(SDL_Texture* tex, int x, int y, int w, int h);
static void renderTexture(SDL_Texture* tex, int x, int y);
void close();
void fillRenderer(SDL_Color col);

int SDL_main(int argc, char* args[])
{
	if (!init_system())
	{
		return 0;
	}

	SDL_Texture* tex1 = loadStreamingTexture("basePic.png");
	SDL_Texture* tex2 = SDL_CreateTexture(ren_main, SDL_PIXELFORMAT_ARGB8888, SDL_TEXTUREACCESS_STREAMING, 1280, 720);
	SDL_Texture* tex3 = SDL_CreateTexture(ren_main, SDL_PIXELFORMAT_ARGB8888, SDL_TEXTUREACCESS_STREAMING, 1280, 720);
	SDL_SetTextureBlendMode(tex1, SDL_BLENDMODE_BLEND);
	SDL_SetTextureBlendMode(tex2, SDL_BLENDMODE_BLEND);
	SDL_SetTextureBlendMode(tex3, SDL_BLENDMODE_BLEND);

	bool progRunning = true;
	SDL_Event e;
	
	float timer = 0;
	void* pix_tex1, * pix_tex2;
	int pitch1, pitch2;

	while (progRunning)
	{
		while (SDL_PollEvent(&e))
		{
			if (e.type == SDL_QUIT)
			{
				progRunning = false;
			}
		}

		fillRenderer({ 0, 0, 0, 255 });

		timer += PI / 50;
		while (timer >= PI * 2)
		{
			timer -= PI * 2;
		}

		verticalOscillation(tex1, tex2, timer, 180, 50);
		SDL_SetTextureAlphaMod(tex2, 126);
		horizontalOscillation2dir(tex1, tex3, timer, 180, 20);
		SDL_SetTextureAlphaMod(tex3, 126);

		renderTexture(tex2, 0, 0);
		renderTexture(tex3, 0, 0);

		SDL_RenderPresent(ren_main);
		SDL_Delay(1000.0f/60.0f);
	}
}

bool init_system()
{
	if (SDL_Init(SDL_INIT_EVERYTHING) < 0)
	{
		std::cout << "SDL initialization error: " << SDL_GetError() << std::endl;
		return false;
	}
	if ((IMG_Init(IMG_INIT_PNG) & IMG_INIT_PNG) != IMG_INIT_PNG)
	{
		std::cout << "IMG initialization error: " << SDL_GetError() << std::endl;
		return false;
	}
	win_main = SDL_CreateWindow(WINDOW_NAME, SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
	if (win_main == NULL)
	{
		
		std::cout << "Window creation error: " << SDL_GetError() << std::endl;
		return false;
	}
	ren_main = SDL_CreateRenderer(win_main, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
	if (ren_main == nullptr) {
		std::cout << "Renderer creation error: " << SDL_GetError() << std::endl;
		return false;
	}
	SDL_SetRenderDrawBlendMode(ren_main, SDL_BLENDMODE_BLEND);
	std::cout << "All systems were successfully initialized\n";
	return true;
}

SDL_Texture* loadTexture(const char* file)
{
	SDL_Texture* texture = IMG_LoadTexture(ren_main, file);
	if (!texture) {
		std::cout << "Texture loading problem: " << file << "  |  " << IMG_GetError() << std::endl;
	}
	else {
		std::cout << file << " successfully loaded\n";
	}
	return texture;
}

void renderTexture(SDL_Texture* tex, int x, int y, int w, int h)
{
	SDL_Rect dst;
	dst.x = x;
	dst.y = y;
	dst.w = w;
	dst.h = h;
	SDL_RenderCopy(ren_main, tex, NULL, &dst);
}

static void renderTexture(SDL_Texture* tex, int x, int y)
{
	int w, h;
	SDL_QueryTexture(tex, NULL, NULL, &w, &h);
	renderTexture(tex, x, y, w, h);
}

void close()
{
	SDL_DestroyRenderer(ren_main);
	SDL_DestroyWindow(win_main);
	IMG_Quit();
	SDL_Quit();
	std::cout << "All systems were successfully closed\n";
}

void fillRenderer(SDL_Color col)
{
	SDL_SetRenderDrawColor(ren_main, col.r, col.g, col.b, col.a);
	SDL_RenderClear(ren_main);
}

SDL_Texture* loadStreamingTexture(const char* file)
{
	SDL_Surface* ssSur = IMG_Load(file);
	SDL_Surface* convSur = SDL_ConvertSurfaceFormat(ssSur, SDL_PIXELFORMAT_ARGB8888, 0);
	SDL_Texture* tex = SDL_CreateTexture(ren_main, SDL_PIXELFORMAT_ARGB8888, SDL_TEXTUREACCESS_STREAMING, 1280, 720);
	void* pix;
	int pitch;
	SDL_LockTexture(tex, NULL, &pix, &pitch);
	memcpy(pix, convSur->pixels, convSur->pitch * convSur->h);
	SDL_UnlockTexture(tex);
	SDL_FreeSurface(ssSur);
	SDL_FreeSurface(convSur);
	return tex;
}