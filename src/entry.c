#include <assert.h>
#include <stdlib.h>

#include <cmath>
#include <cstddef>
#include <cstdio>

#include "raylib.h"
#include "raymath.h"

typedef enum EntityArchetype {
  ARCH_NIL,
  ARCH_PLAYER,
  ARCH_ROCK,
  ARCH_TREE,

  ARCH_TOMATO_PLANT,
  ARCH_PUMPKIN_PLANT,
  ARCH_CORN_PLANT,
  ARCH_PLANT,
  ARCH_SOIL,

  ARCH_ROCK_ITEM,
  ARCH_WOOD_ITEM,
  ARCH_PUMPKIN_ITEM,
  ARCH_TOMATO_ITEM,
  ARCH_CORN_ITEM,

  ARCH_CRAFT_TABLE,
  ARCH_STOCK_POT,
  ARCH_OVEN,
  ARCH_GRILL,

  ARCH_MAX,
} EntityArchetype;

typedef enum SpriteID {
  SPRITE_nil,
  SPRITE_player,
  SPRITE_rock,
  SPRITE_tree,
  SPRITE_wood,
  SPRITE_rock_item,
  SPRITE_tomato,
  SPRITE_pumpkin,
  SPRITE_plant,
  SPRITE_soil,
  SPRITE_table,
  SPRITE_pumpkin_item,
  SPRITE_tomato_item,
  SPRITE_stock_pot,
  SPRITE_oven,
  SPRITE_grill,
  SPRITE_MAX
} SpriteID;

typedef struct Sprite {
  Texture2D texture;
  SpriteID id;
} Sprite;

typedef struct Entity {
  bool is_valid;
  bool is_destroyable;
  bool is_item;
  bool render_sprite;
  SpriteID sprite_id;
  EntityArchetype arch;
  Vector2 position;
  Vector2 size;
  int health;
} Entity;

typedef struct ItemData {
  int amount;
  EntityArchetype arch;
} ItemData;

inline ItemData rock_item(int amount) {
  return (ItemData){.amount = amount, .arch = ARCH_ROCK_ITEM};
}

inline ItemData wood_item(int amount) {
  return (ItemData){.amount = amount, .arch = ARCH_WOOD_ITEM};
}

/*
 * A Crafting ID is something the player can craft while at the
 * crafting table
 */
typedef enum CraftingID {
  CRAFTING_nil,
  CRAFTING_pot,
  CRAFTING_oven,
  CRAFTING_grill,
  CRAFTING_MAX,
} CraftingID;

#define MAX_REQUIREMENTS 6
typedef struct CraftingData {
  SpriteID sprite_id;
  // This is the entity Archetype that this Crafting action will create
  EntityArchetype to_craft;
  // Time required to craft (seconds)
  float time_to_craft;
  ItemData requirements[MAX_REQUIREMENTS];
  int n_ingredient;
} CraftingData;

typedef enum UX_State {
  UX_nil,
  UX_inventory,
  UX_crafting,
  UX_cooking,
  UX_placing,
} UX_State;

#define MAX_ENTITIES 1024
typedef struct World {
  Entity entities[MAX_ENTITIES];
  ItemData inventory_items[ARCH_MAX];
  UX_State ux_state;
  CraftingID placing;
  ItemData holding;
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

const char* get_arch_name(EntityArchetype arch) {
  switch (arch) {
    case ARCH_NIL:
      return "_";
    case ARCH_PLAYER:
      return "Player";
    case ARCH_ROCK:
      return "Rock";
    case ARCH_TREE:
      return "Tree";
    case ARCH_ROCK_ITEM:
      return "Rock";
    case ARCH_WOOD_ITEM:
      return "Pine Wood";
    case ARCH_TOMATO_ITEM:
      return "Tomato";
    case ARCH_PUMPKIN_ITEM:
      return "Pumpkin";
    case ARCH_MAX:
      return "?";
    case ARCH_OVEN:
      return "Oven";
    case ARCH_STOCK_POT:
      return "Stock Pot";
    case ARCH_GRILL:
      return "Grill";
    default:
      return "";
  };
}

// :globals
World* world;
Sprite sprites[SPRITE_MAX];
CraftingData crafts[CRAFTING_MAX];
Sound destroy_sound;
Sound pickup_sound;

int ScreenWidth = 800;
int ScreenHeight = 600;

const float TILE_SIZE = 8.0f;
const float ENTITY_SELECTION_RADIUS = 10;

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
  return (Vector2){floor(x / tile_size) * tile_size,
                   floor(y / tile_size) * tile_size};
}

