#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include "raylib.h"
#include "raymath.h"
#if defined(PLATFORM_WEB)
#include <emscripten/emscripten.h>
#endif

#define ARRAY_LEN(array) (sizeof(array) / sizeof(array[0]))

typedef enum ArchetypeID {
  ARCH_NIL,
  ARCH_PLAYER,
  ARCH_ROCK,
  ARCH_TREE,

  ARCH_TOMATO_PLANT,
  ARCH_PUMPKIN_PLANT,
  ARCH_CORN_PLANT,
  ARCH_PLANT,
  ARCH_SOIL,

  // items
  ARCH_ROCK_ITEM,
  ARCH_WOOD_ITEM,
  ARCH_PUMPKIN_ITEM,
  ARCH_TOMATO_ITEM,
  ARCH_CORN_ITEM,

  // crafting
  ARCH_CRAFT_TABLE,
  ARCH_STOCK_POT,
  ARCH_OVEN,
  ARCH_GRILL,

  // food
  ARCH_FOOD,
  ARCH_FOOD_nil,
  ARCH_FOOD_burger,
  ARCH_FOOD_bread_loaf,
  ARCH_FOOD_bread,
  ARCH_FOOD_baguette,
  ARCH_FOOD_hotdog,
  ARCH_FOOD_cocktail,
  ARCH_FOOD_beer,
  ARCH_FOOD_coffee,
  ARCH_FOOD_soda,
  ARCH_FOOD_potion_blue,
  ARCH_FOOD_potion_red,
  ARCH_FOOD_burrito,
  ARCH_FOOD_tea,
  ARCH_FOOD_lollipop,
  ARCH_FOOD_icecream,
  ARCH_FOOD_beef_leg,
  ARCH_FOOD_chicken_leg,
  ARCH_FOOD_chicken,
  ARCH_FOOD_egg,
  ARCH_FOOD_idk,
  ARCH_FOOD_soup_2,
  ARCH_FOOD_tomato_soup,
  ARCH_FOOD_idk2,
  ARCH_FOOD_caramel,
  ARCH_FOOD_sashimi1,
  ARCH_FOOD_sashimi2,
  ARCH_FOOD_cheesecake,
  ARCH_FOOD_sushi,
  ARCH_FOOD_donut,
  ARCH_FOOD_cake,
  ARCH_FOOD_sashimi3,
  ARCH_FOOD_sushi2,
  ARCH_FOOD_sushi_roll,
  ARCH_FOOD_acorn,
  ARCH_FOOD_sashimi4,
  ARCH_FOOD_juicebox,
  ARCH_FOOD_energy_drink1,
  ARCH_FOOD_energy_drink2,
  ARCH_FOOD_ham,
  ARCH_FOOD_wine_bottle,
  ARCH_FOOD_cola,
  ARCH_FOOD_milk,
  ARCH_FOOD_chocolate,
  ARCH_FOOD_pizza,
  ARCH_FOOD_pizza_whole,
  ARCH_FOOD_cheese,
  ARCH_FOOD_icecream2,
  ARCH_FOOD_icecream3,
  ARCH_FOOD_icecream4,
  ARCH_FOOD_MAX,

  ARCH_MAX,
} ArchetypeID;

