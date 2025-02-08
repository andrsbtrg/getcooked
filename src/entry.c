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
  ARCH_BRICK,

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
  SPRITE_player_up,
  SPRITE_player_left,
  SPRITE_player_right,
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
  SPRITE_brick,

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
  int frames;
  Rectangle frameRec;
} sprite_t;

typedef struct Entity {
  Vector2 position;
  Vector2 size;
  SpriteID sprite_id;
  ArchetypeID arch;
  FoodID food_id;
  float cooking_endtime;
  int health;
  int max_health;
  bool is_valid;
  bool is_destroyable;
  bool is_item;
  bool is_food;
  bool is_pickup;
  bool render_sprite;
  bool is_cookware;
  bool currently_cooking;
  float distort;
  int currentFrame;
  int frameCounter;
} entity_t;

typedef struct ItemData {
  int amount;
  ArchetypeID arch;
  FoodID food_id;
} item_data_t;

static inline item_data_t rock_item(int amount) {
  return (item_data_t){.amount = amount, .arch = ARCH_ROCK_ITEM};
}

static inline item_data_t wood_item(int amount) {
  return (item_data_t){.amount = amount, .arch = ARCH_WOOD_ITEM};
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
  CRAFTING_brick,
  CRAFTING_MAX,
} CraftingID;

#define MAX_REQUIREMENTS 6
typedef struct CraftingData {
  SpriteID sprite_id;
  // This is the entity Archetype that this Crafting action will create
  ArchetypeID to_craft;
  // The station that is able to craft this item
  ArchetypeID station;
  // Time required to craft (seconds)
  float time_to_craft;
  item_data_t requirements[MAX_REQUIREMENTS];
  int n_requirements;
} crafting_data_t;

// Maximum number of ingredients for one dish
#define MAX_INGREDIENTS 6
// Maximum number of dishes cooking at the same time
#define COOKING_MAX 10

typedef struct CookingData {
  bool is_valid;
  // points to an entity which is a cooking station
  entity_t* cooking_on;
  float time_to_cook;
  // array of ingrediens in the cooking station at the time of cooking
  item_data_t ingredients[MAX_INGREDIENTS];
  int n_ingredients;
  float cooking_end_time;
} cooking_data_t;

typedef struct Recipe {
  FoodID result;
  ArchetypeID cooking_station;
  ArchetypeID ingredients[MAX_INGREDIENTS];
  int n_ingredients;
} recipe_t;

typedef struct WorldResourceData {
  ArchetypeID arch;
  float spawn_interval;
  int max_count;
} world_resource_t;

typedef enum UX_State {
  UX_nil,
  UX_inventory,
  UX_crafting,
  UX_cooking,
  UX_placing,
} UX_State;

// :globals
struct World* world;
sprite_t sprites[SPRITE_MAX];
crafting_data_t crafts[CRAFTING_MAX];
cooking_data_t cooking_data[COOKING_MAX];
const world_resource_t world_resources[] = {
    // {ARCH_TREE, 10.f, 10},
    {ARCH_ROCK, 5.f, 15},
    // {ARCH_TOMATO_PLANT, 5.f, 10},
    // {ARCH_PUMPKIN_PLANT, 10.f, 7},
    {ARCH_PLANT, 15.f, 12},
};
recipe_t recipes[100];
Sound destroy_sound;
Sound pickup_sound;

int ScreenWidth = 800;
int ScreenHeight = 600;
Camera2D* camera;
entity_t* player;

// :consts

const float TILE_SIZE = 8.0f;
const float ENTITY_SELECTION_RADIUS = 10.0;
const float PICKUP_RADIUS = 20.0;
const int FRAME_SPEED = 8;

#define MAX_ENTITIES 1024

typedef struct PlayerHealth {
  int max_health;
  int current_health;
  int drop_rate;
} player_health_t;

typedef struct World {
  entity_t entities[MAX_ENTITIES];
  item_data_t inventory_items[ARCH_MAX];
  UX_State ux_state;
  CraftingID placing;
  item_data_t holding;
  float spawn_data[ARRAY_LEN(world_resources)];
  player_health_t health;
} world_t;

typedef struct EntitySort {
  int index;
  float y_pos;
  bool is_valid;
} entity_sort_t;

