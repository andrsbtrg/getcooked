#include "../raylib/include/raylib.h"
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

#define PLAYER_HEIGHT 80
#define PLAYER_WIDTH 60

int main(void) {
    const int screenWidth = 800;
    const int screenHeight = 600;
    InitWindow(screenWidth, screenHeight, "Game");

    // Setup world
    world = (World *)malloc(sizeof(World));

    Image player = LoadImage("../assets/player.png");
    if (player.data == NULL) {
        return 1;
    }
    Texture2D texture = LoadTextureFromImage(player);
    UnloadImage(player);

    SetTargetFPS(60);
    while (!WindowShouldClose()) {
        // Get the DPI scaling factor
        Vector2 scale = GetWindowScaleDPI();

        // Calculate the centered position considering the DPI scaling
        Rectangle player_rec = {
            (screenWidth - 40) / 2 * scale.x, // Center horizontally
            (screenHeight - 40) / 2 * scale.y, // Center vertically
            40, 40
        };

        Vector2 mouse = GetMousePosition();
        BeginDrawing();
        ClearBackground(DARKGRAY);
        DrawRectangleRec(player_rec, RED);
        DrawText(TextFormat("Default Mouse: [%i , %i]", (int)mouse.x, (int)mouse.y), 
                 400, 25, 20, RED);
        EndDrawing();
    }

    UnloadTexture(texture);
    CloseWindow();

    return 0;
}
