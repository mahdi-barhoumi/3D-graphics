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
        Handle surfaceHandle = world.Create();
        Object surface = world.Get(surfaceHandle);
        surface.Add<Transform>(Transform());
        surface.Add<Mesh>(Mesh("./assets/meshes/surface.obj"));
        surface.Add<Texture>(Texture("./assets/textures/stone.png"));

        Handle cameraHandle = world.Create();
        Object camera = world.Get(cameraHandle);
        Transform cameraTransform;
        cameraTransform.TranslateTo(0, -10, 5);
        camera.Add<Transform>(cameraTransform);
        camera.Add<Camera>(Camera());
        camera.Add<Input>(Input(window));

        Handle cubeHandle = world.Create();
        Object cube = world.Get(cubeHandle);
        Transform cubeTransform;
        cubeTransform.TranslateTo(5, 5, 0);
        //cube.Add<Input>(Input(window));
        cube.Add<Transform>(cubeTransform);
        cube.Add<Mesh>(Mesh("./assets/meshes/cube.obj"));
        cube.Add<Texture>(Texture("./assets/textures/dirt.png"));
    }

    Renderer renderer = Renderer();

    while (!window.ShouldClose())
    {
        window.ProcessEvents();
        for (auto [handle, transform, camera, input] : world.View<Transform, Camera, Input>())
        {
            Key key;
            while (input.HasKeys())
            {
                glm::vec3 right;
                glm::vec3 forward;
                glm::vec2 direction;
                switch (input.PopFirstKey())
                {
                    case Key::Z:
                        forward = camera.GetForward();
                        direction = glm::normalize(glm::vec2(forward.x, forward.y));
                        transform.TranslateBy(direction.x * 0.2, direction.y * 0.2, 0);
                    break;
                    case Key::S:
                        forward = camera.GetForward();
                        direction = - glm::normalize(glm::vec2(forward.x, forward.y));
                        transform.TranslateBy(direction.x * 0.2, direction.y * 0.2, 0);
                    break;
                    case Key::D:
                        right = camera.GetRight();
                        direction = glm::normalize(glm::vec2(right.x, right.y));
                        transform.TranslateBy(direction.x * 0.2, direction.y * 0.2, 0);
                    break;
                    case Key::Q:
                        right = camera.GetRight();
                        direction = - glm::normalize(glm::vec2(right.x, right.y));
                        transform.TranslateBy(direction.x * 0.2, direction.y * 0.2, 0);
                    break;
                    case Key::E:
                        camera.Roll(0.5);
                    break;
                    case Key::A:
                        camera.Roll(-0.5);
                    break;
                    case Key::LeftShift:
                        transform.TranslateBy(0, 0, 0.1);
                    break;
                    case Key::LeftControl:
                        transform.TranslateBy(0, 0, -0.1);
                    break;
                }
            }
            Movement movement;
            while (input.HasMovements())
            {
                const float sensitivity = 75;
                movement = input.PopFirstMovement();
                camera.Pan(- movement.deltaX * sensitivity);
                camera.Tilt(movement.deltaY * sensitivity);
            }

        }
        renderer.Render(world, window);
        window.SwapBuffers();
    }

    return 0;
}