typedef enum FoodID {
  FOOD_nil,
  FOOD_burger,
  FOOD_bread_loaf,
  FOOD_bread,
  FOOD_baguette,
  FOOD_hotdog,
  FOOD_cocktail,
  FOOD_beer,
  FOOD_coffee,
  FOOD_soda,
  FOOD_potion_blue,
  FOOD_potion_red,
  FOOD_burrito,
  FOOD_tea,
  FOOD_lollipop,
  FOOD_icecream,
  FOOD_beef_leg,
  FOOD_chicken_leg,
  FOOD_chicken,
  FOOD_egg,
  FOOD_idk,
  FOOD_soup_2,
  FOOD_tomato_soup,
  FOOD_idk2,
  FOOD_caramel,
  FOOD_sashimi1,
  FOOD_sashimi2,
  FOOD_cheesecake,
  FOOD_sushi,
  FOOD_donut,
  FOOD_cake,
  FOOD_sashimi3,
  FOOD_sushi2,
  FOOD_sushi_roll,
  FOOD_acorn,
  FOOD_sashimi4,
  FOOD_juicebox,
  FOOD_energy_drink1,
  FOOD_energy_drink2,
  FOOD_ham,
  FOOD_wine_bottle,
  FOOD_cola,
  FOOD_milk,
  FOOD_chocolate,
  FOOD_pizza,
  FOOD_pizza_whole,
  FOOD_cheese,
  FOOD_icecream2,
  FOOD_icecream3,
  FOOD_icecream4,
  FOOD_MAX,
} FoodID;

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
  SPRITE_shadow,

  SPRITE_FOOD_nil,
  SPRITE_FOOD_burger,
  SPRITE_FOOD_bread_loaf,
  SPRITE_FOOD_bread,
  SPRITE_FOOD_baguette,
  SPRITE_FOOD_hotdog,
  SPRITE_FOOD_cocktail,
  SPRITE_FOOD_beer,
  SPRITE_FOOD_coffee,
  SPRITE_FOOD_soda,
  SPRITE_FOOD_potion_blue,
  SPRITE_FOOD_potion_red,
  SPRITE_FOOD_burrito,
  SPRITE_FOOD_tea,
  SPRITE_FOOD_lollipop,
  SPRITE_FOOD_icecream,
  SPRITE_FOOD_beef_leg,
  SPRITE_FOOD_chicken_leg,
  SPRITE_FOOD_chicken,
  SPRITE_FOOD_egg,
  SPRITE_FOOD_idk,
  SPRITE_FOOD_soup_2,
  SPRITE_FOOD_tomato_soup,
  SPRITE_FOOD_idk2,
  SPRITE_FOOD_caramel,
  SPRITE_FOOD_sashimi1,
  SPRITE_FOOD_sashimi2,
  SPRITE_FOOD_cheesecake,
  SPRITE_FOOD_sushi,
  SPRITE_FOOD_donut,
  SPRITE_FOOD_cake,
  SPRITE_FOOD_sashimi3,
  SPRITE_FOOD_sushi2,
  SPRITE_FOOD_sushi_roll,
  SPRITE_FOOD_acorn,
  SPRITE_FOOD_sashimi4,
  SPRITE_FOOD_juicebox,
  SPRITE_FOOD_energy_drink1,
  SPRITE_FOOD_energy_drink2,
  SPRITE_FOOD_ham,
  SPRITE_FOOD_wine_bottle,
  SPRITE_FOOD_cola,
  SPRITE_FOOD_milk,
  SPRITE_FOOD_chocolate,
  SPRITE_FOOD_pizza,
  SPRITE_FOOD_pizza_whole,
  SPRITE_FOOD_cheese,
  SPRITE_FOOD_icecream2,
  SPRITE_FOOD_icecream3,
  SPRITE_FOOD_icecream4,
  SPRITE_FOOD_MAX,
  // NOTE: do not separate
  SPRITE_MAX
} SpriteID;

typedef struct Sprite {
  Texture2D texture;
  SpriteID id;
} Sprite;

typedef struct Entity {
  Vector2 position;
  Vector2 size;
  SpriteID sprite_id;
  ArchetypeID arch;
  FoodID food_id;
  float cooking_endtime;
  int health;
  bool is_valid;
  bool is_destroyable;
  bool is_item;
  bool is_food;
  bool render_sprite;
  bool is_cookware;
  bool currently_cooking;
  float distort;
} Entity;

typedef struct ItemData {
  int amount;
  ArchetypeID arch;
  FoodID food_id;
} ItemData;

static inline ItemData rock_item(int amount) {
  return (ItemData){.amount = amount, .arch = ARCH_ROCK_ITEM};
}

static inline ItemData wood_item(int amount) {
  return (ItemData){.amount = amount, .arch = ARCH_WOOD_ITEM};
}

static inline bool arch_is_food(ArchetypeID arch) {
  return (arch > ARCH_FOOD_nil && arch < ARCH_FOOD_MAX);
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
  ArchetypeID to_craft;
  // Time required to craft (seconds)
  float time_to_craft;
  ItemData requirements[MAX_REQUIREMENTS];
  int n_requirements;
} CraftingData;

// Maximum number of ingredients for one dish
#define MAX_INGREDIENTS 6
// Maximum number of dishes cooking at the same time
#define COOKING_MAX 10

typedef struct CookingData {
  bool is_valid;
  // points to an entity which is a cooking station
  Entity* cooking_on;
  float time_to_cook;
  // array of ingrediens in the cooking station at the time of cooking
  ItemData ingredients[MAX_INGREDIENTS];
  int n_ingredients;
  float cooking_end_time;
} CookingData;

typedef struct Recipe {
  FoodID result;
  ArchetypeID cooking_station;
  ArchetypeID ingredients[MAX_INGREDIENTS];
  int n_ingredients;
} Recipe;

typedef struct WorldResourceData {
  ArchetypeID arch;
  float spawn_interval;
  int max_count;
} WorldResourceData;

