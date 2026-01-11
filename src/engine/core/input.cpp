#include <engine/core/input.hpp>
#include <engine/core/world.hpp>
#include <engine/core/camera.hpp>
#include <engine/core/object.hpp>
#include <engine/core/window.hpp>
#include <engine/core/physics.hpp>
#include <engine/core/transform.hpp>

namespace Engine
{
    void Input::Control(World& world, Window& window, float deltaTime) const
    {
        if (window.IsKeyPressed(Key::Escape)) window.UnlockCursor();
        else if (window.IsMouseButtonPressed(MouseButton::MouseLeft)) window.LockCursor();

        Object cameraObject = world.GetActiveCamera();
        if (cameraObject.IsValid() && window.IsCursorLocked())
        {
            Camera& camera = cameraObject.Get<Camera>();
            Transform& cameraTransform = cameraObject.Get<Transform>();

            glm::vec2 direction;
            glm::vec2 forward = camera.GetWorldForward(cameraTransform.GetOrientation());
            glm::vec2 right = camera.GetWorldRight(cameraTransform.GetOrientation());
            forward = glm::normalize(forward) * 10.0f * deltaTime;
            right = glm::normalize(right) * 10.0f * deltaTime;

            if (window.IsKeyPressed(Key::Z) || window.IsKeyHeld(Key::Z)) cameraTransform.TranslateBy(glm::vec3(forward, 0.0f));
            if (window.IsKeyPressed(Key::S) || window.IsKeyHeld(Key::S)) cameraTransform.TranslateBy(glm::vec3(-forward, 0.0f));
            if (window.IsKeyPressed(Key::D) || window.IsKeyHeld(Key::D)) cameraTransform.TranslateBy(glm::vec3(right, 0.0f));
            if (window.IsKeyPressed(Key::Q) || window.IsKeyHeld(Key::Q)) cameraTransform.TranslateBy(glm::vec3(-right, 0.0f));
            if (window.IsKeyPressed(Key::E) || window.IsKeyHeld(Key::E)) camera.Roll(90.0f * deltaTime);
            if (window.IsKeyPressed(Key::A) || window.IsKeyHeld(Key::A)) camera.Roll(-90.0f * deltaTime);
            if (window.IsKeyPressed(Key::LeftShift) || window.IsKeyHeld(Key::LeftShift)) cameraTransform.TranslateBy(0, 0, 10.0f * deltaTime);
            if (window.IsKeyPressed(Key::LeftControl) || window.IsKeyHeld(Key::LeftControl)) cameraTransform.TranslateBy(0, 0, -10.0f * deltaTime);

            CursorMovement movement = window.GetCursorMovement();
            camera.Pan(movement.dx * -50.0f);
            camera.Tilt(movement.dy * 50.0f);

            if (camera.GetRoll() != 0.0f)
            {
                float deltaDegrees = 45.0f * deltaTime;
                if (deltaDegrees > std::abs(camera.GetRoll())) camera.SetRoll(0.0f);
                else camera.Roll(-deltaDegrees * camera.GetRoll() / std::abs(camera.GetRoll()));
            }
        }

        for (auto [handle, physics] : world.View<Physics, Controllable>())
        {
            if (window.IsKeyPressed(Key::UpArrow) || window.IsKeyHeld(Key::UpArrow)) physics.ApplyForce(physics.GetMass() * 50 * glm::vec3(0, 1, 0));
            if (window.IsKeyPressed(Key::DownArrow) || window.IsKeyHeld(Key::DownArrow)) physics.ApplyForce(physics.GetMass() * 50 * glm::vec3(0, -1, 0));
            if (window.IsKeyPressed(Key::RightArrow) || window.IsKeyHeld(Key::RightArrow)) physics.ApplyForce(physics.GetMass() * 50 * glm::vec3(1, 0, 0));
            if (window.IsKeyPressed(Key::LeftArrow) || window.IsKeyHeld(Key::LeftArrow)) physics.ApplyForce(physics.GetMass() * 50 * glm::vec3(-1, 0, 0));
        }
    }
}