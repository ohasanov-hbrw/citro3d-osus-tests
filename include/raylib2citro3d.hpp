#include <citro2d.h>
#include <tex3ds.h>
#include "modeosu_t3x.h"

class Renderer{
	public:
        struct Texture2D {
            C2D_Image img;
            Tex3DS_Texture tex3ds;
            C3D_Tex tex;
        };

		Renderer();
        Texture2D LoadTexture2D(const void *, size_t, bool);
        void UnloadTexture2D(Texture2D);
	private:
		
};