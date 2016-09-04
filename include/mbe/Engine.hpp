/* Copyright 2016 Marleson Graf <aszdrick@gmail.com>
   Ghabriel Nunes <ghabriel.nunes@gmail.com> */

#ifndef GAME_ENGINE_HPP
#define GAME_ENGINE_HPP

// SFML Libraries
#include <SFML/Graphics.hpp>
#include <SFML/OpenGL.hpp>
#include <SFML/Window.hpp>

// Standard Libraries
#include <string>

// Custom classes
#include "base/Game.hpp"

namespace mbe {
    using Game = base::Game<std::list<sf::Event>, sf::RenderWindow>;
    
    class Engine {
     public:
        Engine(Game* const, double = 800, double = 600,
            const std::string& = "MBE");

        void run();
        void resize(double, double);
        bool isRunning() const;

     private:
        const double WIDTH;
        const double HEIGHT;
        const std::string TITLE;
        std::unique_ptr<Game::Renderer> windowPtr;
        Game::Renderer& window;
        std::unique_ptr<Game> gamePtr;
        Game& game;

        void processEvents();
        static sf::ContextSettings desiredContextSettings();
    };
}

#endif /* GAME_ENGINE_HPP */
