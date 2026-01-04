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
        Handle surfaceHandle = world.Create();
        Object surface = world.Get(surfaceHandle);
        Transform surfaceTransform = Transform();
        surface.Add<Transform>(surfaceTransform);
        surface.Add<Mesh>(Mesh("./assets/meshes/surface.obj"));
        surface.Add<Texture>(Texture("./assets/textures/stone.png"));
        surface.Add<Physics>(Physics(PlaneCollider(100), true));
    }

    {
        Handle cubeHandle = world.Create();
        Object cube = world.Get(cubeHandle);
        Transform cubeTransform;
        cubeTransform.TranslateTo(5, 5, 20);
        cubeTransform.ScaleTo(2, 2, 2);
        cubeTransform.RotateBy(glm::radians(40.0f), glm::radians(15.0f), 0);
        cube.Add<Input>(Input(window));
        cube.Add<Transform>(cubeTransform);
        cube.Add<Mesh>(Mesh("./assets/meshes/cube.obj"));
        cube.Add<Texture>(Texture("./assets/textures/dirt.png"));
        cube.Add<Physics>(Physics(CubeCollider(2)));
    }

    for (int i = 0; i < 5; ++i)
    {
        for (int j = 0; j < 5; ++j)
        {
            Handle cubeHandle = world.Create();
            Object cube = world.Get(cubeHandle);
            Transform cubeTransform;
            cubeTransform.TranslateTo(2 * i - 10,  2 * j - 10, 10.0f);
            cube.Add<Transform>(cubeTransform);
            cube.Add<Mesh>(Mesh("./assets/meshes/cube.obj"));
            cube.Add<Texture>(Texture("./assets/textures/wood.png"));
            cube.Add<Physics>(Physics(CubeCollider(2)));
        }
    }

    float deltaTime;
    auto stop = std::chrono::high_resolution_clock::now();
    auto start = std::chrono::high_resolution_clock::now();
    while (!window.ShouldClose())
    {
        window.ProcessEvents();
        for (auto [handle, physics, input] : world.View<Physics, Input>())
        {
            Object object = world.Get(handle);
            if (object.Has<Camera>()) continue;
            Key key;
            while (input.HasKeys())
            {
                switch (key = input.PopFirstKey())
                {
                    case Key::UpArrow:
                        physics.ApplyForce(physics.GetMass() * 10 * glm::vec3(0, 1, 0));
                    break;
                    case Key::DownArrow:
                        physics.ApplyForce(physics.GetMass() * 10 * glm::vec3(0, -1, 0));
                    break;
                    case Key::RightArrow:
                        physics.ApplyForce(physics.GetMass() * 10 * glm::vec3(1, 0, 0));
                    break;
                    case Key::LeftArrow:
                        physics.ApplyForce(physics.GetMass() * 10 * glm::vec3(-1, 0, 0));
                    break;
                    case Key::LeftShift:
                        physics.ApplyForce(physics.GetMass() * 10 * glm::vec3(0, 0, 1));
                    break;
                    case Key::LeftControl:
                        physics.ApplyForce(physics.GetMass() * 10 * glm::vec3(0, 0, -1));
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
        stop = std::chrono::high_resolution_clock::now();
        deltaTime = std::chrono::duration_cast<std::chrono::nanoseconds>(stop - start).count();
        start = std::chrono::high_resolution_clock::now();
        deltaTime *= 1e-9;
        window.SetTitle(format("3D, FPS: {:.2f}", 1 / deltaTime));
        solver.Solve(world, deltaTime);
        renderer.Render(world, window);
        window.SwapBuffers();
    }

    return 0;
}