int compare(const void* a, const void* b) {
  float float_a = ((entity_sort_t*)a)->y_pos;
  float float_b = ((entity_sort_t*)b)->y_pos;
  return (float_a - float_b);
}

typedef struct WorldFrame {
  entity_sort_t entities_ysort[MAX_ENTITIES];
  entity_t* selected;
  entity_t* near_player;
  entity_t* near_pickup;
} world_frame_t;

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

static inline Rectangle get_entity_rec(entity_t* entity) {
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
    case ARCH_BRICK:
      return "Brick";
    default:
      return "missingname";
  };
}

static inline int get_dpi_scale(void) {
#if defined(PLATFORM_WEB)
  return 1;
#else
  return GetWindowScaleDPI().x;
#endif
}

entity_t* entity_create(void);
entity_t* create_food(entity_t* cooking_station, Vector2 position);
sprite_t load_sprite(const char* path, SpriteID id, int frames);
void init_entities(void);
void update_player(entity_t* player);
void setup_window(void);
Camera2D* setup_camera(void);
void update_camera(Camera2D*, entity_t*, float dt);
void unload_textures(void);
void draw_sprite(entity_t* entity, Vector2 offset);
void update_entity_frame(entity_t* entity);
int get_max_health(ArchetypeID arch);

// :colors
#define WHITE_TRANSP CLITERAL(Color){255, 255, 255, 50}  // White transparent
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
  sprite_t sprite = sprites[id];
  return v2(sprite.texture.width / (float)sprite.frames, sprite.texture.height);
}

Vector2 get_entity_center(entity_t* entity) {
  return (Vector2){
      entity->position.x + entity->size.x * 0.5f,
      entity->position.y + entity->size.x * 0.5f,
  };
}

void entity_destroy(entity_t* entity) {
  entity->is_valid = false;
  entity = 0;
}

