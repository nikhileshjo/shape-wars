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
    // sf::Clock deltaClock; 
    
    // setup player
    spawnPlayer();

    // initialize debugger
    if (m_windowConfig.activateDebugger)
    {
        ImGui::SFML::Init(m_window);

        // scale the imgui ui and text size by 2
        ImGui::GetStyle().ScaleAllSizes(2.0f);
        ImGui::GetIO().FontGlobalScale = 2.0f;
    }
}

void GameEngine::spawnPlayer()
{
    // reset postion if player already exists
    if (m_entityManager.getEntities("player").size())
    {
        player()->get<CTransform>().position = {m_window.getSize().x/2, m_window.getSize().y/2};
        return;
    }

    // create player if no player exists
    auto p = m_entityManager.addEntity("player");
    p->add<CInput>();
    p->add<CTransform>();
    p->add<CShape>();
    p->add<CCollision>();

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
    p->get<CTransform>().angle = 0;

    p = nullptr;

    return;
}

void GameEngine::sUserInput()
{
    while (auto event = m_window.pollEvent())
    {
        // check for window closing
        if (event ->is<sf::Event::Closed>())
        {
            m_window.close();
        }
        auto& p = player();
        if (p->has<CInput>())
        {
            // check for key press        
            if (const auto* keyPressed = event->getIf<sf::Event::KeyPressed>())
            {
                // print the key that was pressed to the console
                auto keyPress = int(keyPressed->scancode);
                if (keyPress == std::tolower(m_keybindConfig.up) - 'a')
                {
                    p->get<CInput>().up = true;
                }
                if (keyPress == std::tolower(m_keybindConfig.down) - 'a')
                {
                    p->get<CInput>().down = true;
                }
                if (keyPress == std::tolower(m_keybindConfig.left) - 'a')
                {
                    p->get<CInput>().left = true;
                }
                if (keyPress == std::tolower(m_keybindConfig.right) - 'a')
                {
                    p->get<CInput>().right = true;
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

            // mouse press
            if (const auto* mousePress = event->getIf<sf::Event::MouseButtonPressed>())
            {
                auto mouseKey = int(mousePress->button);
                if (mouseKey == 0)
                {
                    spawnBullet(player(), vec2(mousePress->position.x, mousePress->position.y));
                }
                // if mouseKey == 1 // this is right button
                // implement special ability
            }

        }

        // pause
        if (const auto* keyPressed = event->getIf<sf::Event::KeyPressed>())
        {
            auto keyPress = int(keyPressed->scancode);
            if (keyPress == std::tolower(m_keybindConfig.pause) - 'a')
            {
                m_paused = !m_paused;
            }
        }
    }
    return;
}

void GameEngine::sMovement()
{
    // player movement
    auto& p = player();
    if ( p->has<CTransform>() && p->has<CInput>() && p->has<CCollision>())
    {
        auto& pVel = p->get<CTransform>().velocity;
        auto& pPos = p->get<CTransform>().position;

        // setting y direction
        if ( (p->get<CInput>().up) && ((pPos.y - p->get<CCollision>().radius) >= 0))
        {
            pVel.y = (-1);
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
            pVel.x = (-1);
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
        pPos += (pVel.normalize() * m_playerConfig.speed);
        p->get<CTransform>().angle += m_entityRotationRate;
    }    

    // enemy movement
    for (auto& e: m_entityManager.getEntities())
    {
        if ( e-> has<CTransform>() && e->has<CCollision>() && (e->getTag() == "small-enemy" || e->getTag() == "enemy"))
        {
            auto& ePos = e->get<CTransform>().position;
            auto& eVel = e->get<CTransform>().velocity;
            auto& eColR = e->get<CCollision>().radius;
            if (ePos.x - eColR <= 0)
            {
                eVel.x = -(eVel.x);
            }
            if (ePos.x + eColR >= m_window.getSize().x)
            {
                eVel.x = -(eVel.x);
            }
            if (ePos.y - eColR <= 0)
            {
                eVel.y = -(eVel.y);
            }
            if (ePos.y + eColR >= m_window.getSize().y)
            {
                eVel.y = -(eVel.y);
            }

            // calculate new position
            ePos += eVel;
            e->get<CTransform>().angle += m_entityRotationRate;
        }
    }

    // bullet movement

    for (auto& b : m_entityManager.getEntities("bullet"))
    {
        if (b->has<CTransform>())
        {
            b->get<CTransform>().position += b->get<CTransform>().velocity;
        }
    }
}

void GameEngine::spawnEnemy()
{
    auto e = m_entityManager.addEntity("enemy");
    e->add<CTransform>();
    e->add<CShape>();
    e->add<CCollision>();
    e->add<CScore>();

    // set shape
    int vertices = (rand() % ((m_enemyConfig.maxVertices - m_enemyConfig.minVertices) + 1)) + m_enemyConfig.minVertices;
    float speed = (rand() % ((int)(m_enemyConfig.maxSpeed - m_enemyConfig.minSpeed) + 1)) + m_enemyConfig.maxSpeed;
    int fillRed = rand() % 256;
    int fillGreen = rand() % 256;
    int fillBlue = rand() % 256;
    e->get<CShape>().shape.setPointCount(vertices);
    e->get<CShape>().shape.setRadius(m_enemyConfig.shapeRadius);
    e->get<CShape>().shape.setOrigin({m_enemyConfig.shapeRadius, m_enemyConfig.shapeRadius});
    e->get<CShape>().shape.setFillColor(sf::Color(fillRed, fillGreen, fillBlue));
    e->get<CShape>().shape.setOutlineColor(sf::Color(m_enemyConfig.outerRed, m_enemyConfig.outerGreen, m_enemyConfig.outerBlue));
    e->get<CShape>().shape.setOutlineThickness(m_enemyConfig.outerThickness);

    // set collision radius
    e->get<CCollision>().radius = m_enemyConfig.collisionRadius;
    
    // set intial transform
    int minX = m_enemyConfig.collisionRadius + 1;
    int maxX = m_window.getSize().x - m_enemyConfig.collisionRadius - 1;
    float posX = (rand() % (maxX - minX)) + minX;

    int minY = m_enemyConfig.collisionRadius + 1;
    int maxY = m_window.getSize().y - m_enemyConfig.collisionRadius - 1;
    float posY = (rand() % (maxY - minY)) + minY;
    e->get<CTransform>().position = {posX, posY};
    e->get<CTransform>().velocity = {rand() , rand()};
    e->get<CTransform>().velocity = e->get<CTransform>().velocity.normalize() * speed;
    e->get<CTransform>().angle = 0;

    // set score
    e->get<CScore>().score = m_scorePerVertex * vertices;


    e = nullptr;

    return;
}

void GameEngine::sEnemySpawner()
{
    if ( m_currentFrame <= 0)
    {
        spawnEnemy();
        m_currentFrame = (m_windowConfig.frameLimit * m_enemyConfig.spawnInterval);
        return;
    }
    m_currentFrame--;
    return;    
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

void GameEngine::sDebugger()
{
    while (auto event = m_window.pollEvent())
    {
        ImGui::SFML::ProcessEvent(m_window, *event);
    }
    ImGui::SFML::Update(m_window, m_deltaClock.restart());
    // ImGui::ShowDemoWindow();
    ImGui::Begin("Hello, world!");
    ImGui::Button("Look at this pretty button");
    ImGui::End();
}

void GameEngine::sCollision()
{
    // enemy-player collision
    auto& p = player();
    if (p->has<CCollision>() && p->has<CTransform>())
    {
        auto& pPos = p->get<CTransform>().position;

        for (auto& e : m_entityManager.getEntities())
        {
            if (e->has<CCollision>() && e->has<CTransform>() && (e->getTag() == "small-enemy" || e->getTag() == "enemy"))
            {
                auto& ePos = e->get<CTransform>().position;
                float pToEDistSq = std::pow((ePos.x - pPos.x), 2) + std::pow((ePos.y - pPos.y), 2);
                if ( pToEDistSq <= std::pow(p->get<CCollision>().radius + e->get<CCollision>().radius, 2) )
                {
                    e->destroy();
                    m_score = 0;
                    spawnPlayer();
                }
            }
        }
    }

    // enemy-bullet collision
    for (auto& e : m_entityManager.getEntities())
    {
        if (e->has<CCollision>() && e->has<CTransform>() && (e->getTag() == "enemy" || e->getTag() == "small-enemy"))
        {
            auto& ePos = e->get<CTransform>().position;
            for (auto& b : m_entityManager.getEntities("bullet"))
            {
                if (b->has<CCollision>() && b->has<CTransform>())
                {
                    auto& bPos = b->get<CTransform>().position;
                    float bToEDistSq = std::pow((ePos.x - bPos.x), 2) + std::pow((ePos.y - bPos.y), 2);
                    if ( bToEDistSq <= std::pow(b->get<CCollision>().radius + e->get<CCollision>().radius, 2) )
                    {
                        if (e->getTag() == "enemy")
                        {
                            spawnSmallEnemies(e);
                        }
                        m_score += e->get<CScore>().score;
                        e->destroy();
                        b->destroy();
                    }
                }
            }
        }
    }
}

void GameEngine::sRender()
{
    m_window.clear();
    for (auto& e : m_entityManager.getEntities())
    {
        auto& eTran = e->get<CTransform>();
        e->get<CShape>().shape.setPosition({eTran.position.x, eTran.position.y});
        e->get<CShape>().shape.setRotation(sf::degrees(eTran.angle));
        if ( e->has<CLifeSpan>() )
        {
            float lifeSpanRatio = e->get<CLifeSpan>().remaining / e->get<CLifeSpan>().lifeSpan;

            sf::Color fillColor = e->get<CShape>().shape.getFillColor();
            fillColor.a = (lifeSpanRatio) * 255;
            e->get<CShape>().shape.setFillColor(fillColor);

            sf::Color outerColor = e->get<CShape>().shape.getOutlineColor();
            outerColor.a = (lifeSpanRatio) * 255;
            e->get<CShape>().shape.setOutlineColor(outerColor);
        }
        m_window.draw(e->get<CShape>().shape);
    }

    // render score
    m_text->setString(std::to_string(m_score));
    m_text->setFillColor(sf::Color(m_textConfig.red, m_textConfig.blue, m_textConfig.green));
    m_text->setPosition({0,0});
    m_text->setCharacterSize(m_textConfig.size);
    m_window.draw(*m_text);

    // render imgui
    if (m_windowConfig.activateDebugger)
    {
        ImGui::SFML::Render(m_window);
    }

    // display all the renders
    m_window.display();
}

void GameEngine::spawnBullet(std::shared_ptr<Entity> entity, const vec2& mousePos)
{
    if ( entity->has<CTransform>() )
    {
        auto b = m_entityManager.addEntity("bullet");

        //bullet setup
        b->add<CTransform>();
        b->add<CShape>();
        b->add<CCollision>();
        b->add<CLifeSpan>();

        // set bullet postion
        b->get<CTransform>().position = entity->get<CTransform>().position;

        // set bullet velocity
        auto bulletToMouse = mousePos - entity->get<CTransform>().position;
        b->get<CTransform>().velocity = ((bulletToMouse).normalize() * m_bulletConfig.speed);

        // set shape
        b->get<CShape>().shape.setRadius(m_bulletConfig.shapeRadius);
        b->get<CShape>().shape.setFillColor(sf::Color(m_bulletConfig.fillRed, m_bulletConfig.fillGreen, m_bulletConfig.fillBlue));
        b->get<CShape>().shape.setOutlineColor(sf::Color(m_bulletConfig.outerRed, m_bulletConfig.outerGreen, m_bulletConfig.outerBlue));
        b->get<CShape>().shape.setOutlineThickness(m_bulletConfig.outerThickness);
        b->get<CShape>().shape.setPointCount(m_bulletConfig.vertices);
        b->get<CShape>().shape.setOrigin(b->get<CShape>().shape.getGeometricCenter());

        // set life span
        b->get<CLifeSpan>().lifeSpan = (m_bulletConfig.lifeSpan * m_windowConfig.frameLimit);
        b->get<CLifeSpan>().remaining = (m_bulletConfig.lifeSpan * m_windowConfig.frameLimit);

        // set collision radius
        b->get<CCollision>().radius = m_bulletConfig.collisionRadius;

        b = nullptr;
    }
}

void GameEngine::sLifeSpan()
{
    for (auto& e : m_entityManager.getEntities())
    {
        if (e->has<CLifeSpan>())
        {
            e->get<CLifeSpan>().remaining--;
            if (e->get<CLifeSpan>().remaining <= 0)
            {
                e->destroy();
            }
        }
    }
    return;
}

void GameEngine::spawnSmallEnemies(std::shared_ptr<Entity> entity)
{
    if ( entity->has<CShape>() && entity->has<CTransform>())
    {
        int vertexCnt = entity->get<CShape>().shape.getPointCount();
        float radius = entity->get<CShape>().shape.getRadius() * m_smallEnemyScale;
        auto position = entity->get<CTransform>().position;
        auto fillColor = entity->get<CShape>().shape.getFillColor();

        // setup small enemies
        for (int i = 0; i < vertexCnt; i++)
        {
            auto e = m_entityManager.addEntity("small-enemy");
            e->add<CTransform>();
            e->add<CShape>();
            e->add<CCollision>();
            e->add<CScore>();
            e->add<CLifeSpan>();

            // set shape
            e->get<CShape>().shape.setPointCount(vertexCnt);
            e->get<CShape>().shape.setRadius(radius);
            e->get<CShape>().shape.setOrigin(e->get<CShape>().shape.getGeometricCenter());
            e->get<CShape>().shape.setFillColor(fillColor);
            e->get<CShape>().shape.setOutlineColor(sf::Color(m_enemyConfig.outerRed, m_enemyConfig.outerGreen, m_enemyConfig.outerBlue));
            e->get<CShape>().shape.setOutlineThickness(m_enemyConfig.outerThickness);

            // set collision radius
            e->get<CCollision>().radius = m_enemyConfig.collisionRadius * m_smallEnemyScale;

            // set intial transform
            e->get<CTransform>().position = position;
            e->get<CTransform>().velocity = (vec2().normalizedAngleVec((360 / vertexCnt) * i)) * m_smallEnemySpeed;

            // set score
            e->get<CScore>().score = m_scorePerVertex * vertexCnt * 2;

            // set lifespan
            e->get<CLifeSpan>().remaining = m_windowConfig.frameLimit * m_enemyConfig.smallLifeSpan;
            e->get<CLifeSpan>().lifeSpan = m_windowConfig.frameLimit * m_enemyConfig.smallLifeSpan;

            e = nullptr;
        }

    }
}

void GameEngine::run()
{
    // start game loop
    while (m_window.isOpen())
    {
        m_entityManager.update();
        sUserInput();
        sMovement();
        sEnemySpawner();
        sCollision();
        sLifeSpan();
        if (m_windowConfig.activateDebugger)
        {
            sDebugger();
        }
        sRender();
    }

    // ImGui::SFML::Shutdown();

    return;
}