#include <assert.h>
#include <stdlib.h>

#include <cmath>
#include <cstddef>

#include "raylib.h"
#include "raymath.h"

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
  SpriteID sprite_id;
  EntityArchetype arch;
  Vector2 position;
  Vector2 size;
} Entity;

#define MAX_ENTITIES 1024

typedef struct World {
  Entity entities[MAX_ENTITIES];
} World;

inline Vector2 v2(float x, float y) {
  return (Vector2){x, y};
}

inline Rectangle get_entity_rec(Entity* entity) {
  return (Rectangle){entity->position.x, entity->position.y, entity->size.x,
                     entity->size.y};
}

// :globals
World* world;
Sprite sprites[SPRITE_MAX];

int ScreenWidth = 800;
int ScreenHeight = 600;

void init_entities();
void update_player(Entity* player);
void setup_window(void);
void setup_camera(Camera2D*);
void update_camera(Camera2D*, Entity*, float dt);

Entity* entity_create();

bool almost_equals(float a, float b, float epsilon) {
  return fabs(a - b) <= epsilon;
}
bool animate_f32_to_target(float* value,
                           float target,
                           float delta_t,
                           float rate) {
  *value += (target - *value) * (1.0 - pow(2.0f, -rate * delta_t));
  if (almost_equals(*value, target, 0.001f)) {
    *value = target;
    return true;  // reached
  }
  return false;
}
void animate_v2_to_target(Vector2* value,
                          Vector2 target,
                          float delta_t,
                          float rate) {
  animate_f32_to_target(&(value->x), target.x, delta_t, rate);
  animate_f32_to_target(&(value->y), target.y, delta_t, rate);
}

/* Loads the image from path into memory
 * as a texture and assigns the SpriteID id
 */