ArchetypeID get_drop_from(entity_t* destroyed) {
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
      return destroyed->arch;
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
    case ARCH_BRICK:
      return SPRITE_brick;
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

entity_t* create_item_drop(entity_t* destroyed) {
  ArchetypeID arch_drop = get_drop_from(destroyed);
  SpriteID sprite_id = get_sprite_id_from_arch(arch_drop);
  Vector2 size = get_sprite_size(sprite_id);

  entity_t* item = entity_create();
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

void setup_inventory(void) {
  for (int i = 0; i < ARCH_MAX; i++) {
    item_data_t* idata = &world->inventory_items[i];
    ArchetypeID arch = (ArchetypeID)i;
    idata->amount = 0;
    idata->arch = arch;
  }
}

void setup_recipes(void) {
  // :recipes
  recipes[0] = (recipe_t){
      .result = FOOD_egg,
      .cooking_station = ARCH_GRILL,
      .ingredients = {ARCH_ROCK_ITEM},
      .n_ingredients = 1,
  };
  recipes[1] = (recipe_t){
      .result = FOOD_tomato_soup,
      .cooking_station = ARCH_STOCK_POT,
      .ingredients = {ARCH_TOMATO_ITEM},
      .n_ingredients = 1,
  };
  recipes[2] = (recipe_t){
      .result = FOOD_pizza_whole,
      .cooking_station = ARCH_OVEN,
      .ingredients = {ARCH_TOMATO_ITEM, ARCH_WOOD_ITEM},
      .n_ingredients = 2,
  };
}

void load_sprites(void) {
  load_sprite("assets/missing.png", SPRITE_nil, 1);

  load_sprite("assets/player.png", SPRITE_player, 3);
  load_sprite("assets/player_up.png", SPRITE_player_up, 3);
  load_sprite("assets/player_right.png", SPRITE_player_right, 3);
  load_sprite("assets/player_left.png", SPRITE_player_left, 3);

  load_sprite("assets/tree.png", SPRITE_tree, 1);
  load_sprite("assets/rock.png", SPRITE_rock, 1);
  load_sprite("assets/wood.png", SPRITE_wood, 1);
  load_sprite("assets/rock_item.png", SPRITE_rock_item, 1);

  load_sprite("assets/bush.png", SPRITE_tomato, 1);
  load_sprite("assets/pumpkin.png", SPRITE_pumpkin, 1);
  load_sprite("assets/plant.png", SPRITE_plant, 1);
  load_sprite("assets/soil.png", SPRITE_soil, 1);
  load_sprite("assets/kitchen.png", SPRITE_table, 1);
  load_sprite("assets/tomato_item.png", SPRITE_tomato_item, 1);

  load_sprite("assets/pumpkin_item.png", SPRITE_pumpkin_item, 1);

  load_sprite("assets/stock_pot.png", SPRITE_stock_pot, 1);
  load_sprite("assets/grill.png", SPRITE_grill, 1);
  load_sprite("assets/oven.png", SPRITE_oven, 1);
  load_sprite("assets/shadow.png", SPRITE_shadow, 1);
  load_sprite("assets/brick.png", SPRITE_brick, 1);

  for (int i = FOOD_nil; i < FOOD_MAX - 1; i++) {
    const char* filename = TextFormat("assets/fnb_sprite/%i.png", i);
    FoodID id = (FoodID)(i + 1);
    load_sprite(filename, sprite_id_from_food_id(id), 1);
  }
}

Vector2 get_mouse_position(void) {
  Vector2 mouse_pos_screen = GetMousePosition();
  // Apply DPI scaling to the mouse position
  int dpi_scale = get_dpi_scale();
  mouse_pos_screen.x = mouse_pos_screen.x * dpi_scale;
  mouse_pos_screen.y = mouse_pos_screen.y * dpi_scale;
  return mouse_pos_screen;
}

world_t* init_world(void) {
  world_t* world = (world_t*)malloc(sizeof(world_t));
  *world = (world_t){0};
  world->ux_state = UX_nil;
  world->health = (player_health_t){
      .max_health = 100, .current_health = 100, .drop_rate = 0};
  // for (int i = 0; i < ARRAY_LEN(world->spawn_data); i++) {
  //   world->spawn_data[i] = 0.0f;
  // }
  return world;
}

float square(float n) {
  return n * n;
}
float v2_distance_sq(Vector2 a, Vector2 b) {
  return (float)(square(a.x - b.x)) + square(a.y - b.y);
}

bool is_cooking_system(ArchetypeID arch) {
  switch (arch) {
    // case ARCH_OVEN:
    case ARCH_STOCK_POT:
    case ARCH_GRILL:
      return true;
    default:
      return false;
  }
}

bool check_craft_requirements(crafting_data_t craft,
                              const item_data_t inventory_items[ARCH_MAX]) {
  for (int i = 0; i < craft.n_requirements; i++) {
    item_data_t req = craft.requirements[i];
    int have = inventory_items[req.arch].amount;
    if (req.amount > have) {
      return false;
    }
  }
  return true;
}

bool action_button_pressed(void) {
  return IsMouseButtonPressed(MOUSE_BUTTON_LEFT) || IsKeyPressed(KEY_SPACE);
}

// This is const data and shouldn't change during runtime
void setup_crafting_data(void) {
  crafts[CRAFTING_pot] =
      (crafting_data_t){.sprite_id = SPRITE_stock_pot,
                        .to_craft = ARCH_STOCK_POT,
                        .station = ARCH_CRAFT_TABLE,
                        .time_to_craft = 10.0,
                        .requirements = {rock_item(0), wood_item(0)},
                        .n_requirements = 2};

  crafts[CRAFTING_oven] = (crafting_data_t){.sprite_id = SPRITE_oven,
                                            .to_craft = ARCH_OVEN,
                                            .station = ARCH_CRAFT_TABLE,
                                            .time_to_craft = 10.0,
                                            .requirements = {rock_item(4)},
                                            .n_requirements = 1};

  crafts[CRAFTING_grill] =
      (crafting_data_t){.sprite_id = SPRITE_grill,
                        .station = ARCH_CRAFT_TABLE,
                        .to_craft = ARCH_GRILL,
                        .time_to_craft = 6.0,
                        .requirements = {rock_item(1), wood_item(3)},
                        .n_requirements = 2};

  crafts[CRAFTING_brick] = (crafting_data_t){
      .station = ARCH_OVEN,
      .sprite_id = SPRITE_brick,
      .to_craft = ARCH_BRICK,
      .time_to_craft = 6.0,
      .requirements = {rock_item(1)},
      .n_requirements = 1,
  };
}

cooking_data_t* cooking_data_create(void) {
  cooking_data_t* found = NULL;
  for (int i = 0; i < COOKING_MAX; i++) {
    cooking_data_t* c = &cooking_data[i];
    if (!c->is_valid) {
      found = c;
      break;
    }
  }
  *found = (cooking_data_t){0};
  found->is_valid = true;
  return found;
}

cooking_data_t* get_cooking_data(entity_t* cooking_station) {
  cooking_data_t* found = NULL;
  for (int i = 0; i < COOKING_MAX; i++) {
    cooking_data_t* c = &cooking_data[i];
    if (c->cooking_on == cooking_station) {
      found = c;
      // assert(c->is_valid);
      break;
    }
  }
  return found;
}
bool cooking_add_ingredients(entity_t* cooking_station,
                             item_data_t ingredient) {
  cooking_data_t* data = get_cooking_data(cooking_station);
  if (data == NULL) {
    // init cooking data
    data = cooking_data_create();
    data->cooking_on = cooking_station;
  }
  for (int i = 0; i < MAX_INGREDIENTS; i++) {
    if (data->ingredients[i].arch == ARCH_NIL) {
      item_data_t* found = &data->ingredients[i];
      found->arch = ingredient.arch;
      found->amount = ingredient.amount;
      data->n_ingredients++;
      return true;
    }
  }
  // max ingredients reached
  return false;
}

void draw_shadow(entity_t* entity, float offset) {
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

void hit_animation(entity_t* entity) {
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

  SetTargetFPS(60);
#if defined(PLATFORM_WEB)
  emscripten_set_main_loop(update_draw_frame, 0, 1);
#else

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

void update_draw_frame(void) {
  float dt = GetFrameTime();
  update_camera(camera, player, dt);

  BeginDrawing();
  ClearBackground((Color){106, 118, 63, 0});

  update_player(player);
  world_frame_t world_frame = {0};

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
    for (int i = 0; i < (int)ARRAY_LEN(world_resources); i++) {
      world_resource_t data = world_resources[i];

      // TODO: replace with count state to avoid counting each frame
      int entity_count = 0;
      for (int x = 0; x < MAX_ENTITIES; x++) {
        entity_t* e = &world->entities[x];
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
        int health = get_max_health(data.arch);
        entity_t* item_to_spawn = entity_create();
        int pos_x = GetRandomValue(-100, 100);
        int pos_y = GetRandomValue(-100, 100);
        SpriteID sprite_id = get_sprite_id_from_arch(data.arch);
        item_to_spawn->arch = data.arch;
        item_to_spawn->position = round_pos_to_tile(pos_x, pos_y, TILE_SIZE);
        item_to_spawn->sprite_id = sprite_id;
        item_to_spawn->health = health;
        item_to_spawn->max_health = health;
        item_to_spawn->is_destroyable = true;
        item_to_spawn->size = get_sprite_size(sprite_id);
        world->spawn_data[i] = 0.0;
      }
    }
  }

  // :loop update all entities
  for (int i = 0; i < MAX_ENTITIES; i++) {
    entity_t* entity = &world->entities[i];
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
        if (entity->is_item) {
          animate_v2_to_target(&entity->position, player->position, dt, 20.0f);
        }
      }
      if (CheckCollisionPointRec(get_entity_center(player), pickup_rec) &&
          entity->is_item) {
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

    // Hit distortion
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
        entity_t this = world->entities[i];
        world_frame.entities_ysort[i] =
            (entity_sort_t){.index = i,
                            .y_pos = (this.position.y + this.size.y),
                            .is_valid = true};
      }
    }

    qsort(world_frame.entities_ysort, MAX_ENTITIES, sizeof(entity_sort_t),
          compare);
  }

  // :render entities
  {
    for (int x = 0; x < MAX_ENTITIES; x++) {
      if (!world_frame.entities_ysort[x].is_valid)
        continue;
      int i = world_frame.entities_ysort[x].index;
      entity_t* entity = &world->entities[i];
      if (NULL == entity || !entity->is_valid) {
        continue;
      }

      Rectangle rec = get_entity_rec(entity);
      if (entity->is_food) {
        rec = (Rectangle){rec.x, rec.y, rec.width / 2, rec.height / 2};
      }

      // :rendering
      switch (entity->arch) {
        default: {
          Color tile_color = {0};
          if (entity == world_frame.selected ||
              entity == world_frame.near_player) {
            tile_color = WHITE_TRANSP;
          }

          DrawRectangleRec(rec, tile_color);

          Vector2 sprite_offset = v2(0, 0);

          if (entity->is_item) {  // floating shadow animation
            float offset = sin(4 * GetTime());
            sprite_offset.y = offset;
          }

          draw_shadow(entity, sprite_offset.y);

          draw_sprite(entity, sprite_offset);
          // draw cooking effects
          if (entity->is_cookware && entity->currently_cooking) {
            double time_left = entity->cooking_endtime - GetTime();
            DrawRectangle(entity->position.x, entity->position.y,
                          (int)ceil(time_left), 2, ORANGE);
          }
          if (entity->health < entity->max_health) {
            DrawRectangle(entity->position.x, entity->position.y,
                          (int)(entity->health), 2, GREEN);
          }
          break;
        }
      }
    }
  }

  // :ux_state detection
  {
    // UX states like craft and cook depend on being close to certain entity
    if (world_frame.near_player) {
      ArchetypeID arch = world_frame.near_player->arch;
      if (world->ux_state != UX_placing) {
        if (arch == ARCH_CRAFT_TABLE || arch == ARCH_OVEN) {
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
      world->holding = (item_data_t){0};
    }

    // handle open and close inventory
    if (IsKeyPressed(KEY_I) || IsKeyPressed(KEY_TAB)) {
      world->ux_state = UX_inventory;
    } else if (IsKeyPressed(KEY_ESCAPE)) {
      world->ux_state = UX_nil;
      world->holding = (item_data_t){0};
    }
  }

  // :pickup items
  {
    if (world_frame.near_pickup) {
      entity_t* entity_near = world_frame.near_pickup;
      if (entity_near->is_valid && entity_near->is_item) {
        // Pickup animation
        float dist_sq = v2_distance_sq(entity_near->position, player->position);
        if (fabs(dist_sq - 0.1) <= 0.09) {
          world->inventory_items[entity_near->arch].amount += 1;
          if (entity_near->is_food) {
            world->inventory_items[entity_near->arch].food_id =
                entity_near->food_id;
          }
          PlaySound(pickup_sound);
          entity_destroy(entity_near);
        }
      }
    }
  }

  // :clicking
  {
    if (world->ux_state == UX_nil && action_button_pressed()) {
      entity_t* target = NULL;
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

  // :ui status bar
  {
    DrawRectangle(1, 1, world->health.current_health, 5, GREEN);
  }

  // :ui quick access
  if (world->ux_state == UX_nil) {
    int item_pos = 0;
    for (int i = 0; i < ARCH_MAX; i++) {
      item_data_t inventory_item = world->inventory_items[i];
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
      item_data_t inventory_item = world->inventory_items[i];
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
        DrawText(get_arch_name(world->inventory_items[i].arch), text_pos.x,
                 text_pos.y - 20, 18, WHITE);
        if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
          // place item
          entity_t* en = entity_create();
          ArchetypeID arch = (ArchetypeID)i;
          en->arch = arch;
          en->position = round_pos_to_tile(mouse_pos_world.x, mouse_pos_world.y,
                                           TILE_SIZE);
          en->sprite_id = get_sprite_id_from_arch(arch);
          en->size = get_sprite_size(en->sprite_id);
          en->is_destroyable = true;
          en->is_item = true;
          en->health = 1;
          en->is_pickup = true;
          // exit craft mode
          world->ux_state = UX_nil;
          // reduce the inventory
          world->inventory_items[arch].amount -= 1;
        }
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

    // draw crafting options
    // x keeps count of the crafts that are available to craete are current
    // station
    int x = 0;
    for (int i = 1; i < CRAFTING_MAX; i++) {
      CraftingID craft_id = (CraftingID)i;
      crafting_data_t craft = crafts[craft_id];
      // display only crafts that can be made on cooking station near player
      if (craft.station != world_frame.near_player->arch) {
        continue;
      }
      x += 1;
      Vector2 texture_pos = v2(ui_origin_x + 50 * x, ScreenHeight * 0.5);
      Vector2 text_pos = v2(ui_container.x, ui_container.y);

      Rectangle icon_rec =
          (Rectangle){texture_pos.x, texture_pos.y, icon_size, icon_size};

      Color inner_rec_color = rec_color;

      Texture2D texture = sprites[craft.sprite_id].texture;
      // :placing craft
      if (CheckCollisionPointRec(mouse_pos_screen, icon_rec) &&
          IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
        if (check_craft_requirements(craft, world->inventory_items)) {
          world->ux_state = UX_placing;
          world->placing = craft_id;
        }
      }

      // mouse hovering on craft
      if (CheckCollisionPointRec(mouse_pos_screen, icon_rec)) {
        // container rec
        DrawRectangleRec(ui_container, rec_color);
        float offset = 4 + sin(4 * GetTime());
        texture_pos = v2(texture_pos.x, texture_pos.y - offset);
        inner_rec_color = GOLD;
        inner_rec_color.a = 50;
        DrawText(get_arch_name(craft.to_craft), text_pos.x, text_pos.y - 24, 20,
                 WHITE);

        for (int j = 0; j < craft.n_requirements; j++) {
          Color text_color = RED;
          item_data_t req = craft.requirements[j];
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

      DrawRectangleRec(icon_rec, inner_rec_color);  // rectangle behind icon
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
      crafting_data_t craft = crafts[world->placing];
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
        entity_t* en = entity_create();
        en->arch = craft.to_craft;
        en->position =
            round_pos_to_tile(mouse_pos_world.x, mouse_pos_world.y, TILE_SIZE);
        en->sprite_id = craft.sprite_id;
        en->size = get_sprite_size(craft.sprite_id);
        en->is_destroyable = true;
        en->is_cookware = true;
        en->is_item = false;
        en->health = 1;
        en->is_pickup = true;
        // exit craft mode
        world->ux_state = UX_nil;

        // reduce the inventory
        for (int x = 0; x < craft.n_requirements; x++) {
          item_data_t req = craft.requirements[x];
          world->inventory_items[req.arch].amount -= req.amount;
        }
      }
    }
  }
  // :ui cooking
  else if (world->ux_state == UX_cooking) {
    DrawRectangle(0, 0, ScreenWidth, ScreenHeight, (Color){50, 50, 50, 50});
    const char* text = "Cooking...";
    int fontsize = 20;
    int text_width = MeasureText(text, fontsize);
    DrawText(text, ScreenWidth / 2.0 - text_width / 2.0, 20, fontsize, WHITE);
    entity_t* cooking_station = world_frame.near_player;

    int item_pos = 0;
    for (int i = 0; i < ARCH_MAX; i++) {
      item_data_t inventory_item = world->inventory_items[i];
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
          item_data_t holdprev = world->holding;
          if (holdprev.arch == inventory_item.arch) {
            if (world->holding.amount < inventory_item.amount) {
              world->holding.amount += 1;
            }
          } else {
            world->holding =
                (item_data_t){.amount = 1, .arch = inventory_item.arch};
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
      item_data_t holding = world->holding;
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
            world->holding = (item_data_t){0};
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
void init_entities(void) {
  int rock_health = get_max_health(ARCH_ROCK);
  int tree_health = get_max_health(ARCH_TREE);

  entity_t* player = entity_create();
  player->arch = ARCH_PLAYER;
  player->position = v2(0, 0);
  player->sprite_id = SPRITE_player;
  player->size = get_sprite_size(SPRITE_player);
  player->health = 10;
  player->is_destroyable = false;

  entity_t* rock = entity_create();
  rock->arch = ARCH_ROCK;
  rock->position = round_pos_to_tile(10, -5, TILE_SIZE);
  rock->sprite_id = SPRITE_rock;
  rock->size = get_sprite_size(SPRITE_rock);
  rock->health = rock_health;
  rock->max_health = rock_health;
  rock->is_destroyable = true;

  entity_t* kitchen = entity_create();
  kitchen->arch = ARCH_CRAFT_TABLE;
  kitchen->position = v2(10, 0);
  kitchen->sprite_id = SPRITE_table;
  kitchen->size = get_sprite_size(SPRITE_table);
  kitchen->is_destroyable = false;
  kitchen->health = 100;

  for (int i = 0; i < 10; i++) {
    int x = GetRandomValue(-100, 100);
    int y = GetRandomValue(-100, 100);
    entity_t* rock = entity_create();
    rock->arch = ARCH_ROCK;
    rock->position = round_pos_to_tile(x, y, TILE_SIZE);
    rock->sprite_id = SPRITE_rock;
    rock->size = get_sprite_size(SPRITE_rock);
    rock->health = rock_health;
    rock->max_health = rock_health;
    rock->is_destroyable = true;
  }

  for (int i = 0; i < 10; i++) {
    int x = GetRandomValue(-100, 100);
    int y = GetRandomValue(-100, 100);
    entity_t* tree = entity_create();
    tree->position = round_pos_to_tile(x, y, TILE_SIZE);
    tree->arch = ARCH_TREE;
    tree->sprite_id = SPRITE_tree;
    tree->size = get_sprite_size(SPRITE_tree);
    tree->health = tree_health;
    tree->max_health = tree_health;
    tree->is_destroyable = true;
  }

  for (int i = 0; i < 7; i++) {
    int x = GetRandomValue(-100, 100);
    int y = GetRandomValue(-100, 100);
    entity_t* tomato_plant = entity_create();
    tomato_plant->position = round_pos_to_tile(x, y, TILE_SIZE);
    tomato_plant->arch = ARCH_TOMATO_PLANT;
    tomato_plant->sprite_id = SPRITE_tomato;
    tomato_plant->size = get_sprite_size(SPRITE_tomato);
    tomato_plant->health = get_max_health(ARCH_TOMATO_PLANT);
    tomato_plant->max_health = get_max_health(ARCH_TOMATO_PLANT);
    tomato_plant->is_destroyable = true;
  }

  for (int i = 0; i < 5; i++) {
    int x = GetRandomValue(-100, 100);
    int y = GetRandomValue(-100, 100);
    entity_t* pumpkin_plant = entity_create();
    pumpkin_plant->position = round_pos_to_tile(x, y, TILE_SIZE);
    pumpkin_plant->arch = ARCH_PUMPKIN_PLANT;
    pumpkin_plant->sprite_id = get_sprite_id_from_arch(ARCH_PUMPKIN_PLANT);
    pumpkin_plant->size = get_sprite_size(SPRITE_pumpkin);
    pumpkin_plant->health = get_max_health(ARCH_PUMPKIN_PLANT);
    pumpkin_plant->max_health = get_max_health(ARCH_PUMPKIN_PLANT);
    pumpkin_plant->is_destroyable = true;
  }
  entity_t* wood = entity_create();
  wood->position = round_pos_to_tile(20, 30, TILE_SIZE);
  wood->arch = ARCH_WOOD_ITEM;
  wood->sprite_id = SPRITE_wood;
  wood->size = get_sprite_size(SPRITE_wood);
  wood->is_destroyable = false;
  wood->is_item = true;

  return;
}

Camera2D* setup_camera(void) {
  Camera2D* camera2d = (Camera2D*)malloc(sizeof(Camera2D));
  int width = GetRenderWidth();
  int height = GetRenderHeight();
  int dpi_scale = get_dpi_scale();
  camera2d->offset = v2(width / 2.0f, height / 2.0f);
  camera2d->rotation = 0.0f;
  camera2d->target = world->entities[0].position;
  camera2d->zoom = 4.5 * dpi_scale;
  return camera2d;
}

/// Move player
void update_player(entity_t* player) {
  Vector2 input_axis = v2(0.0, 0.0);
  if (IsKeyDown(KEY_A)) {
    input_axis.x = -1;
    player->sprite_id = SPRITE_player_left;
  }
  if (IsKeyDown(KEY_D)) {
    input_axis.x = 1;
    player->sprite_id = SPRITE_player_right;
  }
  if (IsKeyDown(KEY_S)) {
    input_axis.y = 1;
    player->sprite_id = SPRITE_player;
  }
  if (IsKeyDown(KEY_W)) {
    input_axis.y = -1;
    player->sprite_id = SPRITE_player_up;
  }
  input_axis = Vector2Normalize(input_axis);

  Vector2 initial_pos = (Vector2){player->position.x, player->position.y};
  Vector2 movement = Vector2Scale(input_axis, 1.0f);

  // Reset player sprite if not moving
  if (FloatEquals(0.0, movement.x) && FloatEquals(0.0, movement.y)) {
    player->currentFrame = 0;

    sprite_t* player_sprite = &sprites[player->sprite_id];
    player_sprite->frameRec.x = 0;
    return;
  }

  player->position = Vector2Add(initial_pos, movement);
  update_entity_frame(player);
  return;
}
/*
 * Initialize and set the Window size
 */
void setup_window(void) {
  InitWindow(ScreenWidth, ScreenHeight, "Game");
  return;
}

/*
 * Create a new entity inside the world
 */
entity_t* entity_create(void) {
  entity_t* found = 0;
  for (int i = 0; i < MAX_ENTITIES; i++) {
    entity_t* existing = &world->entities[i];
    if (!existing->is_valid) {
      found = existing;
      break;
    }
  }
  // assert(found);  // "No free entities"
  *found = (entity_t){0};
  found->is_valid = true;
  return found;
}

bool ingredients_match(cooking_data_t* data, recipe_t* recipe) {
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
FoodID resolve_ingredients(cooking_data_t* data,
                           recipe_t* recipes,
                           int recipe_count) {
  if (!data->is_valid) {
    return FOOD_nil;  // Invalid or not currently cooking
  }

  // Loop through all the recipes
  for (int i = 0; i < recipe_count; i++) {
    recipe_t* recipe = &recipes[i];

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

entity_t* create_food(entity_t* cooking_station, Vector2 position) {
  entity_t* cooked = entity_create();
  cooking_data_t* cooking_data = get_cooking_data(cooking_station);
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
  *cooking_data = (cooking_data_t){0};

  return cooked;
}
void update_camera(Camera2D* camera, entity_t* player, float dt) {
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
sprite_t load_sprite(const char* path, SpriteID id, int frames) {
  Image image = LoadImage(path);
  if (image.data != NULL) {
    Texture2D texture = LoadTextureFromImage(image);
    UnloadImage(image);
    sprites[id] =
        (sprite_t){.texture = texture,
                   .id = id,
                   .frames = frames,
                   .frameRec = {.x = 0.0f,
                                .y = 0.0f,
                                .width = (float)texture.width / frames,
                                .height = (float)texture.height}};
    return sprites[id];
  }
  return sprites[SPRITE_nil];
}

/*
 * Unloads all loaded textures
 */
void unload_textures(void) {
  for (int i = 0; i < SPRITE_MAX; i++) {
    Texture2D texture = sprites[i].texture;
    UnloadTexture(texture);
  }
}

/*
 * Draws the sprite for an entity and specific offset
 */
void draw_sprite(entity_t* entity, Vector2 offset) {
  sprite_t sprite = sprites[entity->sprite_id];

  Rectangle dest = {.x = entity->position.x + offset.x,
                    .y = entity->position.y + offset.y,
                    .width = entity->size.x,
                    entity->size.y};

  dest = expand_rectangle(dest, 0.5 * entity->distort);
  DrawTexturePro(sprite.texture, sprite.frameRec, dest, v2(0, 0), 0, WHITE);
}

void update_entity_frame(entity_t* entity) {
  // update sprite frame
  sprite_t* entity_sprite = &sprites[entity->sprite_id];
  entity->frameCounter++;

  if (entity->frameCounter >= (60 / FRAME_SPEED)) {
    entity->frameCounter = 0;
    entity->currentFrame++;

    if (entity->currentFrame >= entity_sprite->frames) {
      entity->currentFrame = 0;
    }

    Vector2 sprite_size = get_sprite_size(entity->sprite_id);
    entity_sprite->frameRec.x = (float)entity->currentFrame * sprite_size.x;
  }
}

int get_max_health(ArchetypeID arch) {
  switch (arch) {
    case ARCH_PLAYER:
      return 100;
    case ARCH_ROCK:
      return 5;
    case ARCH_TREE:
      return 10;
    case ARCH_TOMATO_PLANT:
      return 1;
    case ARCH_PUMPKIN_PLANT:
      return 1;
    case ARCH_CORN_PLANT:
      return 1;
    case ARCH_PLANT:
      return 1;
    default:
      return 100;
  };
}
