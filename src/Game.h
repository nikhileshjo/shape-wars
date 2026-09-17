#pragma once
#include <iostream>
#include <fstream>

#include "imgui.h"
#include "imgui-SFML.h"

#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/System/Clock.hpp>
#include <SFML/Window/Event.hpp>
#include <SFML/Graphics.hpp>

#include "EntityManager.h"

struct PlayerConfig { 
                      int   shapeRadius,
                            collisionRadius,
                            speed,
                            fillRed,
                            fillGreen,
                            fillBlue,
                            outerRed,
                            outerGreen,
                            outerBlue,
                            outerThickness,
                            vertices;
                      float S;
                    };
struct EnemyConfig  { 
                      int   shapeRadius,
                            collisionRadius,
                            outerRed,
                            outerGreen,
                            outerBlue,
                            outerThickness,
                            minVertices,
                            maxVertices,
                            smallLifeSpan,
                            spawnInterval;
                      float minSpeed,
                            maxSpeed;
                    };
struct BulletConfig { 
                      int   shapeRadius,
                            collisionRadius,
                            fillRed,
                            fillGreen,
                            fillBlue,
                            outerRed,
                            outerGreen,
                            outerBlue,
                            outerThickness,
                            vertices,
                            lifeSpan;
                      float speed;
                    };
struct WindowConfig { unsigned width,
                               height,
                               frameLimit;
                      bool     activateDebugger,
                               fullScreen;
                    };
struct TextConfig {
                    std::string textFile;
                    int size,
                        red,
                        blue,
                        green;
                  };
struct KeybindsConfig {
                        char up = 'w',
                             left = 'a',
                             down = 's',
                             right = 'd',
                             pause = 'p';
                      };


class GameEngine
{
        EntityManager           m_entityManager;
        sf::RenderWindow        m_window;
        sf::Font                m_font;
        std::optional<sf::Text> m_text;
        PlayerConfig            m_playerConfig;
        EnemyConfig             m_enemyConfig;
        BulletConfig            m_bulletConfig;
        WindowConfig            m_windowConfig;
        KeybindsConfig          m_keybindConfig;
        TextConfig              m_textConfig;
        sf::Clock               m_deltaClock;
        int                     m_score = 0;
        int                     m_currentFrame = 0;
        int                     m_lastEnemySpawnTime = 0;
        bool                    m_paused = false;

    public:
        GameEngine(){}
        void                    init(std::string configFile);

        void                    sRender();
        void                    sDebugger();
        void                    sMovement();
        void                    sUserInput();
        void                    sLifeSpan();
        // void                    sGUI();
        void                    sEnemySpawner();
        void                    sCollision();
       
        void                    spawnPlayer();
        void                    spawnEnemy();
        void                    spawnSmallEnemies(std::shared_ptr<Entity> entity);
        void                    spawnBullet(std::shared_ptr<Entity> entity, const vec2 & mousePos);
        void                    spawnSpecialWeapon(std::shared_ptr<Entity> entity);

        std::shared_ptr<Entity>&player();

        void                    sEnemyMovement();
        void                    sBullet();
        void                    sPlayerEnemyCollision();
        void                    sBulletEnemyCollision();
        void                    run();
};