Sprite load_sprite(const char* path, SpriteID id) {
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

Vector2 v2_screen_to_world(Vector2 position, Camera2D camera) {
  int dpi = GetWindowScaleDPI().x;
  Matrix camera_mat = GetCameraMatrix2D(camera);
  Matrix invMatCamera = MatrixInvert(camera_mat);
  // Apply the camera's zoom and offset before transforming
  position.x += (int)(camera.offset.x / dpi);
  position.y += (int)(camera.offset.y / dpi);
  Vector3 transform =
      Vector3Transform((Vector3){position.x, position.y, 0}, invMatCamera);

  return (Vector2){dpi * transform.x, dpi * transform.y};
}
int main(void) {
  // Required so the window is not 1/4 of the screen in high dpi
  SetConfigFlags(FLAG_WINDOW_HIGHDPI);
  // Setup world
  world = (World*)malloc(sizeof(World));

  setup_window();

  init_entities();

  Entity* player = &world->entities[0];
  player->arch = ARCH_PLAYER;

  Camera2D camera = {0};
  setup_camera(&camera);

  SetTargetFPS(60);

  bool once = true;
  while (!WindowShouldClose()) {
    float dt = GetFrameTime();
    update_camera(&camera, player, dt);

    BeginDrawing();
    ClearBackground(DARKGRAY);

    update_player(player);

    BeginMode2D(camera);

    Vector2 mouse_pos_screen = GetMousePosition();
    mouse_pos_screen.x = mouse_pos_screen.x * 2;
    mouse_pos_screen.y = mouse_pos_screen.y * 2;

    // Apply DPI scaling to the mouse position
    Vector2 scaling = GetWindowScaleDPI();
    // mouse_pos_screen.x *= scaling.x;
    // mouse_pos_screen.y *= scaling.y;

    // Now get the world position
    Vector2 mouse_pos_world = v2_screen_to_world(mouse_pos_screen, camera);
    // Vector2 mouse_pos_world2 = GetScreenToWorld2D(mouse_pos_screen, camera);

    for (int i = 0; i < MAX_ENTITIES; i++) {
      Entity* entity = &world->entities[i];
      if (NULL == entity) {
        continue;
      }
      if (!entity->is_valid) {
        continue;
      }

      Rectangle rec = get_entity_rec(entity);
      switch (entity->arch) {
        default: {
          if (CheckCollisionPointRec(mouse_pos_world, rec)) {
            DrawRectangleRec(rec, RED);
          } else {
            DrawRectangleRec(rec, BLUE);
          }
          DrawTextureV(sprites[entity->sprite_id].texture, entity->position,
                       WHITE);
          DrawText(TextFormat("[%i, %i]", (int)entity->position.x,
                              (int)entity->position.y),
                   entity->position.x, entity->position.y, 1, WHITE);
          break;
        }
      }
    }

    EndMode2D();
    DrawText(TextFormat("Mouse Screen: [%i , %i]", (int)mouse_pos_screen.x,
                        (int)mouse_pos_screen.y),
             400, 25, 20, RED);
    DrawText(TextFormat("Mouse World: [%i , %i]", (int)mouse_pos_world.x,
                        (int)mouse_pos_world.y),
             100, 25, 20, BLUE);

    int offsetX = camera.offset.x / (camera.zoom * 4);
    int offsetY = camera.offset.y / (camera.zoom * 4);
    int x =
        (int)mouse_pos_world.x + (int)(camera.offset.x / camera.zoom) - offsetX;
    // (int)(camera.offset.x * 0.25 / camera.zoom);
    int y =
        (int)mouse_pos_world.y + (int)(camera.offset.y / camera.zoom) - offsetY;
    // (int)(camera.offset.y * 0.25 / camera.zoom);
    DrawText(TextFormat("Mouse Offset: [%i , %i]", x, y), 100, 45, 20, WHITE);
    EndDrawing();
  }

  unload_textures();

  free(world);
  CloseWindow();

  return 0;
}

/*
 * Create entities
 */
void init_entities() {
  load_sprite("/home/andres/projects/game/assets/player.png", SPRITE_player);
  load_sprite("/home/andres/projects/game/assets/tree.png", SPRITE_tree);
  load_sprite("/home/andres/projects/game/assets/rock.png", SPRITE_rock);

  Entity* player = entity_create();
  player->arch = ARCH_PLAYER;
  player->position = v2(0, 0);
  player->sprite_id = SPRITE_player;
  player->size = v2(6, 8);

  for (int i = 0; i < 10; i++) {
    int x = GetRandomValue(-100, 100);
    int y = GetRandomValue(-100, 100);
    Entity* rock = entity_create();
    rock->arch = ARCH_ROCK;
    rock->position = v2(x, y);
    rock->sprite_id = SPRITE_rock;
    rock->size = v2(8, 8);
  }

  Entity* tree = entity_create();
  tree->position = v2(1, 10);
  tree->arch = ARCH_TREE;
  tree->sprite_id = SPRITE_tree;
  tree->size = v2(8, 12);

  return;
}

void setup_camera(Camera2D* camera) {
  int width = GetRenderWidth();
  int height = GetRenderHeight();
  camera->offset = Vector2{width / 2.0f, height / 2.0f};
  camera->rotation = 0.0f;
  camera->target = world->entities[0].position;
  camera->zoom = 20.0;
}

/// Move player
void update_player(Entity* player) {
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
/*
 * Initialize and set the Window size
 */
void setup_window() {
  InitWindow(ScreenWidth, ScreenHeight, "Game");
  return;
}

/*
 * Create a new entity inside the world
 */
Entity* entity_create() {
  Entity* found = 0;
  for (int i = 0; i < MAX_ENTITIES; i++) {
    Entity* existing = &world->entities[i];
    if (!existing->is_valid) {
      found = existing;
      break;
    }
  }
  assert(found);  // "No free entities"
  found->is_valid = 1;
  return found;
}

void update_camera(Camera2D* camera, Entity* player, float dt) {
  animate_v2_to_target(&camera->target, player->position, dt, 5.0f);
}
