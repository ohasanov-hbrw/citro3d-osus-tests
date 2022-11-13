#include <pomelo.hpp>


bool LoadTexture2D(const void * data, size_t size, PML_Image *texture, bool vram){
    texture->tex = new C3D_Tex;
    Tex3DS_Texture t3x = Tex3DS_TextureImport(data, size, texture->tex, NULL, vram); 	
	texture->subtex = *Tex3DS_GetSubTexture(t3x, 0);
    C3D_TexSetFilter(texture->tex, GPU_LINEAR, GPU_NEAREST);
    C3D_TexFlush(texture->tex);
    Tex3DS_TextureFree(t3x);
    return true;
}


void UnloadTexture2D(PML_Image * temp){
    C3D_TexDelete(temp->tex);
    free(temp->tex);
    free(&temp->subtex);
}