#pragma once

#include <any>
#include <map>
#include <string>

namespace wak {

    class Application;

    using StateArgs = std::map<std::string, std::any>;

    class GameState
    {
    public:
        virtual ~GameState() = default;

        virtual void OnActivate(Application& app, StateArgs& args) {}
        virtual void OnUpdate(Application& app, float deltaTime) {}
        virtual void OnRender(Application& app) {}
        virtual void OnDeactivate(Application& app) {}
    };
}