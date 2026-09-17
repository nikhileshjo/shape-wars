#include "EntityManager.h"


// add to buffer
std::shared_ptr<Entity> EntityManager::addEntity(const std::string& tag)
{
    auto e = std::shared_ptr<Entity>(new Entity(tag, m_totalEntities++));
    m_toAdd.push_back(e);
    return e;
}


void EntityManager::update()
{
    // adding entites if any in buffer
    for (auto& e : m_toAdd)
    {
        m_entities.push_back(e);
        m_entityMap[e->getTag()].push_back(e);
    }
    m_toAdd.clear();
    
    // removing entities if dead
    int i = 0;
    int entityCnt = m_entities.size();
    while (i < entityCnt)
    {
        if (!m_entities[i]->isAlive())
        {
            std::string eTag = m_entities[i]->getTag();
            size_t eId = m_entities[i]->getId();
            m_entities.erase(m_entities.begin() + i);
            for (auto j = 0; j < m_entityMap[eTag].size(); j++)
            {
                if (m_entityMap[eTag][j]->getId() == eId)
                {
                    m_entityMap[eTag].erase(m_entityMap[eTag].begin() + j);
                    break;
                }
            }

            entityCnt--;
        }
        else
        {
            i++;
        }
    }
}

EntityVec& EntityManager::getEntities()
{
    return m_entities;
}

EntityVec& EntityManager::getEntities(const std::string& tag)
{
    return m_entityMap[tag];
}