Vector2 get_sprite_size(SpriteID id) {
  Sprite sprite = sprites[id];
  return v2(sprite.texture.width, sprite.texture.height);
}

Vector2 get_entity_center(Entity* entity) {
  return (Vector2){
      entity->position.x + entity->size.x * 0.5f,
      entity->position.y + entity->size.x * 0.5f,
  };
}

void entity_destroy(Entity* entity) {
  entity->is_valid = false;
  entity = {0};
}

EntityArchetype get_drop_from(Entity* destroyed) {
  switch (destroyed->arch) {
    case ARCH_ROCK:
      return ARCH_ROCK_ITEM;
    case ARCH_TREE:
      return ARCH_WOOD_ITEM;
    case ARCH_TOMATO_PLANT:
      return ARCH_TOMATO_ITEM;
    case ARCH_PUMPKIN_PLANT:
      return ARCH_PUMPKIN_ITEM;
    case ARCH_CORN_PLANT:
      return ARCH_CORN_ITEM;
      // makes it so that we can move the plant
    case ARCH_PLANT:
      return ARCH_PLANT;
      // destroying soil returns nothing
    case ARCH_SOIL:
      return ARCH_NIL;
    default:
      return ARCH_NIL;
  }
}

SpriteID get_sprite_id_from_arch(EntityArchetype arch) {
  switch (arch) {
    case ARCH_NIL:
      return SPRITE_nil;
    case ARCH_PLAYER:
      return SPRITE_player;
    case ARCH_ROCK:
      return SPRITE_rock;
    case ARCH_TREE:
      return SPRITE_tree;
    case ARCH_TOMATO_PLANT:
      return SPRITE_tomato;
    case ARCH_PUMPKIN_PLANT:
      return SPRITE_pumpkin;
    case ARCH_CORN_PLANT:
      return SPRITE_nil;
    case ARCH_PLANT:
      return SPRITE_plant;
    case ARCH_SOIL:
      return SPRITE_soil;
    case ARCH_ROCK_ITEM:
      return SPRITE_rock_item;
    case ARCH_WOOD_ITEM:
      return SPRITE_wood;
    case ARCH_PUMPKIN_ITEM:
      return SPRITE_pumpkin_item;
    case ARCH_TOMATO_ITEM:
      return SPRITE_tomato_item;
    case ARCH_CORN_ITEM:
      return SPRITE_nil;
    case ARCH_CRAFT_TABLE:
      return SPRITE_table;
    case ARCH_MAX:
      break;
    case ARCH_STOCK_POT:
      return SPRITE_stock_pot;
    case ARCH_OVEN:
      return SPRITE_oven;
    case ARCH_GRILL:
      return SPRITE_grill;
      break;
  }
  return SPRITE_nil;
}

Entity* create_item_drop(Entity* destroyed) {
  EntityArchetype arch_drop = get_drop_from(destroyed);
  SpriteID sprite_id = get_sprite_id_from_arch(arch_drop);
  Vector2 size = get_sprite_size(sprite_id);

  Entity* item = entity_create();
  Vector2 destroyed_pos = destroyed->position;
  item->position = round_pos_to_tile(
      destroyed_pos.x, destroyed_pos.y + destroyed->size.y, TILE_SIZE);
  item->arch = arch_drop;
  item->sprite_id = sprite_id;
  item->size = size;
  item->is_destroyable = false;
  item->is_item = true;
  return item;
}

