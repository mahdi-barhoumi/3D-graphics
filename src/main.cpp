#include <vector>
#include <iostream>
#include <glm/glm.hpp>
#include <engine/core/mesh.hpp>
#include <engine/core/types.hpp>
#include <engine/core/world.hpp>
#include <engine/core/input.hpp>
#include <engine/core/transform.hpp>
#include <engine/core/window.hpp>
#include <engine/core/object.hpp>
#include <engine/core/camera.hpp>
#include <engine/core/texture.hpp>
#include <engine/core/renderer.hpp>
#include <engine/utils/utils.hpp>

using namespace std;
using namespace glm;
using namespace Engine;

int main(void)
{

    World world;
    Window window = Window("3D", 1280, 720);

    {
        Handle cameraHandle = world.Create();
        Object camera = world.Get(cameraHandle);
        Transform cameraTransform;
        cameraTransform.TranslateTo(5, -10, 5);
        cameraTransform.RotateTo(glm::radians(90.0f), 0, 0);
        camera.Add<Transform>(cameraTransform);
        camera.Add<Camera>(Camera());
        camera.Add<Input>(Input(window));

        Handle cubeHandle = world.Create();
        Object cube = world.Get(cubeHandle);
        Transform cubeTransform;
        cubeTransform.TranslateTo(3, 3, 0);
        cube.Add<Input>(Input(window));
        cube.Add<Transform>(cubeTransform);
        cube.Add<Mesh>(Mesh("./assets/meshes/cube.obj"));
        cube.Add<Texture>(Texture("./assets/textures/stone.png"));
    }

    Renderer renderer = Renderer();

    while (!window.ShouldClose())
    {
        window.ProcessEvents();
        for (auto [handle, transform, input] : world.View<Transform, Input>())
        {
            Key key;
            while ((key = input.PopFirstKey()) != Key::None)
            {
                switch (key)
                {
                    case Key::Up:
                        transform.TranslateBy(0, 0.1, 0);
                    break;
                    case Key::Down:
                        transform.TranslateBy(0, -0.1, 0);
                    break;
                    case Key::Right:
                        transform.TranslateBy(0.1, 0, 0);
                    break;
                    case Key::Left:
                        transform.TranslateBy(-0.1, 0, 0);
                    break;
                }
            }
        }
        renderer.Render(world, window);
        window.SwapBuffers();
    }

    return 0;
}
