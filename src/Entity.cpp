#include "Entity.h"



int Entity::getId()
{
    return m_id;
}

void Entity::setId(int id)
{
    m_id = id;
    return;
}

bool Entity::isAlive()
{
    return m_alive;
}

std::string Entity::getTag()
{
    return m_tag;
}

void Entity::setTag(std::string tag)
{
    m_tag = tag;
    return;
}

void Entity::destroy()
{
    m_alive = false;
    return;
}