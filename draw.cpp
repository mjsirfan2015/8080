#include "display/draw.h"
int main(){
    uint8_t *arr=new uint8_t[SIZE/8];
    for(int i=0;i<SIZE/8;i++){
        arr[i]=0xff;
    }
    pixels=new uint8_t[SIZE*4];
    /*setpixels(arr);
    for(int i=0;i<SIZE/4;i++){
        if(i%20==0)cout<<"\n";
        else cout<<(int)pixels[i]<<" ";
    }*/
    
    setpixels(arr);
    
    draw(300,300);
    return 0;
    }