typedef enum UX_State {
  UX_nil,
  UX_inventory,
  UX_crafting,
  UX_cooking,
  UX_placing,
} UX_State;

struct World;
// :globals
struct World* world;
Sprite sprites[SPRITE_MAX];
CraftingData crafts[CRAFTING_MAX];
CookingData cooking_data[COOKING_MAX];
const WorldResourceData world_resources[] = {
    // {ARCH_TREE, 10.f, 10},
    {ARCH_ROCK, 5.f, 12},
    // {ARCH_TOMATO_PLANT, 5.f, 10},
    // {ARCH_PUMPKIN_PLANT, 10.f, 7},
    {ARCH_PLANT, 15.f, 12},
};
Recipe recipes[100];
Sound destroy_sound;
Sound pickup_sound;

int ScreenWidth = 800;
int ScreenHeight = 600;
Camera2D* camera;
Entity* player;

// :consts

const float TILE_SIZE = 8.0f;
const float ENTITY_SELECTION_RADIUS = 8.0;
const float PICKUP_RADIUS = 8.0;
#define MAX_ENTITIES 1024
typedef struct World {
  Entity entities[MAX_ENTITIES];
  ItemData inventory_items[ARCH_MAX];
  UX_State ux_state;
  CraftingID placing;
  ItemData holding;
  float spawn_data[ARRAY_LEN(world_resources)];
} World;

typedef struct EntitySort {
  int index;
  float y_pos;
} EntitySort;

int compare(const void* a, const void* b) {
  return (*(float*)a - *(float*)b);
}

typedef struct WorldFrame {
  EntitySort entities_ysort[MAX_ENTITIES];
  Entity* selected;
  Entity* near_player;
  Entity* near_pickup;
} WorldFrame;

static inline Vector2 v2(float x, float y) {
  return (Vector2){x, y};
}

static inline Rectangle expand_rectangle(Rectangle rec, float offset) {
  return (Rectangle){
      rec.x -= offset * 0.5,
      rec.y -= offset * 0.5,
      rec.width += offset,
      rec.height += offset,
  };
}

static inline Rectangle get_entity_rec(Entity* entity) {
  return (Rectangle){entity->position.x, entity->position.y, entity->size.x,
                     entity->size.y};
}

SpriteID sprite_id_from_food_id(FoodID id) {
  if (id == FOOD_nil)
    return SPRITE_nil;
  SpriteID sprite_id = (SpriteID)(id + SPRITE_FOOD_nil);

  return sprite_id;
}

ArchetypeID arch_from_food_id(FoodID id) {
  if (id == FOOD_nil) {
    return ARCH_FOOD_nil;
  }
  ArchetypeID arch = (ArchetypeID)(id + ARCH_FOOD_nil);

  return arch;
}

const char* get_arch_name(ArchetypeID arch) {
  switch (arch) {
    case ARCH_NIL:
      return "_";
    case ARCH_PLAYER:
      return "Player";
    case ARCH_ROCK:
      return "Rock";
    case ARCH_TREE:
      return "Tree";
    case ARCH_PLANT:
      return "Plant";
    case ARCH_TOMATO_PLANT:
      return "Tomato Plant";
    case ARCH_PUMPKIN_PLANT:
      return "Pumpkin plant";
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
    case ARCH_FOOD:
      return "Food";
    case ARCH_FOOD_nil:
      return "Dubious food";
    case ARCH_FOOD_egg:
      return "Eggs";
    case ARCH_FOOD_pizza_whole:
      return "Pizza";
    case ARCH_FOOD_tomato_soup:
      return "Tomato soup";
    default:
      return "missingname";
  };
}

static inline int get_dpi_scale() {
#if defined(PLATFORM_WEB)
  return 1;
#else
  return GetWindowScaleDPI().x;
#endif
}

Entity* entity_create();
Entity* create_food(Entity* cooking_station, Vector2 position);
Sprite load_sprite(const char* path, SpriteID id);
void init_entities();
void update_player(Entity* player);
void setup_window(void);
Camera2D* setup_camera();
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
  entity = 0;
}

ArchetypeID get_drop_from(Entity* destroyed) {
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

SpriteID get_sprite_id_from_arch(ArchetypeID arch) {
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
    case ARCH_STOCK_POT:
      return SPRITE_stock_pot;
    case ARCH_OVEN:
      return SPRITE_oven;
    case ARCH_GRILL:
      return SPRITE_grill;
    case ARCH_MAX:
      break;
    default:
      break;
  }

  if (arch > ARCH_FOOD_nil && arch < ARCH_FOOD_MAX) {
    int id = arch - ARCH_FOOD_nil;
    // assert(id > 0);
    SpriteID sprite_id = (SpriteID)(id + SPRITE_FOOD_nil);
    return sprite_id;
  }
  return SPRITE_nil;
}

