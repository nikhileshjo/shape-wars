#pragma once
#include <iostream>
#include "Components.hpp"
// #include "EntityManager.hpp"  // not required as compiler concats files
// this is imported in EntityManager

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
        
        // constructor
        Entity(){}
        Entity(std::string tag, size_t id)
        :m_tag(tag),
         m_id(id)
        {}
    public:

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

        friend class EntityManager;
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