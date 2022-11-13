#include <pomelo.hpp>


void InitPML2D(){
    gfxInitDefault();
	C3D_Init(C3D_DEFAULT_CMDBUF_SIZE);
	C2D_Init(C2D_DEFAULT_MAX_OBJECTS);
	C2D_Prepare();
}

void StopPML2D(){
    C2D_Fini();
	C3D_Fini();
	gfxExit();
}


void LoadTexture2D(const void * data, size_t size, PML_Image *texture, bool vram){
    texture->tex = new C3D_Tex;
    Tex3DS_Texture t3x = Tex3DS_TextureImport(data, size, texture->tex, NULL, vram); 	
	texture->subtex = *Tex3DS_GetSubTexture(t3x, 0);
    C3D_TexSetFilter(texture->tex, GPU_LINEAR, GPU_NEAREST);
    C3D_TexFlush(texture->tex);
    Tex3DS_TextureFree(t3x);
    texture->loaded = true;
}


void UnloadTexture2D(PML_Image * texture){
    C3D_TexDelete(texture->tex);
    free(texture->tex);
    //free(&texture->subtex);
    texture->loaded = false;
}