#include "raylib.h"
#include "raymath.h"
#include <stdlib.h>
#include <assert.h>

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

inline Vector2 v2(float x, float y) { return (Vector2){x, y}; }

World *world;


void init_entities();
void update_player(Entity *player);
void setup_window(void);
void update_camera(Camera2D*, Entity*);


Entity* entity_create();

#define PLAYER_HEIGHT 8
#define PLAYER_WIDTH 6

#define SCREEN_HEIGHT 600
#define SCREEN_WIDTH 800


int main(void) {
    setup_window();

    Image rock_image = LoadImage("/home/andres/projects/game/assets/rock.png");
    if (rock_image.data == NULL) {
        return 1;
    }
    Texture2D rock_texture = LoadTextureFromImage(rock_image);
    UnloadImage(rock_image);

    Image tree_image = LoadImage("/home/andres/projects/game/assets/tree.png");
    if (tree_image.data == NULL) {
        return 1;
    }
    Texture2D tree_texture = LoadTextureFromImage(tree_image);
    UnloadImage(tree_image);

    Image player_image = LoadImage("/home/andres/projects/game/assets/player.png");
    if (player_image.data == NULL) {
        return 1;
    }
    Texture2D player_tx = LoadTextureFromImage(player_image);
    UnloadImage(player_image);

    // Setup world
    world = (World *)malloc(sizeof(World));

    init_entities();

    Entity *player = &world->entities[0];

    Camera2D camera = { 0 };
    camera.target = (Vector2){ player->position.x + 20.0f, player->position.y + 20.0f };
    camera.offset = (Vector2){ SCREEN_WIDTH/2.0f, SCREEN_HEIGHT/2.0f };
    camera.rotation = 0.0f;
    camera.zoom = 7.5f;

    SetTargetFPS(60);
    while (!WindowShouldClose()) {
        // Handle Keys
        update_player(player);
        update_camera(&camera, player);

        BeginDrawing();

        ClearBackground(DARKGRAY);

        BeginMode2D(camera);
            for (int i = 0; i< MAX_ENTITIES; i ++) {
                Entity *found = &world->entities[i];
                if (NULL == found) {
                    continue;
                }
                if (!found->is_valid) {
                    continue;
                }
                if (found->arch == ARCH_PLAYER) {
                    DrawTextureEx(player_tx, found->position, 0.0, 1.0f, WHITE); 
                }
                else if (found -> arch == ARCH_TREE) {
                    DrawTextureV(tree_texture, found->position, WHITE);
                }
                else if(found -> arch == ARCH_ROCK) {
                    DrawTextureV(rock_texture, found->position, WHITE);
                }
            }

        EndMode2D();
        DrawText(TextFormat("Player pos: [%i , %i]", (int)player->position.x, (int)player->position.y),
                 400, 25, 20, RED);
        EndDrawing();
    }

    UnloadTexture(player_tx);
    UnloadTexture(tree_texture);
    UnloadTexture(rock_texture);
    CloseWindow();

    return 0;
}

void init_entities() {
    // initial player position
    Entity* player = entity_create();
    player->arch = ARCH_PLAYER;
    player->position = v2(0,0);

    Entity *rock = entity_create();
    rock->arch = ARCH_ROCK;
    rock->position = v2(10, -5);

    Entity *tree = entity_create();
    tree->position = v2(1,10);
    tree->arch = ARCH_TREE;

    return;
}

/// Move player
void update_player(Entity *player) {
    Vector2 input_axis = Vector2 { 0.0, 0.0 };
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
    Vector2 movement = Vector2Scale(input_axis, 1.0f);
    player->position = Vector2Add(initial_pos, movement);
    return;
}
void setup_window() {
    InitWindow(SCREEN_WIDTH,SCREEN_HEIGHT, "Game");

    Vector2 scale = GetWindowScaleDPI();
    SetWindowSize(SCREEN_WIDTH / scale.x, SCREEN_HEIGHT/ scale.y);
    return;
}

/// @brief Create a new entity inside the world
/// @return The created entity
Entity* entity_create() {
  Entity *found = 0;
  for (int i = 0; i < MAX_ENTITIES; i++) {
    Entity* existing = &world->entities[i];
    if (!existing->is_valid) {
      found = existing;
      break;
    }
  }
  assert(found); // "No free entities"
  found->is_valid = 1;
  return found;
}

void update_camera(Camera2D *camera, Entity* player) {
    camera->target = player->position;
}
