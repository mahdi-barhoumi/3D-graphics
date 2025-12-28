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
        cameraTransform.RotateTo(glm::radians(90.0f), 0, 0);
        cameraTransform.TranslateTo(5, -10, 5);
        camera.Add<Transform>(cameraTransform);
        camera.Add<Camera>(Camera());
        camera.Add<Input>(Input(window));

        Handle cubeHandle = world.Create();
        Object cube = world.Get(cubeHandle);
        Transform cubeTransform;
        cubeTransform.TranslateTo(3, 3, 0);
        //cube.Add<Input>(Input(window));
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
            while (input.HasKeys())
            {
                glm::vec3 forward;
                glm::vec2 direction;
                switch (input.PopFirstKey())
                {
                    case Key::Z:
                        forward = transform.Forward();
                        direction = glm::normalize(glm::vec2(forward.x, forward.y));
                        transform.TranslateBy(direction.x * 0.2, direction.y * 0.2, 0);
                    break;
                    case Key::S:
                        forward = transform.Forward();
                        direction = - glm::normalize(glm::vec2(forward.x, forward.y));
                        transform.TranslateBy(direction.x * 0.2, direction.y * 0.2, 0);
                    break;
                    case Key::D:
                        forward = transform.Right();
                        direction = glm::normalize(glm::vec2(forward.x, forward.y));
                        transform.TranslateBy(direction.x * 0.2, direction.y * 0.2, 0);
                    break;
                    case Key::Q:
                        forward = transform.Right();
                        direction = - glm::normalize(glm::vec2(forward.x, forward.y));
                        transform.TranslateBy(direction.x * 0.2, direction.y * 0.2, 0);
                    break;
                    case Key::E:
                        transform.Roll(0.5);
                    break;
                    case Key::A:
                        transform.Roll(-0.5);
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
                const float sensitivity = 20;
                movement = input.PopFirstMovement();
                transform.Yaw(-movement.deltaX * sensitivity);
                transform.Pitch(movement.deltaY * sensitivity);
            }

        }
        renderer.Render(world, window);
        window.SwapBuffers();
    }

    return 0;
}
