#include <pomelo.hpp>

u32 GetNextPowerOf2(u32 v) {
    v--;
    v |= v >> 1;
    v |= v >> 2;
    v |= v >> 4;
    v |= v >> 8;
    v |= v >> 16;
    v++;
    return (v >= 64 ? v : 64);
}

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
    //texture->tex = new C3D_Tex;
    Tex3DS_Texture t3x = Tex3DS_TextureImport(data, size, &texture->tex, NULL, vram); 	
	texture->subtex = *Tex3DS_GetSubTexture(t3x, 0);
    C3D_TexSetFilter(&texture->tex, GPU_LINEAR, GPU_NEAREST);
    C3D_TexFlush(&texture->tex);
    Tex3DS_TextureFree(t3x);
    texture->loaded = true;
}


void LoadTexturePNG2D(const char * filename, PML_Image *texture, bool vram){
    FILE *file = fopen(filename, "r");
    fseek(file, 0, SEEK_END);
    long fileSize = ftell(file);
    fseek(file, 0, SEEK_SET);
    u8 *data = (u8 *)malloc((fileSize + 1) * sizeof(u8));
    fread(data, fileSize, 1, file);
    fclose(file);
    data[fileSize] = 0;
    png_image image;
    memset(&image, 0, (sizeof image));
    image.version = PNG_IMAGE_VERSION;

    if (png_image_begin_read_from_memory(&image, data, fileSize) != 0) {
        png_bytep buffer;
        image.format = PNG_FORMAT_RGBA;
        buffer = new png_byte[PNG_IMAGE_SIZE(image)];
        
        if (buffer != nullptr && png_image_finish_read(&image, nullptr, buffer, 0, nullptr) != 0) {
            if(image.width > 1024 or image.height > 1024)
                return;
            for (u32 row = 0; row < image.width; row++) {
                for (u32 col = 0; col < image.height; col++) {
                    u32 z = (row + col * image.width) * 4;
                    
                    u8 r = *(u8 *)(buffer + z);
                    u8 g = *(u8 *)(buffer + z + 1);
                    u8 b = *(u8 *)(buffer + z + 2);
                    u8 a = *(u8 *)(buffer + z + 3);
                    
                    *(buffer + z) = a;
                    *(buffer + z + 1) = b;
                    *(buffer + z + 2) = g;
                    *(buffer + z + 3) = r;
                }
            }

            texture->subtex.width = static_cast<u16>(image.width);
            texture->subtex.height = static_cast<u16>(image.height);

            u32 w_pow2 = GetNextPowerOf2(texture->subtex.width);
            u32 h_pow2 = GetNextPowerOf2(texture->subtex.height);

            texture->subtex.left = 0.f;
            texture->subtex.top = 1.f;
            texture->subtex.right = (texture->subtex.width /static_cast<float>(w_pow2));
            texture->subtex.bottom = (1.0 - (texture->subtex.height / static_cast<float>(h_pow2)));

            if(vram)
                C3D_TexInitVRAM(&texture->tex, static_cast<u16>(w_pow2), static_cast<u16>(h_pow2), GPU_RGBA8);
            else
                C3D_TexInit(&texture->tex, static_cast<u16>(w_pow2), static_cast<u16>(h_pow2), GPU_RGBA8);

            memset(texture->tex.data, 0, texture->tex.size);

            for (u32 x = 0; x < texture->subtex.width; x++) {
                for (u32 y = 0; y < texture->subtex.height; y++) {
                    u32 dst_pos = ((((y >> 3) * (w_pow2 >> 3) + (x >> 3)) << 6) + ((x & 1) | ((y & 1) << 1) | ((x & 2) << 1) | ((y & 2) << 2) | ((x & 4) << 2) | ((y & 4) << 3))) * 4;
                    u32 src_pos = (y * texture->subtex.width + x) * 4;
                    memcpy(&(static_cast<u8 *>(texture->tex.data))[dst_pos], &(static_cast<u8 *>(buffer))[src_pos], 4);
                }
            }

            C3D_TexFlush(&texture->tex);
            texture->tex.border = 0xFFFFFFFF;
            C3D_TexSetWrap(&texture->tex, GPU_CLAMP_TO_BORDER, GPU_CLAMP_TO_BORDER);

            
            texture->loaded = true;
            printf("wtf beeeeans\n");
            printf("w%d h%d\n", image.width, image.height);
            delete[] buffer;
            png_image_free(&image);
        }
        else {
            if (buffer == nullptr)
                png_image_free(&image);
            else
                delete[] buffer;
        }
    }

}


void UnloadTexture2D(PML_Image * texture){
    C3D_TexDelete(&texture->tex);
    //free(texture->tex);
    //free(&texture->subtex);
    texture->loaded = false;
}


void DrawTexture2D(PML_Image * texture, int x, int y, uint32_t color, float blend, float scale){
    if(texture->loaded){
        C2D_ImageTint TintColor;
        for(int i = 0; i < 4; i++){
            TintColor.corners[i].color = color;
            TintColor.corners[i].blend = blend;
        }
        C2D_DrawImageAt(C2D_Image{&texture->tex, &texture->subtex}, x, y, 0, &TintColor, scale, scale);
    }
}

void DrawTextureCenter2D(PML_Image * texture, int x, int y, uint32_t color, float blend, float scale){
    if(texture->loaded){
        C2D_ImageTint TintColor;
        for(int i = 0; i < 4; i++){
            TintColor.corners[i].color = color;
            TintColor.corners[i].blend = blend;
        }
        C2D_DrawImageAt(C2D_Image{&texture->tex, &texture->subtex}, x - ((float)(texture->subtex.width / 2)) * scale, y - ((float)(texture->subtex.height / 2)) * scale, 0, &TintColor, scale, scale);
    }
}

void DrawTextureRotated2D(PML_Image * texture, int x, int y, uint32_t color, float blend, float scale, float rotation){
    if(texture->loaded){
        C2D_ImageTint TintColor;
        for(int i = 0; i < 4; i++){
            TintColor.corners[i].color = color;
            TintColor.corners[i].blend = blend;
        }
        float pi = 3.141592;
        C2D_DrawImageAtRotated(C2D_Image{&texture->tex, &texture->subtex}, x, y, 0, (rotation * (pi / 180.0f)), &TintColor, scale, scale);
    }
}