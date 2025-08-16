#include <stdio.h>
#include <stdint.h>
#include <raylib.h>
#include <raymath.h>

#define CDT_IMPL
#include "cdt.h"

#include "entity.h"

#define WINDOW_WIDTH 900
#define WINDOW_HEIGHT 600

#define RENDER_WIDTH 450.f
#define RENDER_HEIGHT 300.f

u32 random() {
    static u32 state = 123456789;
    state ^= state << 13;
    state ^= state >> 17;
    state ^= state << 5;
    return state;
}

#define MAX_ENTITY 100
usize id_counter = 0;
Entity *entities[MAX_ENTITY];

//TODO: assetsystem
Texture textures[10];

typedef struct Knight {
    Entity handle;
    Vector2 next_target;
    f32 wait_timer;
    f32 new_wait_time;
} Knight;

#define KNIGHT_SPEED 100.f

void knight_frame(Entity *_e, f32 dt) {
    if (!_e || _e->type != EType_Knight) return;
    Knight *k = cast(Knight *) _e;

    if (Vector2Distance(k->next_target, k->handle.transform.position) < 2.f) {
        k->wait_timer += dt;
        if (k->wait_timer >= k->new_wait_time) {
            k->wait_timer = 0;
            k->new_wait_time = (random() % 3) + (random() / U32_MAX);
            k->next_target.x = random() % cast(u32) RENDER_WIDTH;
            k->next_target.y = random() % cast(u32) RENDER_HEIGHT;
        }
    }

    Vector2 diff = Vector2Subtract(k->next_target, k->handle.transform.position);
    k->handle.renderable.sprites[0].flip = diff.x < 0;
    Vector2 direction = Vector2Normalize(diff);

    k->handle.transform.position.x += direction.x * KNIGHT_SPEED * dt;
    k->handle.transform.position.y += direction.y * KNIGHT_SPEED * dt;
}

Knight *knight_create(void) {
    Knight *k = calloc(1, sizeof *k);
    entities[id_counter] = cast(Entity *) k;
    k->handle.id = id_counter++;

    k->handle.type = EType_Knight;
    k->handle.flags = EFlag_Process;
    k->handle.frame = knight_frame;
    k->handle.transform.position = (Vector2) {
        .x = random() % cast(u32) RENDER_WIDTH,
        .y = random() % cast(u32) RENDER_HEIGHT,
    };

    k->next_target = k->handle.transform.position;
    k->new_wait_time = (random() % 2) + 1;

#define ANIMATION_TIME 0.15f
    Sprite sprite = {
        .texture = 0,
        .frames[0] = {
            8, 74,
            14, 18,
            0, 0,
            ANIMATION_TIME,
        },
        .frames[1] = {
            41, 74,
            13, 18,
            0, 0,
            ANIMATION_TIME,
        },
        .frames[2] = {
            73, 74,
            13, 18,
            0, 0,
            ANIMATION_TIME,
        },
        .frames[3] = {
            105, 74,
            13, 18,
            0, 0,
            ANIMATION_TIME,
        },
        .frames[4] = {
            136, 74,
            14, 18,
            0, 0,
            ANIMATION_TIME,
        },
        .frames[5] = {
            169, 74,
            13, 18,
            0, 0,
            ANIMATION_TIME,
        },
        .frames[6] = {
            201, 74,
            13, 18,
            0, 0,
            ANIMATION_TIME,
        },
        .frames[7] = {
            233, 74,
            13, 18,
            0, 0,
            ANIMATION_TIME,
        },
        .frame_count = 8,
        .loop = true,
    };
    k->handle.renderable.sprites[0] = sprite;
    k->handle.renderable.sprite_count++;

    return k;
}

int main(void) {
    SetConfigFlags(FLAG_WINDOW_RESIZABLE);
    InitWindow(WINDOW_WIDTH, WINDOW_HEIGHT, "unnamed");
    //SetTargetFPS(60);

    textures[0] = LoadTexture("knight.png");

    //NOTE: this is definitely not the common approach for scaled pixel art games
    //TODO: make your research for this
    RenderTexture2D surface = LoadRenderTexture(RENDER_WIDTH, RENDER_HEIGHT);

    //TODO: decrease this
    printf("sizeof(struct Entity): %lu\n", sizeof(Entity));

    for (usize i = 0; i < MAX_ENTITY; i++) {
        knight_create();
    }

    while (!WindowShouldClose()) {
        f32 dt = GetFrameTime();
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

        for (usize i = 0; i < id_counter; i++) {
            Entity *e = entities[i];
            if (e->flags & EFlag_Process) e->frame(e, dt);
        }

        BeginDrawing();
        ClearBackground(BLACK);

        BeginTextureMode(surface);
        //                       poop color
        ClearBackground(GetColor(0x251818FF));

        //TODO: we gonna need layers
        for (usize i = 0; i < id_counter; i++) {
            Entity *entity = entities[i];
            Entity_Renderable *renderable = &entity->renderable;
            for (usize j = 0; j < renderable->sprite_count; j++) {
                Sprite *sprite = &renderable->sprites[j];
                Texture2D texture = textures[sprite->texture];
                if (sprite->frame_count == 1) {
                    Sprite_Frame frame = sprite->frames[0];
                    Rectangle source = {
                        frame.x, frame.y,
                        frame.w, frame.h,
                    };
                    if (sprite->flip) source.width = -source.width;
                    Rectangle dest = {
                        entity->transform.position.x + cast(f32) frame.off_x,
                        entity->transform.position.y + cast(f32) frame.off_y,
                        frame.w, frame.h,
                    };
                    Vector2 origin = { 0 };
                    DrawTexturePro(texture, source, dest, origin, entity->transform.rotation, WHITE);
                } else {
                    sprite->timer += dt;
                    Sprite_Frame frame = sprite->frames[sprite->current_frame];
                    if (sprite->timer >= frame.time) {
                        sprite->timer = 0;
                        sprite->current_frame++;
                        if (sprite->current_frame >= sprite->frame_count) {
                            if (sprite->loop) sprite->current_frame = 0;
                            else sprite->current_frame--;
                        }
                    }
                    //?
                    frame = sprite->frames[sprite->current_frame];

                    Rectangle source = {
                        frame.x, frame.y,
                        frame.w, frame.h,
                    };
                    if (sprite->flip) source.width = -source.width;
                    Rectangle dest = {
                        entity->transform.position.x + cast(f32) frame.off_x,
                        entity->transform.position.y + cast(f32) frame.off_y,
                        frame.w, frame.h,
                    };
                    Vector2 origin = { 0 };
                    DrawTexturePro(texture, source, dest, origin, entity->transform.rotation, WHITE);
                }
            }
        }
        
        DrawCircleV(GetMousePosition(), 10.f, ColorAlpha(GetColor(0x18186CFF), 0.5f));
        EndTextureMode();

        DrawTexturePro(surface.texture, surface_src_rect, surface_dest_rect, (Vector2){ 0 }, 0, WHITE);

        DrawFPS(0, 0);
        EndDrawing();
    }

    CloseWindow();

    return 0;
}
