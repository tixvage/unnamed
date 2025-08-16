#ifndef _UN_ENTITY_H
#define _UN_ENTITY_H

#include "cdt.h"
#include "raylib.h"

typedef struct Transform2D {
    Vector2 position;
    f32 rotation;
} Transform2D;

typedef usize Texture_Handle;

typedef struct Sprite_Frame {
    u16 x, y;
    u8 w, h;

    u8 off_x, off_y;
    f32 time;
} Sprite_Frame;

#define FRAME_PER_SPRITE 12
typedef struct Sprite {
    Texture_Handle texture;
    Sprite_Frame frames[FRAME_PER_SPRITE];
    usize frame_count;
    usize current_frame;
    bool loop, flip;
    f32 timer;
} Sprite;

#define SPRITE_PER_ENTITY 3
typedef struct Entity_Renderable {
    Sprite sprites[SPRITE_PER_ENTITY];
    usize sprite_count;
} Entity_Renderable;

//TODO: i am not sure if this system is good enough
typedef usize Entity_ID;

//reference: https://en.wikipedia.org/wiki/X_macro
#define LIST_OF_ENTITY_TYPES \
    X(Player) \
    X(Enemy) \
    X(Knight)

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
} Entity_Flag;

struct Entity;
typedef void (* Entity_Frame)(struct Entity *, f32);
typedef struct Entity {
    Entity_ID id;
    Entity_Type type;
    Entity_Flag flags;
    Entity_Frame frame;
    Transform2D transform;
    Entity_Renderable renderable;
} Entity;

#endif
