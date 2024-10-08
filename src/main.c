#include "raylib.h"
#include "cubicmap.h"

int main(void)
{
    const int screenWidth = 800;
    const int screenHeight = 450;

    InitWindow(screenWidth, screenHeight, "astro janitor");

    Camera camera = { 0 };
    camera.position = (Vector3){ 0.2f, 0.4f, 0.2f };
    camera.target = (Vector3){ 0.185f, 0.4f, 0.0f };
    camera.up = (Vector3){ 0.0f, 1.0f, 0.0f };
    camera.fovy = 45.0f;
    camera.projection = CAMERA_PERSPECTIVE;

    // TODO: Load cubicmap from image data converted with tools
    Image imMap = LoadImage("../resources/cubicmap.png");
    Texture2D cubicmap = LoadTextureFromImage(imMap);
    Mesh mesh = GenMeshCubicmap2(imMap, (Vector3){ 1.0f, 1.0f, 1.0f });
    Model model = LoadModelFromMesh2(mesh);

    Texture2D texture = LoadTexture("../resources/cubicmap_atlas.png");
    model.materials[0].maps[MATERIAL_MAP_DIFFUSE].texture = texture;

    Color *mapPixels = LoadImageColors(imMap);
    UnloadImage(imMap);

    Vector3 mapPosition = { -16.0f, 0.0f, -8.0f };

    DisableCursor();

    SetTargetFPS(60);

    while (!WindowShouldClose())
    {
        // Update
        Vector3 oldCamPos = camera.position;

        UpdateCamera(&camera, CAMERA_FIRST_PERSON);

        // Check player collision (we simplify to 2D collision detection)
        Vector2 playerPos = { camera.position.x, camera.position.z };
        float playerRadius = 0.1f;  // Collision radius (player is modelled as a cilinder for collision)

        int playerCellX = (int)(playerPos.x - mapPosition.x + 0.5f);
        int playerCellY = (int)(playerPos.y - mapPosition.z + 0.5f);

        // Out-of-limits security check
        if (playerCellX < 0) playerCellX = 0;
        else if (playerCellX >= cubicmap.width) playerCellX = cubicmap.width - 1;

        if (playerCellY < 0) playerCellY = 0;
        else if (playerCellY >= cubicmap.height) playerCellY = cubicmap.height - 1;

        // Check map collisions using image data and player position
        // TODO: Improvement: Just check player surrounding cells for collision
        for (int y = 0; y < cubicmap.height; y++)
        {
            for (int x = 0; x < cubicmap.width; x++)
            {
                if ((mapPixels[y*cubicmap.width + x].r == 255) &&       // Collision: white pixel, only check R channel
                    (CheckCollisionCircleRec(playerPos, playerRadius,
                    (Rectangle){ mapPosition.x - 0.5f + x*1.0f, mapPosition.z - 0.5f + y*1.0f, 1.0f, 1.0f })))
                {
                    // Collision detected, reset camera position
                    camera.position = oldCamPos;
                }
            }
        }

        // Draw
        BeginDrawing();

            ClearBackground(RAYWHITE);

            BeginMode3D(camera);
                DrawModel(model, mapPosition, 1.0f, WHITE);                     // Draw maze map
            EndMode3D();

            DrawTextureEx(cubicmap, (Vector2){ GetScreenWidth() - cubicmap.width*4.0f - 20, 20.0f }, 0.0f, 4.0f, WHITE);
            DrawRectangleLines(GetScreenWidth() - cubicmap.width*4 - 20, 20, cubicmap.width*4, cubicmap.height*4, GREEN);
            DrawRectangle(GetScreenWidth() - cubicmap.width*4 - 20 + playerCellX*4, 20 + playerCellY*4, 4, 4, RED);

            DrawFPS(10, 10);

        EndDrawing();
    }

    UnloadImageColors(mapPixels);
    UnloadTexture(cubicmap);
    UnloadTexture(texture);
    UnloadModel(model);
    CloseWindow();

    return 0;
}
