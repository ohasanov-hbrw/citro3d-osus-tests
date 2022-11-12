#include <pomelo.hpp>


bool LoadTexture2D(const void * data, size_t size, PML_Image *texture, bool vram){
    texture->img.tex = new C3D_Tex[sizeof(C3D_Tex)];
    texture->t3x = Tex3DS_TextureImport(data, size, texture->img.tex, NULL, vram); 	
	texture->img.subtex = Tex3DS_GetSubTexture(texture->t3x, 0);
    C3D_TexSetFilter(texture->img.tex, GPU_LINEAR, GPU_NEAREST);
    C3D_TexFlush(texture->img.tex);
    return true;
}


void UnloadTexture2D(PML_Image * temp){
    Tex3DS_TextureFree(temp->t3x);
    C3D_TexDelete(temp->img.tex);
    free((void *) temp->img.tex);
    free((void *) temp->img.subtex);
}