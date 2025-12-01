#pragma once

#include <vector>

#include <object.hpp>
#include <light.hpp>
#include <camera.hpp>

class world
{
    friend class renderer;
    private:
        camera camera;
        std::vector<object> objects;
        std::vector<light> lights;
    public:
        world();
        void addObject(const object& object);
        void addLight(const light& light);
        void setCamera(const class camera& camera);
        void load(void);
        void unload(void);
        void update(void);
};
