#include "raylib.h"
#include "raymath.h"
#include <stdlib.h>

typedef enum EntityArchetype {
    ARCH_NIL = 0,
    ARCH_PLAYER = 1,
    ARCH_ROCK = 2,
    ARCH_TREE = 3,
} EntityArchetype;

typedef struct Entity {
    bool is_valid;
    EntityArchetype arch;
    Vector2 position;
} Entity;

#define MAX_ENTITIES 1024
typedef struct World {
    Entity entities[MAX_ENTITIES];
} World;

World *world;
void init_entities(World *world);
void update_player(Entity *player);
void setup_window(void);

#define PLAYER_HEIGHT 80
#define PLAYER_WIDTH 60


int main(void) {
    setup_window();

    Image player = LoadImage("../assets/player.png");
    if (player.data == NULL) {
        return 1;
    }
    Texture2D texture = LoadTextureFromImage(player);
    UnloadImage(player);

    // Setup world
    world = (World *)malloc(sizeof(World));

    init_entities(world);

    SetTargetFPS(60);
    while (!WindowShouldClose()) {
        // Handle Keys
        Entity * player = &world->entities[0];
        update_player(player);

        // Drawing
        BeginDrawing();

        ClearBackground(DARKGRAY);
        DrawTextureV(texture, player->position, WHITE);

        DrawText(TextFormat("Player pos: [%i , %i]", (int)player->position.x, (int)player->position.y),
                 400, 25, 20, RED);
        EndDrawing();
    }

    UnloadTexture(texture);
    CloseWindow();

    return 0;
}

void init_entities(World * world) {
    // initial player position
    Vector2 player_pos = { 0, 0 };

    Entity player = { 0 };
    player.arch = ARCH_PLAYER;
    player.position = player_pos;
    player.is_valid = true;

    world->entities[0] = player;

    return;
}

void update_player(Entity *player) {
    // move player
    Vector2 input_axis = Vector2 { 0.0, 0.0 };
    // left
    if (IsKeyDown(KEY_A)) {
        input_axis.x = -1;
    }
    if (IsKeyDown(KEY_D)) {
        input_axis.x = 1;
    }
    if (IsKeyDown(KEY_S)) {
        input_axis.y = 1;
    }
    if (IsKeyDown(KEY_W)) {
        input_axis.y = -1;
    }
    input_axis = Vector2Normalize(input_axis);

    Vector2 initial_pos = (Vector2) { player->position.x, player->position.y};
    Vector2 movement = Vector2Scale(input_axis, 10);
    player->position = Vector2Add(initial_pos, movement);
    return;
}
void setup_window() {
    int screenWidth = 800;
    int screenHeight = 600;
    InitWindow(screenWidth, screenHeight, "Game");

    Vector2 scale = GetWindowScaleDPI();
    SetWindowSize(screenWidth / scale.x, screenHeight / scale.y);
    return;
}
