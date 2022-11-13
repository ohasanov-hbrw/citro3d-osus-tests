#include <citro2d.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <tex3ds.h>
#include "modeosu_t3x.h"

struct PML_Image{
    C3D_Tex* tex;
    Tex3DS_SubTexture subtex;
    bool loaded = false;
};


void InitPML2D();
void StopPML2D();

void LoadTexture2D(const void *, size_t, PML_Image *, bool);
void UnloadTexture2D(PML_Image * );

void DrawTexture2D(PML_Image *, int, int, uint32_t, float, float);
void DrawTextureCenter2D(PML_Image *, int, int, uint32_t, float, float);
void DrawTextureRotated2D(PML_Image *, int, int, uint32_t, float, float, float);
