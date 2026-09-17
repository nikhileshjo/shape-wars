#include "Game.h"

void GameEngine::init(std::string configFile)
{
    // reading config file
    
    std::ifstream fin(configFile);
    std::string tmp;

    while (fin >> tmp)
    {
        if (tmp == "Window")
        {
            fin >> m_windowConfig.width
                >> m_windowConfig.height
                >> m_windowConfig.frameLimit
                >> m_windowConfig.fullScreen
                >> m_windowConfig.activateDebugger;
        }
        else if (tmp == "Font")
        {
            fin >> m_textConfig.textFile
                >> m_textConfig.size
                >> m_textConfig.red
                >> m_textConfig.green
                >> m_textConfig.blue;  
        }
        else if (tmp == "Player")
        {
            fin >> m_playerConfig.shapeRadius
                >> m_playerConfig.collisionRadius
                >> m_playerConfig.speed
                >> m_playerConfig.fillRed
                >> m_playerConfig.fillGreen
                >> m_playerConfig.fillBlue
                >> m_playerConfig.outerRed
                >> m_playerConfig.outerGreen
                >> m_playerConfig.outerBlue
                >> m_playerConfig.outerThickness
                >> m_playerConfig.vertices;
        }
        else if (tmp == "Enemy")
        {
            fin >> m_enemyConfig.shapeRadius
                >> m_enemyConfig.collisionRadius
                >> m_enemyConfig.minSpeed
                >> m_enemyConfig.maxSpeed
                >> m_enemyConfig.outerRed
                >> m_enemyConfig.outerGreen
                >> m_enemyConfig.outerBlue
                >> m_enemyConfig.outerThickness
                >> m_enemyConfig.minVertices
                >> m_enemyConfig.maxVertices
                >> m_enemyConfig.smallLifeSpan
                >> m_enemyConfig.spawnInterval;
        }
        else if (tmp == "Bullet")
        {
            fin >> m_bulletConfig.shapeRadius
                >> m_bulletConfig.collisionRadius
                >> m_bulletConfig.speed
                >> m_bulletConfig.fillRed
                >> m_bulletConfig.fillGreen
                >> m_bulletConfig.fillBlue
                >> m_bulletConfig.outerRed
                >> m_bulletConfig.outerGreen
                >> m_bulletConfig.outerBlue
                >> m_bulletConfig.outerThickness
                >> m_bulletConfig.vertices
                >> m_bulletConfig.lifeSpan;
        }
        else if (tmp == "Keybinds")
        {
            fin >> m_keybindConfig.up
                >> m_keybindConfig.left
                >> m_keybindConfig.down
                >> m_keybindConfig.right
                >> m_keybindConfig.pause;
        }
        else
        {
            std::cout << "Unidentified values in config file" << std::endl;
            std::exit(1);
        }
    }

    // setup text
    if (!m_font.openFromFile(m_textConfig.textFile))
    {
        std::cout<< "Couldn't load font!" << std::endl;
        std::exit(1);
    }
    m_text.emplace(m_font, "", m_textConfig.size);


    // setup renderer
    if (m_windowConfig.fullScreen)
    {
        m_window.create(sf::VideoMode({m_windowConfig.width, m_windowConfig.height}), "Shape wars", sf::State::Fullscreen);
    }
    else
    {
        m_window.create(sf::VideoMode({m_windowConfig.width, m_windowConfig.height}), "Shape wars", sf::State::Windowed);
    }
    m_window.setFramerateLimit(m_windowConfig.frameLimit);
    m_window.setKeyRepeatEnabled(false);
    sf::Clock deltaClock;
    
    // setup player
    spawnPlayer();
}

void GameEngine::spawnPlayer()
{
    auto p = m_entityManager.addEntity("player");
    p->add<CInput>();
    p->add<CTransform>();
    p->add<CShape>();
    p->add<CCollision>();

    // set controls
    // p->get<CInput>().up = std::tolower(m_keybindConfig.up) - 'a';
    // p->get<CInput>().left = std::tolower(m_keybindConfig.left) - 'a';
    // p->get<CInput>().down = std::tolower(m_keybindConfig.down) - 'a';
    // p->get<CInput>().right = std::tolower(m_keybindConfig.right) - 'a';

    // set shape
    p->get<CShape>().shape.setPointCount(m_playerConfig.vertices);
    p->get<CShape>().shape.setRadius(m_playerConfig.shapeRadius);
    p->get<CShape>().shape.setOrigin({m_playerConfig.shapeRadius, m_playerConfig.shapeRadius});
    p->get<CShape>().shape.setFillColor(sf::Color(m_playerConfig.fillRed, m_playerConfig.fillGreen, m_playerConfig.fillBlue));
    p->get<CShape>().shape.setOutlineColor(sf::Color(m_playerConfig.outerRed, m_playerConfig.outerGreen, m_playerConfig.outerBlue));
    p->get<CShape>().shape.setOutlineThickness(m_playerConfig.outerThickness);

    // set collision radius
    p->get<CCollision>().radius = m_playerConfig.collisionRadius;
    
    // set intial trasnform
    p->get<CTransform>().position = {m_window.getSize().x/2, m_window.getSize().y/2};
    p->get<CTransform>().velocity = {0 , 0};

    p = nullptr;

    return;
}