inline void setup_inventory() {
  for (int i = 0; i < ARCH_MAX; i++) {
    ItemData* idata = &world->inventory_items[i];
    EntityArchetype arch = (EntityArchetype)i;
    idata->amount = 0;
    idata->arch = arch;
  }
}

void load_sprites() {
  load_sprite("/home/andres/projects/game/assets/missing.png", SPRITE_nil);
  load_sprite("/home/andres/projects/game/assets/player.png", SPRITE_player);
  load_sprite("/home/andres/projects/game/assets/tree.png", SPRITE_tree);
  load_sprite("/home/andres/projects/game/assets/rock.png", SPRITE_rock);
  load_sprite("/home/andres/projects/game/assets/wood.png", SPRITE_wood);
  load_sprite("/home/andres/projects/game/assets/rock_item.png",
              SPRITE_rock_item);

  load_sprite("/home/andres/projects/game/assets/bush.png", SPRITE_tomato);
  load_sprite("/home/andres/projects/game/assets/pumpkin.png", SPRITE_pumpkin);
  load_sprite("/home/andres/projects/game/assets/plant.png", SPRITE_plant);
  load_sprite("/home/andres/projects/game/assets/soil.png", SPRITE_soil);
  load_sprite("/home/andres/projects/game/assets/kitchen.png", SPRITE_table);
  load_sprite("/home/andres/projects/game/assets/tomato_item.png",
              SPRITE_tomato_item);

  load_sprite("/home/andres/projects/game/assets/pumpkin_item.png",
              SPRITE_pumpkin_item);

  load_sprite("/home/andres/projects/game/assets/stock_pot.png",
              SPRITE_stock_pot);
  load_sprite("/home/andres/projects/game/assets/grill.png", SPRITE_grill);
  load_sprite("/home/andres/projects/game/assets/oven.png", SPRITE_oven);
}

Vector2 get_mouse_position() {
  Vector2 mouse_pos_screen = GetMousePosition();
  // Apply DPI scaling to the mouse position
  int dpi_scale = GetWindowScaleDPI().x;
  mouse_pos_screen.x = mouse_pos_screen.x * dpi_scale;
  mouse_pos_screen.y = mouse_pos_screen.y * dpi_scale;
  return mouse_pos_screen;
}

World* init_world() {
  World* world = (World*)malloc(sizeof(World));
  world->ux_state = UX_nil;
  return world;
}

bool is_cooking_system(EntityArchetype arch) {
  switch (arch) {
    case ARCH_OVEN:
    case ARCH_STOCK_POT:
    case ARCH_GRILL:
      return true;
    default:
      return false;
  }
}

bool check_craft_requirements(CraftingData craft,
                              const ItemData inventory_items[ARCH_MAX]) {
  for (int i = 0; i < craft.n_ingredient; i++) {
    ItemData req = craft.requirements[i];
    int have = inventory_items[req.arch].amount;
    if (req.amount > have) {
      return false;
    }
  }
  return true;
}

bool action_button_pressed() {
  return IsMouseButtonPressed(MOUSE_BUTTON_LEFT) || IsKeyPressed(KEY_SPACE);
}

// This is const data and should change during runtime
void setup_crafting_data() {
  crafts[CRAFTING_pot] = {.sprite_id = SPRITE_stock_pot,
                          .to_craft = ARCH_STOCK_POT,
                          .requirements = {rock_item(0), wood_item(0)},
                          .n_ingredient = 2};

  crafts[CRAFTING_oven] = {.sprite_id = SPRITE_oven,
                           .to_craft = ARCH_OVEN,
                           .requirements = {rock_item(4)},
                           .n_ingredient = 1};

  crafts[CRAFTING_grill] = {.sprite_id = SPRITE_grill,
                            .to_craft = ARCH_GRILL,
                            .requirements = {rock_item(1), wood_item(3)},
                            .n_ingredient = 2};
}

