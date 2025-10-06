#include <object.hpp>
#include <light.hpp>
#include <camera.hpp>
#include <world.hpp>
#include <renderer.hpp>

int main(void)
{
    world worldScene;
    object cube("./models/cube.obj");
    worldScene.addObject(cube);
    cube.rotate(45, 45, 0);
    cube.moveTo(5, 0, 0);
    worldScene.addObject(cube);
    camera cam;
    cam.moveTo(-5, 0, 0);
    worldScene.setCamera(cam);
    renderer system(worldScene);
    system.mainloop();
    return 0;
}
