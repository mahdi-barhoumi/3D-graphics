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
#include <engine/core/solver.hpp>
#include <engine/core/physics.hpp>
#include <engine/core/collider.hpp>
#include <engine/utils/utils.hpp>

using namespace std;
using namespace glm;
using namespace Engine;

int main(void)
{
    World world;
    Window window = Window("3D", 1280, 720);
    Solver solver = Solver();
    Renderer renderer = Renderer();

    {
        Handle handle = world.Create();
        Object camera = world.Get(handle);
        Transform transform;
        transform.TranslateTo(0, -10, 5);
        camera.Add<Transform>(transform);
        camera.Add<Camera>(Camera());
        camera.Add<Input>(Input(window));
    }

    {
        Handle handle = world.Create();
        Object surface = world.Get(handle);
        surface.Add<Transform>(Transform());
        surface.Add<Mesh>(Mesh("./assets/meshes/surface.obj"));
        surface.Add<Texture>(Texture("./assets/textures/stone.png"));
    }

    {
        Handle cubeHandle = world.Create();
        Object cube = world.Get(cubeHandle);
        Transform cubeTransform;
        cubeTransform.TranslateTo(5, 5, 1);
        cube.Add<Input>(Input(window));
        cube.Add<Transform>(cubeTransform);
        cube.Add<Mesh>(Mesh("./assets/meshes/cube.obj"));
        cube.Add<Texture>(Texture("./assets/textures/dirt.png"));
        cube.Add<Physics>(Physics(CubeCollider(2)));
    }

    {
        Handle cubeHandle = world.Create();
        Object cube = world.Get(cubeHandle);
        Transform cubeTransform;
        cubeTransform.TranslateTo(-5, -5, 1);
        cube.Add<Transform>(cubeTransform);
        cube.Add<Mesh>(Mesh("./assets/meshes/cube.obj"));
        cube.Add<Texture>(Texture("./assets/textures/stone.png"));
        cube.Add<Physics>(Physics(CubeCollider(2)));
    }

    while (!window.ShouldClose())
    {
        window.ProcessEvents();
        for (auto [handle, transform, input] : world.View<Transform, Input>())
        {
            Object object = world.Get(handle);
            if (object.Has<Camera>()) continue;
            Key key;
            while (input.HasKeys())
            {
                switch (key = input.PopFirstKey())
                {
                    case Key::UpArrow:
                        transform.TranslateBy(0, 0.1, 0);
                    break;
                    case Key::DownArrow:
                        transform.TranslateBy(0, -0.1, 0);
                    break;
                    case Key::RightArrow:
                        transform.TranslateBy(0.1, 0, 0);
                    break;
                    case Key::LeftArrow:
                        transform.TranslateBy(-0.1, 0, 0);
                    break;
                    case Key::LeftShift:
                        transform.TranslateBy(0, 0, 0.1);
                    break;
                    case Key::LeftControl:
                        transform.TranslateBy(0, 0, -0.1);
                    break;
                }
            }
        }
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
        solver.Solve(world, 0);
        renderer.Render(world, window);
        window.SwapBuffers();
    }

    return 0;
}
