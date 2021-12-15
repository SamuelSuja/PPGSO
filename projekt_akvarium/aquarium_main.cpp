//PPGSO kniznice:
#include <ppgso/ppgso.h>

//Headery z projektu:
#include "SceneWindow.h"

int main()
{
    // Vytvorime okno pre scenu
    SceneWindow window;

    // Loop aby ostalo okno otvorene
    while (window.pollEvents()) {}

    return 0;
}
