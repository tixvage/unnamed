#include <stdio.h>
#include <stdint.h>
#include <raylib.h>

#define CDT_IMPL
#include "cdt.h"

#define WINDOW_WIDTH 900
#define WINDOW_HEIGHT 600

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

typedef struct Player {
    Entity handle;
    i32 health;
    String_View name;
} Player;

typedef struct Enemy {
    Entity handle;
    u8 damage;
    String_View name;
} Enemy;

Entity *entities[20];

int main(void) {
    usize id_counter = 0;

    {
        Player *p = create(Player);
        entities[id_counter] = cast(Entity*) p;
        p->handle.id = id_counter++;
        p->handle.type = EType_Player;
        p->handle.flags = EFlag_Process | EFlag_Renderable;
        p->health = 69;
        p->name = sv("Knaye");
        Enemy *e1 = create(Enemy);
        entities[id_counter] = cast(Entity *) e1;
        e1->handle.id = id_counter++;
        e1->handle.type = EType_Enemy;
        e1->handle.flags = EFlag_Process | EFlag_Renderable;
        e1->damage = 42;
        e1->name = sv("Yzeey");
        Enemy *e2 = create(Enemy);
        entities[id_counter] = cast(Entity *) e2;
        e2->handle.id = id_counter++;
        e2->handle.type = EType_Enemy;
        e2->handle.flags = EFlag_Process | EFlag_Renderable;
        e2->damage = 16;
        e2->name = sv("Wset");
    }

    for (usize i = 0; i < id_counter; i++) {
        Entity *handle = entities[i];
        printf("entity[%ld]:%s(%d)\n", handle->id, Entity_Type_Name_Map[handle->type], handle->type);
        //NOTE(melih): change vim's indentation for switch cases it's weird
        switch (handle->type) {
            case EType_None: {
                printf("opsi dopsi ogus bogus\n");
            } break;
            case EType_Player: {
                Player *p = cast(Player *) handle;
                printf("my name is %.*s, health: %d\n", sv_arg(p->name), p->health);
            } break;
            case EType_Enemy: {
                Enemy *e = cast(Enemy *) handle;
                printf("meine name ist %.*s, damage: %d\n", sv_arg(e->name), e->damage);
            } break;
            default: {} break;
        }
        destroy(handle);
        entities[i] = NULL;
    }

    todo("game");

    InitWindow(WINDOW_WIDTH, WINDOW_HEIGHT, "unnamed");
    SetTargetFPS(60);

    while (!WindowShouldClose()) {
        BeginDrawing();
        ClearBackground(GetColor(0x251818FF));
        DrawFPS(0, 0);
        EndDrawing();
    }

    CloseWindow();

    return 0;
}
