#include <SDL2/SDL.h>
#include <SDL2/SDL_render.h>
#include <iostream>
#include <cstdint>
#include <vector>
#define DISP_WIDTH 224
#define DISP_HEIGHT 256
#define SIZE DISP_WIDTH*DISP_HEIGHT
using namespace std;

static uint8_t *pixels;
void setpixel(int&,int);
void setpixels(uint8_t *vram){
    int size=SIZE/8;
    int offset=0;
    uint8_t val=0;
    
    for (int y=0; y<224; y++) {
        for (int x=0; x<32; x++) {
        val = vram[(y<<5|x)];
        int off=0,offset=0;
      while(val!=0){
          offset=( DISP_WIDTH* 4 *(DISP_HEIGHT-(x<<3|off)-1)) + y * 4;
        if((val&0x1)==1)
        {
            pixels[ offset + 0 ] = 0;        // b
            pixels[ offset + 1 ] = 0;        // g
            pixels[ offset + 2 ] = 255;        // r
            pixels[ offset + 3 ] = SDL_ALPHA_OPAQUE;    // a
        }
        else
        {
            pixels[ offset + 0 ] = 0;        // b
            pixels[ offset + 1 ] = 0;        // g
            pixels[ offset + 2 ] = 0;        // r
            pixels[ offset + 3 ] = SDL_ALPHA_OPAQUE;    // a
        }
        val>>=1;
        off++;
      }
    }
    }
//cout<<"offset: "<<offset/4<<endl;
}

void setpixel(int &offset,int set){
    if(set!=0){
        //cout<<"Here!";
        pixels[offset]=255;
        pixels[++offset]=0;
        pixels[++offset]=0;
        pixels[++offset]=SDL_ALPHA_OPAQUE;
        ++offset;
    }
}
void clear(){
    for(int i=0;i<SIZE/4;i++){
        pixels[i]=0;
    }
}
void draw( int scrWidth,int scrHeight)
{
    SDL_Init( SDL_INIT_EVERYTHING );
    atexit( SDL_Quit );

    SDL_Window* window = SDL_CreateWindow(
        "SDL2",
        SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED,
        scrWidth,scrHeight,
        SDL_WINDOW_SHOWN
        );

    SDL_Renderer* renderer = SDL_CreateRenderer
        (
        window,
        -1,
        SDL_RENDERER_ACCELERATED
        );

    SDL_RendererInfo info;
    SDL_GetRendererInfo( renderer, &info );
    cout << "Renderer name: " << info.name << endl;
    cout << "Texture formats: " << endl;
    for( Uint32 i = 0; i < info.num_texture_formats; i++ )
    {
        cout << SDL_GetPixelFormatName( info.texture_formats[i] ) << endl;
    }

    
    SDL_Texture* texture = SDL_CreateTexture
        (
        renderer,
        SDL_PIXELFORMAT_ARGB8888,
        SDL_TEXTUREACCESS_STREAMING,
        DISP_WIDTH, DISP_HEIGHT
        );
    
    int size=SIZE;
    SDL_Event event;
    bool running = true;
    int cnt=0;
    while( running )
    {   //clear();
        //cout<<"Draw is Running"<<endl;
        const Uint64 start = SDL_GetPerformanceCounter();

        SDL_SetRenderDrawColor( renderer, 0, 0, 0, SDL_ALPHA_OPAQUE );
        SDL_RenderClear( renderer );

        while( SDL_PollEvent( &event ) )
        {
            if( ( SDL_QUIT == event.type ) ||
                ( SDL_KEYDOWN == event.type && SDL_SCANCODE_ESCAPE == event.key.keysym.scancode ) )
            {
                running = false;
                break;
            }
        }
       
        // splat down some random pixels
        /*for( unsigned int i = 0; i < DISP_WIDTH; i++ )
        {
            for( unsigned int j = 0; j < DISP_HEIGHT; j++ )
            {
        

            const unsigned int offset = ( DISP_HEIGHT * 4 * i ) + j * 4;
            if(i<DISP_WIDTH/2){
                pixels[ offset + 0 ] = 255;        // b
                pixels[ offset + 1 ] = 0;        // g
                pixels[ offset + 2 ] = 0;        // r
                pixels[ offset + 3 ] = SDL_ALPHA_OPAQUE;    // a
            }
            else{
                pixels[ offset + 0 ] = 0;        // b
                pixels[ offset + 1 ] = 0;        // g
                pixels[ offset + 2 ] = 0;        // r
                pixels[ offset + 3 ] = SDL_ALPHA_OPAQUE;    // a
            }
        }
        }*/

        SDL_UpdateTexture
            (
            texture,
            NULL,
            &pixels[0],
            DISP_WIDTH * 4
            );

        SDL_RenderCopy( renderer, texture, NULL, NULL );
        SDL_RenderPresent( renderer );

       /* const Uint64 end = SDL_GetPerformanceCounter();
        const static Uint64 freq = SDL_GetPerformanceFrequency();
        const double seconds = ( end - start ) / static_cast< double >( freq );
        //cout << "Frame time: " << seconds * 1000.0 << "ms" << endl;*/
    }

    SDL_DestroyRenderer( renderer );
    SDL_DestroyWindow( window );
    SDL_Quit();
}