Entity* create_item_drop(Entity* destroyed) {
  ArchetypeID arch_drop = get_drop_from(destroyed);
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

void setup_inventory() {
  for (int i = 0; i < ARCH_MAX; i++) {
    ItemData* idata = &world->inventory_items[i];
    ArchetypeID arch = (ArchetypeID)i;
    idata->amount = 0;
    idata->arch = arch;
  }
}

void setup_recipes() {
  // :recipes
  recipes[0] = (Recipe){
      .result = FOOD_egg,
      .cooking_station = ARCH_GRILL,
      .ingredients = {ARCH_ROCK_ITEM},
      .n_ingredients = 1,
  };
  recipes[1] = (Recipe){
      .result = FOOD_tomato_soup,
      .cooking_station = ARCH_STOCK_POT,
      .ingredients = {ARCH_TOMATO_ITEM},
      .n_ingredients = 1,
  };
  recipes[2] = (Recipe){
      .result = FOOD_pizza_whole,
      .cooking_station = ARCH_OVEN,
      .ingredients = {ARCH_TOMATO_ITEM, ARCH_WOOD_ITEM},
      .n_ingredients = 2,
  };
}

void load_sprites() {
  load_sprite("assets/missing.png", SPRITE_nil);
  load_sprite("assets/player.png", SPRITE_player);
  load_sprite("assets/tree.png", SPRITE_tree);
  load_sprite("assets/rock.png", SPRITE_rock);
  load_sprite("assets/wood.png", SPRITE_wood);
  load_sprite("assets/rock_item.png", SPRITE_rock_item);

  load_sprite("assets/bush.png", SPRITE_tomato);
  load_sprite("assets/pumpkin.png", SPRITE_pumpkin);
  load_sprite("assets/plant.png", SPRITE_plant);
  load_sprite("assets/soil.png", SPRITE_soil);
  load_sprite("assets/kitchen.png", SPRITE_table);
  load_sprite("assets/tomato_item.png", SPRITE_tomato_item);

  load_sprite("assets/pumpkin_item.png", SPRITE_pumpkin_item);

  load_sprite("assets/stock_pot.png", SPRITE_stock_pot);
  load_sprite("assets/grill.png", SPRITE_grill);
  load_sprite("assets/oven.png", SPRITE_oven);
  load_sprite("assets/shadow.png", SPRITE_shadow);

  for (int i = FOOD_nil; i < FOOD_MAX - 1; i++) {
    const char* filename = TextFormat("assets/fnb_sprite/%i.png", i);
    FoodID id = (FoodID)(i + 1);
    load_sprite(filename, sprite_id_from_food_id(id));
  }
}

Vector2 get_mouse_position() {
  Vector2 mouse_pos_screen = GetMousePosition();
  // Apply DPI scaling to the mouse position
  int dpi_scale = get_dpi_scale();
  mouse_pos_screen.x = mouse_pos_screen.x * dpi_scale;
  mouse_pos_screen.y = mouse_pos_screen.y * dpi_scale;
  return mouse_pos_screen;
}

World* init_world() {
  World* world = (World*)malloc(sizeof(World));
  *world = (World){0};
  world->ux_state = UX_nil;
  // for (int i = 0; i < ARRAY_LEN(world->spawn_data); i++) {
  //   world->spawn_data[i] = 0.0f;
  // }
  return world;
}

bool is_cooking_system(ArchetypeID arch) {
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
  for (int i = 0; i < craft.n_requirements; i++) {
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

// This is const data and shouldn't change during runtime
void setup_crafting_data() {
  crafts[CRAFTING_pot] =
      (CraftingData){.sprite_id = SPRITE_stock_pot,
                     .to_craft = ARCH_STOCK_POT,
                     .time_to_craft = 10.0,
                     .requirements = {rock_item(0), wood_item(0)},
                     .n_requirements = 2};

  crafts[CRAFTING_oven] = (CraftingData){.sprite_id = SPRITE_oven,
                                         .to_craft = ARCH_OVEN,
                                         .time_to_craft = 10.0,
                                         .requirements = {rock_item(4)},
                                         .n_requirements = 1};

  crafts[CRAFTING_grill] =
      (CraftingData){.sprite_id = SPRITE_grill,
                     .to_craft = ARCH_GRILL,
                     .time_to_craft = 6.0,
                     .requirements = {rock_item(1), wood_item(3)},
                     .n_requirements = 2};
}

CookingData* cooking_data_create() {
  CookingData* found = NULL;
  for (int i = 0; i < COOKING_MAX; i++) {
    CookingData* c = &cooking_data[i];
    if (!c->is_valid) {
      found = c;
      break;
    }
  }
  *found = (CookingData){0};
  found->is_valid = true;
  return found;
}

CookingData* get_cooking_data(Entity* cooking_station) {
  CookingData* found = NULL;
  for (int i = 0; i < COOKING_MAX; i++) {
    CookingData* c = &cooking_data[i];
    if (c->cooking_on == cooking_station) {
      found = c;
      // assert(c->is_valid);
      break;
    }
  }
  return found;
}
bool cooking_add_ingredients(Entity* cooking_station, ItemData ingredient) {
  CookingData* data = get_cooking_data(cooking_station);
  if (data == NULL) {
    // init cooking data
    data = cooking_data_create();
    data->cooking_on = cooking_station;
  }
  for (int i = 0; i < MAX_INGREDIENTS; i++) {
    if (data->ingredients[i].arch == ARCH_NIL) {
      ItemData* found = &data->ingredients[i];
      found->arch = ingredient.arch;
      found->amount = ingredient.amount;
      data->n_ingredients++;
      return true;
    }
  }
  // max ingredients reached
  return false;
}

void draw_shadow(Entity* entity, Rectangle rec, float offset) {
  Texture tex = sprites[SPRITE_shadow].texture;
  float v_offset = 2 * (entity->size.x / tex.width);
  Vector2 shadow_pos =
      Vector2Add(entity->position, v2(0, entity->size.y - v_offset));

  float size_offset = 0.05 * offset;

  DrawTexturePro(
      tex,
      (Rectangle){// source rectangle
                  .x = 0,
                  .y = 0,
                  .width = tex.width,
                  .height = tex.height},
      (Rectangle){// dest rectangle
                  .x = shadow_pos.x,
                  .y = shadow_pos.y,
                  .width = (1 + size_offset) * entity->size.x,
                  .height = (1 + size_offset) * 0.4 * entity->size.y},
      v2(0, 0), 0.0f, WHITE);
}

void update_draw_frame(void);

void hit_animation(Entity* entity) {
  entity->distort = 5.0f;
}

int main(void) {
  // Required so the window is not 1/4 of the screen in high dpi
  SetConfigFlags(FLAG_WINDOW_HIGHDPI);
  // :world setup
  world = init_world();

  setup_window();
  SetExitKey(0);

  InitAudioDevice();
  destroy_sound = LoadSound("assets/61_Hit_03.wav");
  pickup_sound = LoadSound("assets/001_Hover_01.wav");

  load_sprites();
  init_entities();

  player = &world->entities[0];

  // :inventory setup
  setup_inventory();
  // :crafting data setup
  setup_crafting_data();
  setup_recipes();

  camera = setup_camera();

#if defined(PLATFORM_WEB)
  emscripten_set_main_loop(update_draw_frame, 0, 1);
#else
  SetTargetFPS(60);

  while (!WindowShouldClose()) {
    update_draw_frame();
  }

#endif

  unload_textures();
  UnloadSound(destroy_sound);
  UnloadSound(pickup_sound);

  free(world);
  CloseAudioDevice();
  CloseWindow();

  return 0;
}

void update_draw_frame() {
  float dt = GetFrameTime();
  update_camera(camera, player, dt);

  BeginDrawing();
  ClearBackground((Color){106, 118, 63});

  update_player(player);
  WorldFrame world_frame = {0};

  Vector2 mouse_pos_screen = get_mouse_position();

  Vector2 mouse_pos_world = v2_screen_to_world(mouse_pos_screen, *camera);

  BeginMode2D(*camera);

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
          DrawRectangleRec(rec, (Color){47, 60, 2, 30});
        }
      }
    }
  }

  // :spawn resources
  {
    for (int i = 0; i < ARRAY_LEN(world_resources); i++) {
      WorldResourceData data = world_resources[i];

      // TODO: replace with count state to avoid counting each frame
      int entity_count = 0;
      for (int x = 0; x < MAX_ENTITIES; x++) {
        Entity* e = &world->entities[x];
        if (e->arch == data.arch) {
          entity_count += 1;
        }
      }

      if (entity_count >= data.max_count) {
        continue;
      }

      // setup spawn timer
      if (almost_equals(0.0, world->spawn_data[i], 0.01)) {
        if (entity_count < data.max_count) {
          world->spawn_data[i] = GetTime() + data.spawn_interval;
        }
      }
      if (GetTime() > world->spawn_data[i]) {
        // spawn the thing
        Entity* item_to_spawn = entity_create();
        int pos_x = GetRandomValue(-100, 100);
        int pos_y = GetRandomValue(-100, 100);
        SpriteID sprite_id = get_sprite_id_from_arch(data.arch);
        item_to_spawn->arch = data.arch;
        item_to_spawn->position = round_pos_to_tile(pos_x, pos_y, TILE_SIZE);
        item_to_spawn->sprite_id = sprite_id;
        item_to_spawn->health = 2;
        item_to_spawn->is_destroyable = true;
        item_to_spawn->size = get_sprite_size(sprite_id);
        world->spawn_data[i] = 0.0;
      }
    }
  }

  // :loop update all entities
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
    Rectangle pickup_rec = expand_rectangle(rec, PICKUP_RADIUS);

    // :selecting
    if (entity->arch != ARCH_PLAYER) {
      if (CheckCollisionPointRec(mouse_pos_world, selection_rec)) {
        if (!world_frame.selected)
          world_frame.selected = entity;
      }
      if (CheckCollisionPointRec(get_entity_center(player), selection_rec)) {
        world_frame.near_player = entity;
      }
      if (CheckCollisionPointRec(get_entity_center(player), pickup_rec)) {
        world_frame.near_pickup = entity;
      }
    }

    // :cooking time
    if (entity->is_cookware) {
      if (entity->currently_cooking) {
        // setup timer
        if (entity->cooking_endtime == 0) {
          entity->cooking_endtime =
              GetTime() + crafts[world->placing].time_to_craft;
        }
        // cook the thing
        if (GetTime() > entity->cooking_endtime) {
          Vector2 pos = round_pos_to_tile(
              entity->position.x, entity->position.y - TILE_SIZE, TILE_SIZE);

          create_food(entity, pos);

          entity->currently_cooking = false;
          entity->cooking_endtime = 0;
        }
      }
    }

    if (entity->distort > 0) {
      entity->distort -= 0.5;
    }
    if (entity->distort < 0) {
      entity->distort = 0;
    }
  }

  // :sort in y
  {
    for (int i = 0; i < MAX_ENTITIES; i++) {
      if (world->entities[i].is_valid) {
        world_frame.entities_ysort[i] =
            (EntitySort){.index = i, .y_pos = world->entities[i].position.y};
      }
    }

    qsort(world_frame.entities_ysort, MAX_ENTITIES, sizeof(EntitySort),
          compare);
  }

  // :rendering
  {
    for (int x = 0; x < MAX_ENTITIES; x++) {
      int i = world_frame.entities_ysort[x].index;
      Entity* entity = &world->entities[i];
      if (NULL == entity) {
        continue;
      }
      if (!entity->is_valid) {
        continue;
      }

      Rectangle rec = get_entity_rec(entity);
      if (entity->is_food) {
        rec = (Rectangle){rec.x, rec.y, rec.width / 2, rec.height / 2};
      }

      // :rendering
      switch (entity->arch) {
        case ARCH_PLAYER: {
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
            float scale = 1.0;
            if (entity->is_food) {
              scale = 0.5f;
            }

            draw_shadow(entity, rec, offset);
            DrawTextureEx(sprites[entity->sprite_id].texture, pos, 0, scale,
                          WHITE);
          } else {
            draw_shadow(entity, rec, 0.0f);
            DrawTextureEx(sprites[entity->sprite_id].texture, entity->position,
                          0.0, (1 + 0.01 * entity->distort), WHITE);
          }
          // draw cooking effects
          if (entity->is_cookware && entity->currently_cooking) {
            double time_left = entity->cooking_endtime - GetTime();
            DrawText(TextFormat("%i", (int)ceil(time_left)), entity->position.x,
                     entity->position.y, 2, WHITE);
          }

          break;
        }
      }
    }
    // player shadow

    Rectangle player_rec = get_entity_rec(player);
    draw_shadow(player, player_rec, 0.0f);
    // :render player
    DrawTextureV(sprites[player->sprite_id].texture, player->position, WHITE);
  }

  // :ux_state detection
  {
    // UX states like craft and cook depend on being close to certain entity
    if (world_frame.near_player) {
      ArchetypeID arch = world_frame.near_player->arch;
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
    else if (world->ux_state == UX_crafting || world->ux_state == UX_cooking) {
      world->ux_state = UX_nil;
      world->holding = (ItemData){0};
    }

    // handle open and close inventory
    if (IsKeyPressed(KEY_I) || IsKeyPressed(KEY_TAB)) {
      world->ux_state = UX_inventory;
    } else if (IsKeyPressed(KEY_ESCAPE)) {
      world->ux_state = UX_nil;
      world->holding = (ItemData){0};
    }
  }

  // :pickup items
  {
    if (world_frame.near_pickup) {
      Entity* entity_near = world_frame.near_pickup;
      if (entity_near->is_valid && entity_near->is_item) {
        // TODO: Pickup animation
        PlaySound(pickup_sound);

        world->inventory_items[entity_near->arch].amount += 1;
        if (entity_near->is_food) {
          world->inventory_items[entity_near->arch].food_id =
              entity_near->food_id;
        }
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
        // attack effect
        hit_animation(target);
        if (target->health <= 0) {
          create_item_drop(target);
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

      float scale = 5.0;
      if (arch_is_food(inventory_item.arch)) {
        scale = 2.5;
      }
      DrawTextureEx(
          sprites[get_sprite_id_from_arch(inventory_item.arch)].texture,
          texture_pos, 0, scale, WHITE);

      DrawText(TextFormat("[%i]", world->inventory_items[i].amount), text_pos.x,
               text_pos.y + 20, 20, WHITE);
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
      Vector2 texture_pos =
          v2(-50 + ScreenWidth / 2.0 + 50 * item_pos, ScreenHeight / 2.0 - 40);
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
      DrawText(TextFormat("[%i]", world->inventory_items[i].amount), text_pos.x,
               text_pos.y + 20, 20, WHITE);
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
        DrawText(get_arch_name(craft.to_craft), text_pos.x, text_pos.y - 24, 20,
                 WHITE);

        for (int j = 0; j < craft.n_requirements; j++) {
          Color text_color = RED;
          ItemData req = craft.requirements[j];
          int player_has = world->inventory_items[req.arch].amount;
          if (player_has >= req.amount) {
            text_color = WHITE;
          }
          Texture texture = sprites[get_sprite_id_from_arch(req.arch)].texture;
          DrawTexturePro(
              texture,
              (Rectangle){0, 0, (float)texture.width, (float)texture.height},
              (Rectangle){ui_origin_x + padding,
                          ui_origin_y + padding + j * icon_size, icon_size,
                          icon_size},
              v2(0, 0), 0, WHITE);
          DrawText(TextFormat("%i / %i", req.amount, player_has),
                   ui_origin_x + padding + icon_size,
                   ui_origin_y + padding + j * icon_size, 18, text_color);
        }
      }
      DrawTexturePro(
          texture,
          (Rectangle){0, 0, (float)texture.width, (float)texture.height},
          (Rectangle){texture_pos.x, texture_pos.y, icon_rec.width,
                      icon_rec.height},
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

      int world_factor = camera->zoom / get_dpi_scale();
      // the tile size on this case is on Screen space size
      float tile_size = TILE_SIZE * world_factor;
      Vector2 pos =
          round_pos_to_tile(mouse_pos_screen.x, mouse_pos_screen.y, tile_size);

      float width = texture.width * world_factor;
      float height = texture.height * world_factor;
      DrawTexturePro(
          texture,
          (Rectangle){0, 0, (float)texture.width, (float)texture.height},
          (Rectangle){pos.x, pos.y, width, height}, v2(0, 0), 0,
          (Color){255, 255, 255, 124});

      if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
        // place craft
        Entity* en = entity_create();
        en->arch = craft.to_craft;
        en->position =
            round_pos_to_tile(mouse_pos_world.x, mouse_pos_world.y, TILE_SIZE);
        en->sprite_id = craft.sprite_id;
        en->size = get_sprite_size(craft.sprite_id);
        en->is_destroyable = false;
        en->is_cookware = true;
        en->is_item = false;
        en->health = 100;
        // exit craft mode
        world->ux_state = UX_nil;

        // reduce the inventory
        for (int x = 0; x < craft.n_requirements; x++) {
          ItemData req = craft.requirements[x];
          world->inventory_items[req.arch].amount -= req.amount;
        }
      }
    }
  }
  // :ui cooking
  else if (world->ux_state == UX_cooking) {
    DrawRectangle(0, 0, ScreenWidth, ScreenHeight, (Color){50, 0, 0, 50});
    const char* text = "Cooking...";
    int fontsize = 20;
    int text_width = MeasureText(text, fontsize);
    DrawText(text, ScreenWidth / 2.0 - text_width / 2.0, 20, fontsize, WHITE);
    Entity* cooking_station = world_frame.near_player;

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
            world->holding =
                (ItemData){.amount = 1, .arch = inventory_item.arch};
          }
        }
      }
      DrawRectangleRec(rec, rec_color);

      float scale = 5.0;
      if (arch_is_food(inventory_item.arch)) {
        scale = 2.5;
      }
      DrawTextureEx(
          sprites[get_sprite_id_from_arch(inventory_item.arch)].texture,
          texture_pos, 0, scale, WHITE);

      DrawText(TextFormat("[%i]", world->inventory_items[i].amount), text_pos.x,
               text_pos.y + 20, 20, WHITE);
      item_pos++;
    }
    if (world->holding.arch != ARCH_NIL) {
      Vector2 sprite_pos = v2(mouse_pos_screen.x - 20, mouse_pos_screen.y - 20);
      ItemData holding = world->holding;
      Color item_text_color = WHITE;
      float scale = 5.0;
      if (CheckCollisionPointRec(mouse_pos_world,
                                 get_entity_rec(cooking_station))) {
        item_text_color = GREEN;
        float offset = fabs(sin(4 * GetTime()));
        scale += offset;
        if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
          // place in the kitchen
          cooking_station->currently_cooking = true;
          if (cooking_add_ingredients(cooking_station, world->holding)) {
            // reduce inventory
            world->inventory_items[world->holding.arch].amount -=
                world->holding.amount;
            // drop items
            world->holding = (ItemData){};
          }
        }
      }
      DrawTextureEx(sprites[get_sprite_id_from_arch(holding.arch)].texture,
                    sprite_pos, 0, scale, WHITE);
      DrawText(TextFormat("%i", holding.amount), sprite_pos.x, sprite_pos.y, 20,
               item_text_color);
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
  return;
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

