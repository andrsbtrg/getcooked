#include <assert.h>
#include <stdlib.h>

#include <cmath>
#include <cstddef>
#include <cstdio>

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

typedef struct WorldFrame {
  Entity* selected;
  Entity* near_player;
} WorldFrame;

inline Vector2 v2(float x, float y) {
  return (Vector2){x, y};
}

inline Rectangle expand_rectangle(Rectangle rec, float offset) {
  return (Rectangle){
      rec.x -= offset * 0.5,
      rec.y -= offset * 0.5,
      rec.width += offset,
      rec.height += offset,
  };
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

const float TILE_SIZE = 8.0f;

Entity* entity_create();
Sprite load_sprite(const char* path, SpriteID id);
void init_entities();
void update_player(Entity* player);
void setup_window(void);
void setup_camera(Camera2D*);
void update_camera(Camera2D*, Entity*, float dt);
void unload_textures();

// :math
bool almost_equals(float a, float b, float epsilon);

// :engine functions
Matrix get_camera_2d_mat(Camera2D camera);
Vector2 v2_screen_to_world(Vector2 screenPos, Camera2D camera);

// :animation functions
bool animate_f32_to_target(float* value,
                           float target,
                           float delta_t,
                           float rate);

void animate_v2_to_target(Vector2* value,
                          Vector2 target,
                          float delta_t,
                          float rate);

Vector2 world_to_tile_v2(Vector2 position, float tile_size) {
  return (Vector2){position.x / tile_size, position.y / tile_size};
}
Vector2 round_pos_to_tile(int x, int y, float tile_size) {
  return (Vector2){(int)(x / tile_size) * tile_size,
                   (int)(y / tile_size) * tile_size};
}

Vector2 get_entity_center(Entity* entity) {
  return (Vector2){
      entity->position.x + entity->size.x * 0.5f,
      entity->position.y + entity->size.x * 0.5f,
  };
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

  while (!WindowShouldClose()) {
    float dt = GetFrameTime();
    update_camera(&camera, player, dt);

    BeginDrawing();
    ClearBackground(Color{25, 25, 31, 0});

    update_player(player);
    WorldFrame world_frame = {0};

    // Apply DPI scaling to the mouse position
    Vector2 mouse_pos_screen = GetMousePosition();
    int dpi_scale = GetWindowScaleDPI().x;
    mouse_pos_screen.x = mouse_pos_screen.x * dpi_scale;
    mouse_pos_screen.y = mouse_pos_screen.y * dpi_scale;

    // Now get the world position
    Vector2 mouse_pos_world = v2_screen_to_world(mouse_pos_screen, camera);

    BeginMode2D(camera);

    // :tile rendering

    Vector2 player_pos_tile = world_to_tile_v2(player->position, TILE_SIZE);
    int tile_radius_x = 13;
    int tile_radius_y = 10;
    for (int i = player_pos_tile.x - tile_radius_x;
         i < player_pos_tile.x + tile_radius_x; i++) {
      for (int j = player_pos_tile.y - tile_radius_y;
           j < player_pos_tile.y + tile_radius_y; j++) {
        Rectangle rec = {TILE_SIZE * i, TILE_SIZE * j, TILE_SIZE, TILE_SIZE};

        // draw tile when hovered with mouse
        if (CheckCollisionPointRec(mouse_pos_world, rec)) {
          DrawRectangleLinesEx(rec, 0.5f, (Color){255, 255, 255, 100});
        }
        // draw checkerboard
        if (abs(j % 2) == abs(i % 2)) {
          DrawRectangleRec(rec, (Color){255, 255, 255, 10});
        }
      }
    }

    for (int i = 0; i < MAX_ENTITIES; i++) {
      Entity* entity = &world->entities[i];
      if (NULL == entity) {
        continue;
      }
      if (!entity->is_valid) {
        continue;
      }

      Rectangle rec = get_entity_rec(entity);
      Rectangle selection_rec = expand_rectangle(rec, 10);

      // :selecting
      if (CheckCollisionPointRec(mouse_pos_world, selection_rec)) {
        world_frame.selected = entity;
      }

      // :rendering
      switch (entity->arch) {
        case ARCH_PLAYER: {
          if (CheckCollisionPointRec(mouse_pos_world, rec)) {
            DrawRectangleLinesEx(rec, 1.5f, RED);
          }
          DrawTextureV(sprites[entity->sprite_id].texture, entity->position,
                       WHITE);
          break;
        }
        default: {
          if (entity == world_frame.selected) {
            DrawRectangleRec(rec, (Color){255, 255, 255, 50});
          }
          if (CheckCollisionPointRec(get_entity_center(player),
                                     selection_rec)) {
            DrawRectangleRec(rec, (Color){255, 0, 0, 50});
          }
          DrawTextureV(sprites[entity->sprite_id].texture, entity->position,
                       WHITE);
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
    rock->position = round_pos_to_tile(x, y, TILE_SIZE);
    rock->sprite_id = SPRITE_rock;
    rock->size = v2(8, 8);
  }

  for (int i = 0; i < 10; i++) {
    int x = GetRandomValue(-100, 100);
    int y = GetRandomValue(-100, 100);
    Entity* tree = entity_create();
    tree->position = round_pos_to_tile(x, y, TILE_SIZE);
    tree->arch = ARCH_TREE;
    tree->sprite_id = SPRITE_tree;
    tree->size = v2(8, 16);
  }

  return;
}

void setup_camera(Camera2D* camera) {
  int width = GetRenderWidth();
  int height = GetRenderHeight();
  int dpi_scale = GetWindowScaleDPI().x;
  camera->offset = Vector2{width / 2.0f, height / 2.0f};
  camera->rotation = 0.0f;
  camera->target = world->entities[0].position;
  camera->zoom = 5 * dpi_scale;
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

/*
 * Gets the camera 2D matrix.
 */
Matrix get_camera_2d_mat(Camera2D camera) {
  Matrix matTransform = MatrixIdentity();

  // Translate matrix to camera's target (world coordinates)
  matTransform = MatrixTranslate(-camera.target.x, -camera.target.y, 0.0f);

  // Apply camera's rotation if any (in radians)
  if (camera.rotation != 0.0f) {
    float rotationRadians = camera.rotation * DEG2RAD;
    matTransform = MatrixMultiply(matTransform, MatrixRotateZ(rotationRadians));
  }

  // Apply camera's zoom factor
  matTransform =
      MatrixMultiply(matTransform, MatrixScale(camera.zoom, camera.zoom, 1.0f));

  // Apply camera's offset (usually center of screen)
  matTransform = MatrixMultiply(
      matTransform, MatrixTranslate(camera.offset.x, camera.offset.y, 0.0f));

  return matTransform;
}

/*
 * Converts screen coordinates to world coordinates.
 * Works with high DPI monitor
 */
Vector2 v2_screen_to_world(Vector2 screenPos, Camera2D camera) {
  // Get DPI scaling for the window (if using high-DPI mode)
  int dpi = GetWindowScaleDPI().x;

  // Adjust screen coordinates for DPI scaling
  screenPos.x *= dpi;
  screenPos.y *= dpi;

  Matrix cameraMatrix = get_camera_2d_mat(camera);

  // Invert the camera matrix to reverse the transformation
  Matrix invCameraMatrix = MatrixInvert(cameraMatrix);

  // Transform the screen position into world coordinates
  Vector3 screenPos3D = {screenPos.x, screenPos.y, 0.0f};
  Vector3 worldPos3D = Vector3Transform(screenPos3D, invCameraMatrix);

  return (Vector2){worldPos3D.x, worldPos3D.y};
}

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
