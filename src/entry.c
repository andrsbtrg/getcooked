#include "raylib.h"
#include "raymath.h"
#include <assert.h>
#include <cstddef>
#include <stdlib.h>

typedef enum EntityArchetype {
  ARCH_NIL = 0,
  ARCH_PLAYER = 1,
  ARCH_ROCK = 2,
  ARCH_TREE = 3,
} EntityArchetype;

typedef enum SpriteID {
  SPRITE_nil,
  SPRITE_player,
  SPRITE_rock,
  SPRITE_tree,
  SPRITE_MAX
} SpriteID;

typedef struct Sprite {
  Texture2D texture;
  SpriteID id;
} Sprite;

typedef struct Entity {
  bool is_valid;
  bool render_sprite;
  Vector2 size;
  SpriteID sprite_id;
  EntityArchetype arch;
  Vector2 position;
} Entity;

#define SCREEN_HEIGHT 600
#define SCREEN_WIDTH 800
#define MAX_ENTITIES 1024

typedef struct World {
  Entity entities[MAX_ENTITIES];
} World;

inline Vector2 v2(float x, float y) { return (Vector2){x, y}; }

// :globals
World *world;
Sprite sprites[SPRITE_MAX];

void init_entities();
void update_player(Entity *player);
void setup_window(void);
void update_camera(Camera2D *, Entity *, float dt);
Entity *entity_create();

bool almost_equals(float a, float b, float epsilon) {
  return fabs(a - b) <= epsilon;
}
bool animate_f32_to_target(float *value, float target, float delta_t,
                           float rate) {
  *value += (target - *value) * (1.0 - pow(2.0f, -rate * delta_t));
  if (almost_equals(*value, target, 0.001f)) {
    *value = target;
    return true; // reached
  }
  return false;
}
void animate_v2_to_target(Vector2 *value, Vector2 target, float delta_t,
                          float rate) {
  animate_f32_to_target(&(value->x), target.x, delta_t, rate);
  animate_f32_to_target(&(value->y), target.y, delta_t, rate);
}

/* Loads the image from path into memory
 * as a texture and assigns the SpriteID id
 */
Sprite load_sprite(const char *path, SpriteID id) {
  Image image = LoadImage(path);
  if (image.data != NULL) {
    Texture2D texture = LoadTextureFromImage(image);
    UnloadImage(image);
    sprites[id] = Sprite{texture = texture, id = id};
    return sprites[id];
  }
  return sprites[SPRITE_nil];
}

/*
 * Unloads all loaded textures
 */
void unload_textures() {
  for (int i = 0; i < SPRITE_MAX; i++) {
    Texture2D texture = sprites[i].texture;
    UnloadTexture(texture);
  }
}

int main(void) {
  // Setup world
  world = (World *)malloc(sizeof(World));

  setup_window();

  init_entities();

  Entity *player = &world->entities[0];

  Camera2D camera = {0};
  camera.target =
      (Vector2){player->position.x + 20.0f, player->position.y + 20.0f};
  camera.offset = (Vector2){SCREEN_WIDTH / 2.0f, SCREEN_HEIGHT / 2.0f};
  camera.rotation = 0.0f;
  camera.zoom = 7.5f;

  SetTargetFPS(60);
  while (!WindowShouldClose()) {
    float dt = GetFrameTime();
    update_camera(&camera, player, dt);

    update_player(player);

    BeginDrawing();

    ClearBackground(DARKGRAY);

    BeginMode2D(camera);
    for (int i = 0; i < MAX_ENTITIES; i++) {
      Entity *found = &world->entities[i];
      if (NULL == found) {
        continue;
      }
      if (!found->is_valid) {
        continue;
      }
      switch (found->arch) {
      case ARCH_PLAYER: {
        DrawTextureEx(sprites[found->sprite_id].texture, found->position, 0.0,
                      1.0f, WHITE);
      }
      default: {
        DrawTextureV(sprites[found->sprite_id].texture, found->position, WHITE);
      }
      }
    }

    EndMode2D();
    DrawText(TextFormat("Player pos: [%i , %i]", (int)player->position.x,
                        (int)player->position.y),
             400, 25, 20, RED);
    EndDrawing();
  }

  unload_textures();
  CloseWindow();

  return 0;
}

void init_entities() {
  load_sprite("/home/andres/projects/game/assets/player.png", SPRITE_player);
  load_sprite("/home/andres/projects/game/assets/tree.png", SPRITE_tree);
  load_sprite("/home/andres/projects/game/assets/rock.png", SPRITE_rock);

  Entity *player = entity_create();
  player->arch = ARCH_PLAYER;
  player->position = v2(0, 0);
  player->sprite_id = SPRITE_player;

  for (int i = 0; i < 20; i++) {
    int x = GetRandomValue(-100, 100);
    int y = GetRandomValue(-100, 100);
    Entity *rock = entity_create();
    rock->arch = ARCH_ROCK;
    rock->position = v2(x, y);
    rock->sprite_id = SPRITE_rock;
  }

  Entity *tree = entity_create();
  tree->position = v2(1, 10);
  tree->arch = ARCH_TREE;
  tree->sprite_id = SPRITE_tree;

  return;
}

/// Move player
void update_player(Entity *player) {
  Vector2 input_axis = Vector2{0.0, 0.0};
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

  Vector2 initial_pos = (Vector2){player->position.x, player->position.y};
  Vector2 movement = Vector2Scale(input_axis, 1.0f);
  player->position = Vector2Add(initial_pos, movement);
  return;
}
void setup_window() {
  InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "Game");

  Vector2 scale = GetWindowScaleDPI();
  SetWindowSize(SCREEN_WIDTH / scale.x, SCREEN_HEIGHT / scale.y);
  return;
}

/*
 * Create a new entity inside the world
 */
Entity *entity_create() {
  Entity *found = 0;
  for (int i = 0; i < MAX_ENTITIES; i++) {
    Entity *existing = &world->entities[i];
    if (!existing->is_valid) {
      found = existing;
      break;
    }
  }
  assert(found); // "No free entities"
  found->is_valid = 1;
  return found;
}

void update_camera(Camera2D *camera, Entity *player, float dt) {
  animate_v2_to_target(&camera->target, player->position, dt, 15.0f);
}
