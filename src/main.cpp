// Simple citro2d untextured shape example
#include <citro2d.h>

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
	while (aptMainLoop()){
		hidScanInput();
		u32 kDown = hidKeysDown();
		if (kDown & KEY_START)
			break; // break in order to return to hbmenu
		
		gettimeofday(&tv, NULL); 
		if(lastMs > tv.tv_usec){
			secs++;
		}

		

		printf("\x1B[2J\x1B[H");
		printf("freamreta daat\n");
		printf("CPU:     %6.2f%%\n", C3D_GetProcessingTime()*6.0f);
		printf("GPU:     %6.2f%%\n", C3D_GetDrawingTime()*6.0f);
		printf("CmdBuf:  %6.2f%%\n", C3D_GetCmdBufUsage()*100.0f);
		printf("s:      %lld.%lld\n", secs, (long long int)tv.tv_usec / 1000);
		long long int delta = std::abs(lastMs - tv.tv_usec);
		if(lastMs > tv.tv_usec){
			delta = std::abs(lastMs - (tv.tv_usec + 1000000));
		}
		delta /= 1000;
		printf("delta:  %lld\n", delta);
		printf("fps:  %6.2f\n", 1000.0f / (double)delta);


		lastMs = tv.tv_usec;

		C3D_FrameBegin(C3D_FRAME_SYNCDRAW);
		C2D_TargetClear(top, C2D_Color32(0x00, 0x00, 0x00, 0xFF));
		C2D_SceneBegin(top);

		C3D_FrameEnd(0);
	}

	C2D_Fini();
	C3D_Fini();
	gfxExit();
	return 0;
}