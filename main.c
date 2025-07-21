#include <stdio.h>
#include <stdint.h>
#include <raylib.h>

#define CDT_IMPL
#include "cdt.h"

#include "entity.h"

#define WINDOW_WIDTH 900
#define WINDOW_HEIGHT 600

#define RENDER_WIDTH 450.f
#define RENDER_HEIGHT 300.f

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
    
    SetConfigFlags(FLAG_WINDOW_RESIZABLE);
    InitWindow(WINDOW_WIDTH, WINDOW_HEIGHT, "unnamed");
    SetTargetFPS(60);

    //NOTE: this is definitely not the common approach for scaled pixel art games
    //TODO: make your research for this
    RenderTexture2D surface = LoadRenderTexture(RENDER_WIDTH, RENDER_HEIGHT);

    while (!WindowShouldClose()) {
        f32 window_width = cast(f32) GetScreenWidth();
        f32 window_height = cast(f32) GetScreenHeight();
        f32 scale_factor = min(window_width / RENDER_WIDTH, window_height / RENDER_HEIGHT);
        f32 final_width = surface.texture.width * scale_factor;
        f32 final_height = surface.texture.height * scale_factor;
        i32 surface_x_offset = (window_width - final_width) / 2;
        i32 surface_y_offset = (window_height - final_height) / 2;

        Rectangle surface_src_rect = {
            .x = 0,
            .y = 0,
            .width = surface.texture.width,
            .height = -surface.texture.height,
        };
        Rectangle surface_dest_rect = {
            .x = surface_x_offset,
            .y = surface_y_offset,
            .width = final_width,
            .height = final_height,
        };

        //NOTE: how these functions work actaully??
        SetMouseOffset(-surface_x_offset, -surface_y_offset);
        SetMouseScale(1/scale_factor, 1/scale_factor);

        BeginDrawing();
        ClearBackground(BLACK);
        BeginTextureMode(surface);
        //                       poop color
        ClearBackground(GetColor(0x251818FF));
        DrawCircleV(GetMousePosition(), 20.f, GetColor(0x18186CFF));
        EndTextureMode();

        DrawTexturePro(surface.texture, surface_src_rect, surface_dest_rect, (Vector2){ 0 }, 0, WHITE);

        DrawFPS(0, 0);
        EndDrawing();
    }

    CloseWindow();

    return 0;
}