int main(void) {
  // Required so the window is not 1/4 of the screen in high dpi
  SetConfigFlags(FLAG_WINDOW_HIGHDPI);
  // :world setup
  world = init_world();

  setup_window();
  SetExitKey(0);

  InitAudioDevice();
  destroy_sound = LoadSound("/home/andres/projects/game/assets/61_Hit_03.wav");
  pickup_sound =
      LoadSound("/home/andres/projects/game/assets/001_Hover_01.wav");

  load_sprites();
  init_entities();

  Entity* player = &world->entities[0];

  // :inventory setup
  setup_inventory();
  // :crafting data setup
  setup_crafting_data();

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

    Vector2 mouse_pos_screen = get_mouse_position();

    Vector2 mouse_pos_world = v2_screen_to_world(mouse_pos_screen, camera);

    BeginMode2D(camera);

    // :tile rendering
    {
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
    }

    // :loop all entities
    for (int i = 0; i < MAX_ENTITIES; i++) {
      Entity* entity = &world->entities[i];
      if (NULL == entity) {
        continue;
      }
      if (!entity->is_valid) {
        continue;
      }

      Rectangle rec = get_entity_rec(entity);
      Rectangle selection_rec = expand_rectangle(rec, ENTITY_SELECTION_RADIUS);

      // :selecting
      if (entity->arch != ARCH_PLAYER) {
        if (CheckCollisionPointRec(mouse_pos_world, selection_rec)) {
          if (!world_frame.selected)
            world_frame.selected = entity;
        }
        if (CheckCollisionPointRec(get_entity_center(player), selection_rec)) {
          world_frame.near_player = entity;
        }
      }

      // :rendering
      switch (entity->arch) {
        case ARCH_PLAYER: {
          DrawTextureV(sprites[entity->sprite_id].texture, entity->position,
                       WHITE);
          break;
        }
        default: {
          Color tile_color = {0};
          if (entity == world_frame.selected) {
            tile_color = (Color){255, 255, 255, 50};
          }
          if (entity == world_frame.near_player) {
            tile_color = (Color){255, 0, 0, 50};
          }
          DrawRectangleRec(rec, tile_color);
          if (entity->is_item) {
            // float
            float offset = sin(4 * GetTime());
            Vector2 pos = v2(entity->position.x, entity->position.y + offset);
            DrawTextureV(sprites[entity->sprite_id].texture, pos, WHITE);
          } else {
            DrawTextureV(sprites[entity->sprite_id].texture, entity->position,
                         WHITE);
          }
          break;
        }
      }
    }

    // :ux_state detection
    {
      // UX states like craft and cook depend on being close to certain entity
      if (world_frame.near_player) {
        EntityArchetype arch = world_frame.near_player->arch;
        if (world->ux_state != UX_placing) {
          if (arch == ARCH_CRAFT_TABLE) {
            world->ux_state = UX_crafting;
          } else if (is_cooking_system(arch)) {
            world->ux_state = UX_cooking;
          }
        }
      }
      // closes UX states that depend on being close to an entity
      // if we are not close to any entity
      else if (world->ux_state == UX_crafting ||
               world->ux_state == UX_cooking) {
        world->ux_state = UX_nil;
        world->holding = {0};
      }

      // handle open and close inventory
      if (IsKeyPressed(KEY_I) || IsKeyPressed(KEY_TAB)) {
        world->ux_state = UX_inventory;
      } else if (IsKeyPressed(KEY_ESCAPE)) {
        world->ux_state = UX_nil;
        world->holding = {0};
      }
    }

    // :pickup items
    {
      if (world_frame.near_player) {
        Entity* entity_near = world_frame.near_player;
        if (entity_near->is_valid && entity_near->is_item) {
          // TODO: Pickup animation
          PlaySound(pickup_sound);

          world->inventory_items[entity_near->arch].amount += 1;
          entity_destroy(entity_near);
        }
      }
    }

    // :clicking
    {
      if (world->ux_state == UX_nil && action_button_pressed()) {
        Entity* target = NULL;
        // If there are selected item and item near player
        // it will prefer to destroy the item near player
        if (world_frame.near_player) {
          target = world_frame.near_player;
        } else if (world_frame.selected) {
          target = world_frame.selected;
        }
        if (target && target->is_destroyable) {
          PlaySound(destroy_sound);
          // :destroy
          target->health--;
          if (target->health <= 0) {
            Entity* item = create_item_drop(target);
            entity_destroy(target);
          }
        }
      }
    }

    EndMode2D();

    // :ui quick access
    if (world->ux_state == UX_nil) {
      int item_pos = 0;
      for (int i = 0; i < ARCH_MAX; i++) {
        ItemData inventory_item = world->inventory_items[i];
        if (inventory_item.amount == 0)
          continue;
        Vector2 texture_pos =
            v2(-50 + ScreenWidth / 2.0 + 50 * item_pos, ScreenHeight - 45);
        Vector2 text_pos = v2(texture_pos.x, texture_pos.y);

        Rectangle rec = (Rectangle){texture_pos.x, texture_pos.y, 40, 40};

        Color rec_color = (Color){245, 245, 245, 50};
        if (CheckCollisionPointRec(mouse_pos_screen, rec)) {
          float offset = 4 + sin(4 * GetTime());
          texture_pos = v2(texture_pos.x, texture_pos.y - offset);
          rec_color = GOLD;
          rec_color.a = 50;
          DrawText(get_arch_name(world->inventory_items[i].arch), text_pos.x,
                   text_pos.y - 20, 18, WHITE);
        }
        DrawRectangleRec(rec, rec_color);

        DrawTextureEx(
            sprites[get_sprite_id_from_arch(inventory_item.arch)].texture,
            texture_pos, 0, 5, WHITE);
        DrawText(TextFormat("[%i]", world->inventory_items[i].amount),
                 text_pos.x, text_pos.y + 20, 20, WHITE);
        item_pos++;
      }
    }
    // :ui inventory
    else if (world->ux_state == UX_inventory) {
      int item_pos = 0;
      int fontsize = 20;
      const char* text = "Press ESC to exit inventory";
      int text_width = MeasureText(text, fontsize);
      DrawText(text, ScreenWidth / 2.0 - text_width / 2.0, 20, fontsize, WHITE);
      for (int i = 0; i < ARCH_MAX; i++) {
        if (world->inventory_items[i].amount == 0)
          continue;
        ItemData inventory_item = world->inventory_items[i];
        Vector2 texture_pos = v2(-50 + ScreenWidth / 2.0 + 50 * item_pos,
                                 ScreenHeight / 2.0 - 40);
        Vector2 text_pos = v2(texture_pos.x, texture_pos.y);

        Rectangle rec = (Rectangle){texture_pos.x, texture_pos.y, 40, 40};

        Color rec_color = (Color){245, 245, 245, 50};
        if (CheckCollisionPointRec(mouse_pos_screen, rec)) {
          float offset = 4 + sin(4 * GetTime());
          texture_pos = v2(texture_pos.x, texture_pos.y - offset);
          rec_color = GOLD;
          rec_color.a = 50;
          // TODO: Fix ui animation
          // animate_v2_to_target(
          //     &texture_pos, v2(texture_pos.x, texture_pos.y - 100),
          //     dt, 1.0f);
          DrawText(get_arch_name(world->inventory_items[i].arch), text_pos.x,
                   text_pos.y - 20, 18, WHITE);
        }
        DrawRectangleRec(rec, rec_color);

        DrawTextureEx(
            sprites[get_sprite_id_from_arch(inventory_item.arch)].texture,
            texture_pos, 0, 5, WHITE);
        DrawText(TextFormat("[%i]", world->inventory_items[i].amount),
                 text_pos.x, text_pos.y + 20, 20, WHITE);
        item_pos++;
      }
    }
    // :ui crafting
    else if (world->ux_state == UX_crafting) {
      const char* text = "Crafting...";
      int fontsize = 20;
      int text_width = MeasureText(text, fontsize);
      DrawText(text, ScreenWidth / 2.0 - text_width / 2.0, 20, fontsize, WHITE);

      Color rec_color = (Color){0, 0, 0, 200};
      int padding = 10;
      float icon_size = 40.0f;
      float ui_origin_x = ScreenWidth * 0.5f - 0.5f * 5.0f * icon_size;
      float ui_origin_y = ScreenHeight * 0.2f;
      Rectangle ui_container = {ui_origin_x, ui_origin_y, 6 * icon_size,
                                4 * icon_size};

      // container rec
      DrawRectangleRec(ui_container, rec_color);

      // draw crafting options
      for (int i = 1; i < CRAFTING_MAX; i++) {
        CraftingID craft_id = (CraftingID)i;
        CraftingData craft = crafts[craft_id];
        Vector2 texture_pos = v2(ui_origin_x + 50 * i, ScreenHeight * 0.5);
        Vector2 text_pos = v2(ui_container.x, ui_container.y);

        Rectangle icon_rec =
            (Rectangle){texture_pos.x, texture_pos.y, icon_size, icon_size};

        DrawRectangleRec(icon_rec, rec_color);
        Texture2D texture = sprites[craft.sprite_id].texture;
        // :placing craft
        if (CheckCollisionPointRec(mouse_pos_screen, icon_rec) &&
            IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
          if (check_craft_requirements(craft, world->inventory_items)) {
            world->ux_state = UX_placing;
            world->placing = craft_id;
          }
        }

        if (CheckCollisionPointRec(mouse_pos_screen, icon_rec)) {
          float offset = 4 + sin(4 * GetTime());
          texture_pos = v2(texture_pos.x, texture_pos.y - offset);
          rec_color = GOLD;
          rec_color.a = 50;
          const char* text = get_arch_name(craft.to_craft);
          float text_length = MeasureText(text, 20);
          DrawText(get_arch_name(craft.to_craft), text_pos.x, text_pos.y - 24,
                   20, WHITE);

          for (int j = 0; j < craft.n_ingredient; j++) {
            Color text_color = RED;
            ItemData req = craft.requirements[j];
            int player_has = world->inventory_items[req.arch].amount;
            if (player_has >= req.amount) {
              text_color = WHITE;
            }
            Texture texture =
                sprites[get_sprite_id_from_arch(req.arch)].texture;
            DrawTexturePro(
                texture,
                (Rectangle){0, 0, (float)texture.width, (float)texture.height},
                {ui_origin_x + padding, ui_origin_y + padding + j * icon_size,
                 icon_size, icon_size},
                v2(0, 0), 0, WHITE);
            DrawText(TextFormat("%i / %i", req.amount, player_has),
                     ui_origin_x + padding + icon_size,
                     ui_origin_y + padding + j * icon_size, 18, text_color);
          }
        }
        DrawTexturePro(
            texture,
            (Rectangle){0, 0, (float)texture.width, (float)texture.height},
            {texture_pos.x, texture_pos.y, icon_rec.width, icon_rec.height},
            v2(0, 0), 0, WHITE);
      }
    }
    // :ui placing
    else if (world->ux_state == UX_placing) {
      const char* text = "Placing...\nPress ESC to exit Placing mode";
      int fontsize = 20;
      int text_width = MeasureText(text, fontsize);
      DrawText(text, ScreenWidth / 2.0 - text_width / 2.0, 20, fontsize, WHITE);

      if (world->placing != CRAFTING_nil) {
        CraftingData craft = crafts[world->placing];
        Texture texture = sprites[craft.sprite_id].texture;

        int world_factor = camera.zoom / GetWindowScaleDPI().x;

        // the tile size on this case is on Screen space size
        float tile_size = TILE_SIZE * world_factor;
        Vector2 pos = round_pos_to_tile(mouse_pos_screen.x, mouse_pos_screen.y,
                                        tile_size);

        float width = texture.width * world_factor;
        float height = texture.height * world_factor;
        DrawTexturePro(
            texture,
            (Rectangle){0, 0, (float)texture.width, (float)texture.height},
            {pos.x, pos.y, width, height}, v2(0, 0), 0,
            (Color){255, 255, 255, 124});

        if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
          // place craft
          // FIXME: this doesn't clear all the entities struct previous data
          Entity* en = entity_create();
          en->arch = craft.to_craft;
          en->position = round_pos_to_tile(mouse_pos_world.x, mouse_pos_world.y,
                                           TILE_SIZE);
          en->sprite_id = craft.sprite_id;
          en->size = get_sprite_size(craft.sprite_id);
          en->is_destroyable = false;
          en->is_item = false;
          en->health = 100;
          // exit craft mode
          world->ux_state = UX_nil;

          // reduce the inventory
          for (int x = 0; x < craft.n_ingredient; x++) {
            ItemData req = craft.requirements[x];
            world->inventory_items[req.arch].amount -= req.amount;
          }
        }
      }
    }
    // :ui cooking
    else if (world->ux_state == UX_cooking) {
      const char* text = "Cooking...";
      int fontsize = 20;
      int text_width = MeasureText(text, fontsize);
      DrawText(text, ScreenWidth / 2.0 - text_width / 2.0, 20, fontsize, WHITE);

      int item_pos = 0;
      for (int i = 0; i < ARCH_MAX; i++) {
        ItemData inventory_item = world->inventory_items[i];
        if (inventory_item.amount == 0)
          continue;
        Vector2 texture_pos =
            v2(-50 + ScreenWidth / 2.0 + 50 * item_pos, ScreenHeight - 45);
        Vector2 text_pos = v2(texture_pos.x, texture_pos.y);

        Rectangle rec = (Rectangle){texture_pos.x, texture_pos.y, 40, 40};

        Color rec_color = (Color){245, 245, 245, 50};
        if (CheckCollisionPointRec(mouse_pos_screen, rec)) {
          float offset = 4 + sin(4 * GetTime());
          texture_pos = v2(texture_pos.x, texture_pos.y - offset);
          rec_color = GOLD;
          rec_color.a = 50;
          DrawText(get_arch_name(world->inventory_items[i].arch), text_pos.x,
                   text_pos.y - 20, 18, WHITE);
          if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
            ItemData holdprev = world->holding;
            if (holdprev.arch == inventory_item.arch) {
              if (world->holding.amount < inventory_item.amount) {
                world->holding.amount += 1;
              }
            } else {
              // if (holdprev.arch == ARCH_NIL) {
              world->holding = {.amount = 1, .arch = inventory_item.arch};
            }
          }
        }
        DrawRectangleRec(rec, rec_color);

        DrawTextureEx(
            sprites[get_sprite_id_from_arch(inventory_item.arch)].texture,
            texture_pos, 0, 5, WHITE);
        DrawText(TextFormat("[%i]", world->inventory_items[i].amount),
                 text_pos.x, text_pos.y + 20, 20, WHITE);
        item_pos++;
      }
      if (world->holding.arch != ARCH_NIL) {
        Vector2 sprite_pos =
            v2(mouse_pos_screen.x - 20, mouse_pos_screen.y - 20);
        ItemData holding = world->holding;
        DrawTextureEx(sprites[get_sprite_id_from_arch(holding.arch)].texture,
                      sprite_pos, 0, 5, WHITE);
        DrawText(TextFormat("%i", holding.amount), sprite_pos.x, sprite_pos.y,
                 20, WHITE);
      }
    }

    // :ui dbg
    {
      // DrawText(TextFormat("Mouse World: [%i , %i]", (int)mouse_pos_world.x,
      //                     (int)mouse_pos_world.y),
      //          400, 25, 20, RED);
      // Vector2 dbg_pos =
      //     round_pos_to_tile(mouse_pos_world.x, mouse_pos_world.y,
      //     TILE_SIZE);
      //
      // DrawText(
      //     TextFormat("Tile pos: [%i , %i]", (int)dbg_pos.x,
      //     (int)dbg_pos.y), 100, 25, 20, BLUE);
      DrawFPS(0, 0);
    }

    EndDrawing();
  }

  unload_textures();
  UnloadSound(destroy_sound);
  UnloadSound(pickup_sound);

  free(world);
  CloseAudioDevice();
  CloseWindow();

  return 0;
}

