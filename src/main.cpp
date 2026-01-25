#include <format>
#include <string>
#include <chrono>
#include <iostream>
#include <engine/core/mesh.hpp>
#include <engine/core/world.hpp>
#include <engine/core/input.hpp>
#include <engine/core/audio.hpp>
#include <engine/core/transform.hpp>
#include <engine/core/window.hpp>
#include <engine/core/object.hpp>
#include <engine/core/camera.hpp>
#include <engine/core/texture.hpp>
#include <engine/core/renderer.hpp>
#include <engine/core/solver.hpp>
#include <engine/core/physics.hpp>
#include <engine/core/collider.hpp>

using namespace std;

using namespace Engine;

int main(void)
{
    World world = World();
    Window window = Window(1600, 900);
    Renderer renderer = Renderer();
    Solver solver = Solver();
    Input input = Input();

    {
        Object camera = world.Create();
        Transform transform;
        camera.Add<Transform>(Vector3(0.0f, -10.0f, 5.0f));
        camera.Add<Camera>();
        camera.Add<Audio>("./assets/sounds/explosion.ogg");
        world.SetActiveCamera(camera);
    }

    {
        Object surface = world.Create();
        surface.Add<Transform>();
        surface.Add<Mesh>("./assets/meshes/surface.obj");
        surface.Add<Texture>("./assets/textures/stone.png");
        surface.Add<Collider>(BoxCollider(1000.0f, 1000.0f, 0.0f));
        surface.Add<Physics>(Physics::Type::Stationary);
    }

    {
        Object cube = world.Create();
        Transform cubeTransform;
        cube.Add<Transform>(Vector3(5.0f, 5.0f, 20.0f), Vector3(1.0f, 2.0f, 2.0f), Quaternion(Radians(40.0f), Radians(15.0f), 0.0f));
        cube.Add<Mesh>("./assets/meshes/cube.obj");
        cube.Add<Texture>("./assets/textures/dirt.png");
        cube.Add<Collider>(BoxCollider(2.0f));
        cube.Add<Physics>(4.0f);
        cube.Add<Controllable>();
    }

    for (int i = 0; i < 5; ++i)
    {
        for (int j = 0; j < 5; ++j)
        {
            Object cube = world.Create();
            cube.Add<Transform>(Vector3(2.0f * i - 10.0f,  2.0f * j - 10.0f, 20.0f));
            cube.Add<Mesh>("./assets/meshes/cube.obj");
            cube.Add<Texture>("./assets/textures/wood.png");
            cube.Add<Collider>(BoxCollider(2.0f));
            cube.Add<Physics>();
        }
    }

    for (int i = 0; i < 5; ++i)
    {
        for (int j = 0; j < 5; ++j)
        {
            Object sphere = world.Create();
            sphere.Add<Transform>(Vector3(2.0f * i - 10.0f,  2.0f * j - 10.0f, 10.0f), Vector3(1.0f, 2.0f, 2.0f));
            sphere.Add<Mesh>("./assets/meshes/sphere.obj");
            sphere.Add<Texture>("./assets/textures/wood.png");
            sphere.Add<Collider>(SphereCollider(1));
            sphere.Add<Physics>();
        }
    }

    float deltaTime;
    auto now = chrono::high_resolution_clock::now();
    auto startTime = now;
    
    size_t frames = 0;
    while (!window.ShouldClose())
    {
        window.ProcessEvents();
        deltaTime = chrono::duration_cast<chrono::nanoseconds>(chrono::high_resolution_clock::now() - now).count() * 1e-9;
        now = chrono::high_resolution_clock::now();
        window.SetTitle(format("FPS: {:.2f}", 1.0f / deltaTime));
        input.Control(world, window, deltaTime);
        solver.Solve(world, deltaTime);
        renderer.Render(world, window);

        ++frames;
    }

    auto totalTime = chrono::duration_cast<chrono::nanoseconds>(now - startTime).count() * 1e-9;
    float averageFPS = frames / totalTime;

    cout << format("Average FPS: {:.2f}\n", averageFPS);

    return 0;
}
