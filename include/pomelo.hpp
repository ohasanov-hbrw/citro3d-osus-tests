
#include <citro2d.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <tex3ds.h>
#include "modeosu_t3x.h"

struct PML_Image{
    C2D_Image img;
    Tex3DS_Texture t3x;
};

bool LoadTexture2D(const void *, size_t, PML_Image *, bool);

void UnloadTexture2D(PML_Image * );
