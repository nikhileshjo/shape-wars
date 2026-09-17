#pragma once
#include <iostream>
#include "Entity.h"
#include <vector>
#include <memory>
#include <map>

using EntityVec = std::vector<std::shared_ptr<Entity>>;
using EntityMap = std::map<std::string, EntityVec>;

class EntityManager
{
        EntityVec m_entities;
        EntityMap m_entityMap;
        EntityVec m_toAdd;
        size_t    m_totalEntities = 0;
    public:
        EntityManager(){}

        void                     update();                            // add/remove entites
        std::shared_ptr<Entity>  addEntity(const std::string& tag);   // add entites to buffer 
        EntityVec&               getEntities();                       // return m_entities
        EntityVec&               getEntities(const std::string& tag); // return entities of a particular tag
};