Camera2D* setup_camera() {
  Camera2D* camera2d = (Camera2D*)malloc(sizeof(Camera2D));
  int width = GetRenderWidth();
  int height = GetRenderHeight();
  int dpi_scale = get_dpi_scale();
  camera2d->offset = v2(width / 2.0f, height / 2.0f);
  camera2d->rotation = 0.0f;
  camera2d->target = world->entities[0].position;
  camera2d->zoom = 5 * dpi_scale;
  return camera2d;
}

/// Move player
void update_player(Entity* player) {
  Vector2 input_axis = v2(0.0, 0.0);
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
  // assert(found);  // "No free entities"
  *found = (Entity){0};
  found->is_valid = true;
  return found;
}

bool ingredients_match(CookingData* data, Recipe* recipe) {
  if (data->n_ingredients != recipe->n_ingredients) {
    return false;  // Ingredient count must match
  }

  // Create a boolean array to track matched ingredients
  bool matched[MAX_INGREDIENTS] = {false};

  // Check each ingredient in the recipe
  for (int i = 0; i < recipe->n_ingredients; i++) {
    bool found = false;

    // Try to find a match for recipe ingredient[i] in data->ingredients
    for (int j = 0; j < data->n_ingredients; j++) {
      if (!matched[j] && recipe->ingredients[i] == data->ingredients[j].arch) {
        matched[j] = true;  // Mark ingredient as matched
        found = true;
        break;
      }
    }

    if (!found) {
      return false;  // If any ingredient is not found, the recipe doesn't match
    }
  }

  return true;  // All ingredients matched
}