void GameEngine::sUserInput()
{
    while (auto event = m_window.pollEvent())
    {
        auto& p = player();

        // check for window closing
        if (event ->is<sf::Event::Closed>())
        {
            std::cout << "Event closed" << std::endl;
            m_window.close();
        }

        // check for key press        
        if (const auto* keyPressed = event->getIf<sf::Event::KeyPressed>())
        {
            // print the key that was pressed to the console
            auto keyPress = int(keyPressed->scancode);
            if (keyPress == std::tolower(m_keybindConfig.up) - 'a')
            {
                std::cout << "pressed up" << std::endl;
                p->get<CInput>().up = true;
            }
            if (keyPress == std::tolower(m_keybindConfig.down) - 'a')
            {
                p->get<CInput>().down = true;
            }
            if (keyPress == std::tolower(m_keybindConfig.left) - 'a')
            {
                std::cout << "pressed left" << std::endl;
                p->get<CInput>().left = true;
            }
            if (keyPress == std::tolower(m_keybindConfig.right) - 'a')
            {
                p->get<CInput>().right = true;
            }
            if (keyPress == std::tolower(m_keybindConfig.pause) - 'a')
            {
                m_paused = !m_paused;
            }
            
            
        }

        // check key release
        if (const auto* keyReleased = event->getIf<sf::Event::KeyReleased>())
        {
            auto keyRelease = int(keyReleased->scancode);
            if (keyRelease == std::tolower(m_keybindConfig.up) - 'a')
            {
                p->get<CInput>().up = false;
            }
            if (keyRelease == std::tolower(m_keybindConfig.down) - 'a')
            {
                p->get<CInput>().down = false;
            }
            if (keyRelease == std::tolower(m_keybindConfig.left) - 'a')
            {
                p->get<CInput>().left = false;
            }
            if (keyRelease == std::tolower(m_keybindConfig.right) - 'a')
            {
                p->get<CInput>().right = false;
            }
        }
        
        // implement mouse press
    }
    return;
}

void GameEngine::sMovement()
{
    // player movement
    auto& p = player();
    auto& pVel = p->get<CTransform>().velocity;
    auto& pPos = p->get<CTransform>().position;

    // setting y direction
    if ( (p->get<CInput>().up) && ((pPos.y - p->get<CCollision>().radius) >= 0))
    {
        std::cout << "y = up" << std::endl;
        pVel.y = (-1);
        std::cout << "pVel.y = " << pVel.y << std::endl;
    }
    else if ( p->get<CInput>().down && ((pPos.y + p->get<CCollision>().radius) <= m_window.getSize().y))
    {
        pVel.y = 1;
    }
    else if (!p->get<CInput>().up)
    {
        pVel.y = 0;
    }
    else if (!p->get<CInput>().down)
    {
        pVel.y = 0;
    }

    // setting x direction
    if ( p->get<CInput>().left && ((pPos.x - p->get<CCollision>().radius) >= 0))
    {
        std::cout << "x left" << std::endl;
        pVel.x = (-1);
        std::cout << "pVel.x = " << pVel.x << std::endl;
    }
    else if ( p->get<CInput>().right && ((pPos.x + p->get<CCollision>().radius) <= m_window.getSize().x))
    {
        pVel.x = 1;
    }
    else if (!p->get<CInput>().left)
    {
        pVel.x = 0;
    }
    else if (!p->get<CInput>().right)
    {
        pVel.x = 0;
    }

    // calculate the next position
    std::cout << "player velocity: ";
    pVel.print();

    std::cout << "player position: ";
    pPos.print();
    pPos += (pVel.normalize() * m_playerConfig.speed);
    

    // enemy movement

    // bullet movement

    // for (auto& e : m_entityManager.getEntities())
    // {
    //     if (e->has<CTransform>())
    //     {

    //     }
    // }
}

std::shared_ptr<Entity>& GameEngine::player()
{
    if (m_entityManager.getEntities("player").size())
    {
        return m_entityManager.getEntities("player").back();
    }
    else
    {
        std::cout << "No player created" << std::endl;
        std::exit(1);
    }
}

void GameEngine::sRender()
{
    m_window.clear();
    for (auto& e : m_entityManager.getEntities())
    {
        auto& ePos = e->get<CTransform>().position;
        e->get<CShape>().shape.setPosition({ePos.x, ePos.y});
        m_window.draw(e->get<CShape>().shape);
    }
    // add imgui
    m_window.display();
}

void GameEngine::run()
{
    // m_window.create(sf::VideoMode({1280, 740}), "Shape wars", sf::State::Fullscreen);
    // m_window.setFramerateLimit(60);
    // m_window.setKeyRepeatEnabled(false);

    // sf::Clock deltaClock;
    
    // spawn player
    

    while (m_window.isOpen())
    {
        m_entityManager.update();
        // while (auto event = m_window.pollEvent())
        // {
        //     if (event ->is<sf::Event::Closed>())
        //     {
        //         std::cout << "Event closed" << std::endl;
        //         m_window.close();
        //     }
            
        //     // if (const auto* keyPressed = event->getIf<sf::Event::KeyPressed>())
        //     // {
        //     //     // print the key that was pressed to the console
        //     //     auto keyPress = int(keyPressed->scancode);
        //     //     sPlayerMovement(keyPress, true);
        //     // }
        //     // if (const auto* keyReleased = event->getIf<sf::Event::KeyReleased>())
        //     // {
        //     //     auto keyRelease = int(keyReleased->scancode);
        //     //     sPlayerMovement(keyRelease, false);
        //     // }                
        // }

        sUserInput();
        sMovement();
        sRender();
    }
    return;
}