/*
 * Create entities
 */
void init_entities() {
  int rock_health = 3;
  int tree_health = 3;
  int plant_health = 1;

  Entity* player = entity_create();
  player->arch = ARCH_PLAYER;
  player->position = v2(0, 0);
  player->sprite_id = SPRITE_player;
  player->size = get_sprite_size(SPRITE_player);
  player->health = 10;
  player->is_destroyable = false;

  Entity* kitchen = entity_create();
  kitchen->arch = ARCH_CRAFT_TABLE;
  kitchen->position = v2(10, 0);
  kitchen->sprite_id = SPRITE_table;
  kitchen->size = get_sprite_size(SPRITE_table);
  kitchen->is_destroyable = false;
  kitchen->health = 100;

  for (int i = 0; i < 10; i++) {
    int x = GetRandomValue(-100, 100);
    int y = GetRandomValue(-100, 100);
    Entity* rock = entity_create();
    rock->arch = ARCH_ROCK;
    rock->position = round_pos_to_tile(x, y, TILE_SIZE);
    rock->sprite_id = SPRITE_rock;
    rock->size = get_sprite_size(SPRITE_rock);
    rock->health = rock_health;
    rock->is_destroyable = true;
  }

  for (int i = 0; i < 5; i++) {
    int x = GetRandomValue(-100, 100);
    int y = GetRandomValue(-100, 100);
    Entity* tree = entity_create();
    tree->position = round_pos_to_tile(x, y, TILE_SIZE);
    tree->arch = ARCH_TREE;
    tree->sprite_id = SPRITE_tree;
    tree->size = get_sprite_size(SPRITE_tree);
    tree->health = tree_health;
    tree->is_destroyable = true;
  }

  for (int i = 0; i < 7; i++) {
    int x = GetRandomValue(-100, 100);
    int y = GetRandomValue(-100, 100);
    Entity* bush = entity_create();
    bush->position = round_pos_to_tile(x, y, TILE_SIZE);
    bush->arch = ARCH_TOMATO_PLANT;
    bush->sprite_id = SPRITE_tomato;
    bush->size = get_sprite_size(SPRITE_tomato);
    bush->health = plant_health;
    bush->is_destroyable = true;
  }

  for (int i = 0; i < 5; i++) {
    int x = GetRandomValue(-100, 100);
    int y = GetRandomValue(-100, 100);
    Entity* bush = entity_create();
    bush->position = round_pos_to_tile(x, y, TILE_SIZE);
    bush->arch = ARCH_PUMPKIN_PLANT;
    bush->sprite_id = get_sprite_id_from_arch(ARCH_PUMPKIN_PLANT);
    bush->size = get_sprite_size(SPRITE_tomato);
    bush->health = plant_health;
    bush->is_destroyable = true;
  }
  Entity* wood = entity_create();
  wood->position = round_pos_to_tile(20, 30, TILE_SIZE);
  wood->arch = ARCH_WOOD_ITEM;
  wood->sprite_id = SPRITE_wood;
  wood->size = get_sprite_size(SPRITE_wood);
  wood->is_destroyable = true;
  wood->is_item = true;

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
  found->is_valid = true;
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
