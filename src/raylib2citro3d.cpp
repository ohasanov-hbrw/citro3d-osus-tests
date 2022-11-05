#include <raylib2citro3d.hpp>

Renderer::Renderer(){

}

Renderer::Texture2D Renderer::LoadTexture2D(const void * data, size_t size, bool vram){
    Texture2D temp;
    temp.tex3ds = Tex3DS_TextureImport(data, size, &temp.tex, NULL, vram); 	
	temp.img.tex = &temp.tex;
	temp.img.subtex = Tex3DS_GetSubTexture(temp.tex3ds, 0);
    return temp;
}

void Renderer::UnloadTexture2D(Texture2D){
    
}