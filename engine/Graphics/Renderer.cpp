#include "Renderer.h"

namespace Renderer
{
Renderer::Renderer(){
    color[0] = 0.0f;
    color[1] = 0.0f;
    color[2] = 0.0f;
    color[3] = 1.0f;
}

void Renderer::SetColor(float r, float g, float b, float a){
    color[0] = r;
    color[1] = g;
    color[2] = b;
    color[3] = a;
}
void Renderer::Clear(){

}



} // Renderer