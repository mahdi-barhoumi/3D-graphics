#include <format>
#include <string>
#include <chrono>
#include <iostream>
#include <glm/glm.hpp>
#include <engine/core/mesh.hpp>
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

using namespace std;
using namespace glm;

using namespace Engine;

int main(void)
{
    World world;
    Window window = Window(1600, 900);
    Renderer renderer = Renderer();
    Solver solver = Solver();
    Input input = Input();

    {
        Object camera = world.Create();
        Transform transform;
        transform.TranslateTo(0, -10, 5);
        camera.Add<Transform>(transform);
        camera.Add<Camera>(Camera());
        world.SetActiveCamera(camera);
    }

    {
        Object surface = world.Create();
        Transform surfaceTransform = Transform();
        surface.Add<Transform>(surfaceTransform);
        surface.Add<Mesh>(Mesh("./assets/meshes/surface.obj"));
        surface.Add<Texture>(Texture("./assets/textures/stone.png"));
        surface.Add<Physics>(Physics(PlaneCollider(1000), true));
    }

    {
        Object cube = world.Create();
        Transform cubeTransform;
        cubeTransform.TranslateTo(5, 5, 20);
        cubeTransform.ScaleTo(2, 2, 2);
        cubeTransform.RotateBy(glm::radians(40.0f), glm::radians(15.0f), 0);
        cube.Add<Transform>(cubeTransform);
        cube.Add<Mesh>(Mesh("./assets/meshes/cube.obj"));
        cube.Add<Texture>(Texture("./assets/textures/dirt.png"));
        cube.Add<Physics>(Physics(CubeCollider(2), 4.0f));
        cube.Add<Controllable>();
    }

    for (int i = 0; i < 5; ++i)
    {
        for (int j = 0; j < 5; ++j)
        {
            Object cube = world.Create();
            Transform cubeTransform;
            cubeTransform.TranslateTo(2 * i - 10,  2 * j - 10, 20.0f);
            cube.Add<Transform>(cubeTransform);
            cube.Add<Mesh>(Mesh("./assets/meshes/cube.obj"));
            cube.Add<Texture>(Texture("./assets/textures/wood.png"));
            cube.Add<Physics>(Physics(CubeCollider(2)));
        }
    }

    for (int i = 0; i < 5; ++i)
    {
        for (int j = 0; j < 5; ++j)
        {
            Object sphere = world.Create();
            Transform sphereTransform;
            sphereTransform.TranslateTo(2 * i - 10,  2 * j - 10, 10.0f);
            sphere.Add<Transform>(sphereTransform);
            sphere.Add<Mesh>(Mesh("./assets/meshes/sphere.obj"));
            sphere.Add<Texture>(Texture("./assets/textures/wood.png"));
            sphere.Add<Physics>(Physics(SphereCollider(1)));
        }
    }

    float deltaTime;
    auto now = chrono::high_resolution_clock::now();
    
    while (!window.ShouldClose())
    {
        window.ProcessEvents();
        deltaTime = chrono::duration_cast<chrono::nanoseconds>(chrono::high_resolution_clock::now() - now).count() * 1e-9;
        now = chrono::high_resolution_clock::now();
        window.SetTitle(format("FPS: {:.2f}", 1 / deltaTime));
        input.Control(world, window, deltaTime);
        solver.Solve(world, deltaTime);
        renderer.Render(world, window);
    }

    return 0;
}
