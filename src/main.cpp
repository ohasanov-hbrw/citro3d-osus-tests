// Simple citro2d untextured shape example

#include <3ds.h>
#include <pomelo.hpp>

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
int angle = 0;
bool loaded = false;

/*static C3D_Tex osu_tex;
static Tex3DS_Texture osu_texture;
static C2D_Image osu_image;*/


PML_Image osu_texture2d;
C3D_RenderTarget* renderTarget;

int xcords[100];
int ycords[100];
int cxcords[100];
int cycords[100];
//---------------------------------------------------------------------------------
int main(int argc, char* argv[]) {
//---------------------------------------------------------------------------------
	// Init libs
	InitPML2D();
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

	LoadTexture2D(modeosu_t3x, modeosu_t3x_size, &osu_texture2d, true);
	loaded = true;

	renderTarget = C3D_RenderTargetCreateFromTex(osu_texture2d.tex, GPU_TEXFACE_2D, 0, -1);

	int x = 0; 
	int y = 0; 

	for(int i = 0; i < 100; i++){
		xcords[i] = rand() % (400 - 64);
		ycords[i] = rand() % (240 - 64);
		int cx = rand() % 2;
		if(cx == 0) cxcords[i] = -2;
		else cxcords[i] = 2;
		int cy = rand() % 2;
		if(cy == 0) cycords[i] = -2;
		else cycords[i] = 2;
	}

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


		for(int i = 0; i < 100; i++){
			if(cycords[i] + ycords[i] > 240-64)	cycords[i] = -2;
			if(cxcords[i] + xcords[i] > 400-64)	cxcords[i] = -2;
			if(cycords[i] + ycords[i] < 0)	cycords[i] = 2;
			if(cxcords[i] + xcords[i] < 0)	cxcords[i] = 2;
			ycords[i] += cycords[i];
			xcords[i] += cxcords[i];
		}

		gettimeofday(&tv, NULL); 
		if(lastMs > tv.tv_usec){
			secs++;
		}

		long long int delta = std::abs(lastMs - tv.tv_usec);
		if(lastMs > tv.tv_usec){
			delta = std::abs(lastMs - (tv.tv_usec + 1000000));
		}
		delta /= 1000;

		//if (kHeld & KEY_A){
		printf("\x1b[1;1Hfreamreta daat\x1b[K");
		printf("\x1b[2;1HCPU:     %6.2f%%\x1b[K", C3D_GetProcessingTime()*6.0f);
		printf("\x1b[3;1HGPU:     %6.2f%%\x1b[K", C3D_GetDrawingTime()*6.0f);
		printf("\x1b[4;1HCmdBuf:  %6.2f%%\x1b[K", C3D_GetCmdBufUsage()*100.0f);
		printf("\x1b[5;1Hs:      %lld.%lld\x1b[K", secs, (long long int)tv.tv_usec / 1000);
		printf("\x1b[6;1Hdelta:  %lld\x1b[K", delta);
		printf("\x1b[7;1Hfps:  %6.2f\x1b[K", 1000.0f / (double)delta);
		printf("\x1b[8;1Hangle:  %d\x1b[K", angle);
		printf("\x1b[9;1Hloaded:  %d\x1b[K", (int)loaded);
		//}

		lastMs = tv.tv_usec;

		if (kHeld & KEY_X)
			angle += 2;
		if (kHeld & KEY_Y)
			angle -= 2;

		if (kDown & KEY_A){
			UnloadTexture2D(&osu_texture2d);
			loaded = false;
		}
		if (kDown & KEY_B){
			LoadTexture2D(modeosu_t3x, modeosu_t3x_size, &osu_texture2d, true);
			loaded = true;
		}

		C3D_FrameBegin(C3D_FRAME_SYNCDRAW); // C3D_FRAME_SYNCDRAW
		C2D_Prepare();
		C2D_TargetClear(top, C2D_Color32(0x00, 0x00, 0x00, 0xFF));

		if(loaded){
			if (kDown & KEY_SELECT){
				//C2D_SceneTarget(renderTarget);
				C2D_SceneBegin(renderTarget);
				C2D_TargetClear(renderTarget, C2D_Color32(0xFF, 0xFF, 0xFF, 0xFF));
				C2D_DrawRectSolid(0, 0, 0, 10, 10, C2D_Color32(0x00, 0x00, 0xFF, 0xFF));
			}
		}
		//C2D_SceneTarget(top);
		C2D_SceneBegin(top);



		
		//if(osu_texture2d.loaded)
			//C2D_DrawImageAt(C2D_Image{osu_texture2d.tex, &osu_texture2d.subtex}, x, y, 0, &TintColor, 0.5f, 0.5f);
		DrawTextureRotated2D(&osu_texture2d, x, y, C2D_Color32(0, 255, 0, 255), 0.5f, 0.5f, angle);
		for(int i = 0; i < 15; i++)
			//if(osu_texture2d.loaded)
				//C2D_DrawImageAt(C2D_Image{osu_texture2d.tex, &osu_texture2d.subtex}, xcords[i], ycords[i], 0, &Red, 0.25f, 0.25f);
			DrawTexture2D(&osu_texture2d, xcords[i], ycords[i], C2D_Color32(255, 0, 0, 128), 0.5f, 0.25f);


		C2D_DrawRectSolid(0, 0, 0, 30, 30, C2D_Color32(0x00, 0x00, 0xFF, 0xFF));
		C2D_Flush();

		C3D_FrameEnd(0);
	}

	UnloadTexture2D(&osu_texture2d);
	StopPML2D();


	return 0;
}