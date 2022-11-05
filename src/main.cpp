// Simple citro2d untextured shape example

#include <3ds.h>
#include <raylib2citro3d.hpp>

#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>
#include <time.h>

#define SCREEN_WIDTH  400
#define SCREEN_HEIGHT 240

long long int secs = 0;

long long int lastMs;
struct timeval tv;


/*static C3D_Tex osu_tex;
static Tex3DS_Texture osu_texture;
static C2D_Image osu_image;*/

Renderer raylib;
Renderer::Texture2D osu_texture2d;


//---------------------------------------------------------------------------------
int main(int argc, char* argv[]) {
//---------------------------------------------------------------------------------
	// Init libs
	gfxInitDefault();
	C3D_Init(C3D_DEFAULT_CMDBUF_SIZE);
	C2D_Init(C2D_DEFAULT_MAX_OBJECTS);
	C2D_Prepare();
	consoleInit(GFX_BOTTOM, NULL);
	C3D_RenderTarget* top = C2D_CreateScreenTarget(GFX_TOP, GFX_LEFT);
	gettimeofday(&tv, NULL); 
	lastMs = tv.tv_usec;

	printf("\x1B[2J\x1B[H");
	printf("loodin teksturz\n");

	//C3D_TexLoadImage(&osu_tex, modeosu_t3x, GPU_TEXFACE_2D, 0);

	/*osu_texture = Tex3DS_TextureImport(modeosu_t3x, modeosu_t3x_size, &osu_tex, NULL, false); 	
	osu_image.tex = &osu_tex;
	osu_image.subtex = Tex3DS_GetSubTexture(osu_texture, 0);*/

	osu_texture2d = raylib.LoadTexture2D(modeosu_t3x, modeosu_t3x_size, false);

	int x = 0; 
	int y = 0; 

	C2D_ImageTint TintColor;
	for(int i = 0; i < 4; i++){
		TintColor.corners[i].color = C2D_Color32(0, 255, 0, 255);
		TintColor.corners[i].blend = 0.5f;
	}

	C2D_ImageTint Red;
	for(int i = 0; i < 4; i++){
		Red.corners[i].color = C2D_Color32(255, 0, 0, 255);
		Red.corners[i].blend = 0.5f;
	}


	int x1 = 0;
	int y1 = 0;
	int cx = 2;
	int cy = 2;

	while (aptMainLoop()){
		hidScanInput();
		u32 kDown = hidKeysDown();
		u32 kHeld = hidKeysHeld();
		if (kDown & KEY_START)
			break; // break in order to return to hbmenu

		if (kHeld & KEY_DRIGHT)
			x+=2;
		if (kHeld & KEY_DLEFT)
			x-=2;
		if (kHeld & KEY_DUP)
			y-=2;
		if (kHeld & KEY_DDOWN)
			y+=2;
		if(y > 240)	y = 240;
		if(x > 400)	x = 400;
		if(y < 0)	y = 0;
		if(x < 0)	x = 0;

		if(y1 + cy > 240-64)	cy = -2;
		if(x1 + cx > 400-64)	cx = -2;
		if(y1 + cy < 0)	cy = 2;
		if(x1 + cx < 0)	cx = 2;

		x1 += cx;
		y1 += cy;

		gettimeofday(&tv, NULL); 
		if(lastMs > tv.tv_usec){
			secs++;
		}

		long long int delta = std::abs(lastMs - tv.tv_usec);
		if(lastMs > tv.tv_usec){
			delta = std::abs(lastMs - (tv.tv_usec + 1000000));
		}
		delta /= 1000;

		if (kHeld & KEY_A){
			printf("\x1B[2J\x1B[H");
			printf("freamreta daat\n");
			printf("CPU:     %6.2f%%\n", C3D_GetProcessingTime()*6.0f);
			printf("GPU:     %6.2f%%\n", C3D_GetDrawingTime()*6.0f);
			printf("CmdBuf:  %6.2f%%\n", C3D_GetCmdBufUsage()*100.0f);
			printf("s:      %lld.%lld\n", secs, (long long int)tv.tv_usec / 1000);
			printf("delta:  %lld\n", delta);
			printf("fps:  %6.2f\n", 1000.0f / (double)delta);
		}

		lastMs = tv.tv_usec;

		C3D_FrameBegin(C3D_FRAME_SYNCDRAW);
		C2D_TargetClear(top, C2D_Color32(0x00, 0x00, 0x00, 0xFF));
		C2D_SceneBegin(top);


		
		C2D_DrawImageAt(osu_texture2d.img, x, y, 0, &TintColor, 0.5f, 0.5f);
		C2D_DrawImageAt(osu_texture2d.img, x1, y1, 0, &Red, 0.25f, 0.25f);

		C3D_FrameEnd(0);
	}

	C2D_Fini();
	C3D_Fini();
	gfxExit();
	return 0;
}