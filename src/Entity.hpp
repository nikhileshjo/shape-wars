#include <iostream>
#include "Components.hpp"

using ComponentTuple = std::tuple<CTransform,
                                  CCollision,
                                  CScore,
                                  CShape,
                                  CLifeSpan,
                                  CInput,
                                  CSpecialAbility>;

class Entity
{
    private:
        ComponentTuple    m_components;
        bool              m_alive      = true;
        int               m_id         = 0;
        std::string       m_tag        = "default";
    public:
        // constructor
        Entity(){}

        // component functions
        template <typename T>
        T&            get();
        template <typename T>
        void          add();
        template <typename T>
        bool          has();
        template <typename T>
        void          remove();

        // entity bookkeeping
        int           getId();
        void          setId(int id);
        bool          isAlive();
        std::string   getTag();
        void          setTag(std::string tag);
        void          destroy();
};


template <typename T>
T& Entity::get()
{
    return std::get<T>(m_components);
}

template <typename T>
void Entity::add()
{
    std::get<T>(m_components).exists = true;
    return;
}

template <typename T>
bool Entity::has()
{
    return std::get<T>(m_components).exists;
}

template <typename T>
void Entity::remove()
{
    std::get<T>(m_components).exists = false;
    return;
}

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
    *this = Entity();
    m_alive = false;
    return;
}