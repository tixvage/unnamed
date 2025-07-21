#ifndef _UN_ENTITY_H
#define _UN_ENTITY_H

#include "cdt.h"

//TODO: i am not sure if this system is good enough
//ENTITY SYSTEM START

typedef usize Entity_ID;

//reference: https://en.wikipedia.org/wiki/X_macro
#define LIST_OF_ENTITY_TYPES \
    X(Player) \
    X(Enemy)

typedef enum Entity_Type {
    EType_None = 0,
#define X(x) EType_ ## x,
    LIST_OF_ENTITY_TYPES
#undef X
    EType_Count,
} Entity_Type;

const char *Entity_Type_Name_Map[EType_Count] = {
#define X(x) [EType_ ## x] = #x,
    LIST_OF_ENTITY_TYPES
#undef X
};

typedef enum Entity_Flag {
    EFlag_None = 0,
    EFlag_Process = 1 << 0,
    EFlag_Renderable = 1 << 1,
    EFlag_Collidable = 1 << 2,
} Entity_Flag;

typedef struct Entity {
    Entity_ID id;
    Entity_Type type;
    Entity_Flag flags;
} Entity;

//ENTITY SYSTEM END

#endif