/*
 * Must check the ingredients and define what food is the result
 * of the ingredients and the cooking station used
 */
FoodID resolve_ingredients(CookingData* data,
                           Recipe* recipes,
                           int recipe_count) {
  if (!data->is_valid) {
    return FOOD_nil;  // Invalid or not currently cooking
  }

  // Loop through all the recipes
  for (int i = 0; i < recipe_count; i++) {
    Recipe* recipe = &recipes[i];

    // Check if the cooking station matches
    if (recipe->cooking_station != data->cooking_on->arch) {
      continue;
    }

    // Check if the ingredients match (in any order)
    if (ingredients_match(data, recipe)) {
      return recipe->result;  // Found a matching recipe
    }
  }

  return FOOD_nil;  // No matching recipe found
}

Entity* create_food(Entity* cooking_station, Vector2 position) {
  Entity* cooked = entity_create();
  CookingData* cooking_data = get_cooking_data(cooking_station);
  int n_recipes = (sizeof recipes / sizeof recipes[0]);
  FoodID food_id = resolve_ingredients(cooking_data, recipes, n_recipes);
  ArchetypeID arch = arch_from_food_id(food_id);
  SpriteID sprite_id = get_sprite_id_from_arch(arch);
  cooked->arch = arch;
  cooked->sprite_id = sprite_id;
  cooked->size = get_sprite_size(sprite_id);
  cooked->position = position;
  cooked->food_id = food_id;
  cooked->is_item = true;
  cooked->is_food = true;

  // clean work station data
  *cooking_data = (CookingData){0};

  return cooked;
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
  int dpi = get_dpi_scale();

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
    sprites[id] = (Sprite){.texture = texture, .id = id};
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
