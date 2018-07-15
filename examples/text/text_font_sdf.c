/*******************************************************************************************
*
*   raylib [text] example - TTF loading and usage
*
*   This example has been created using raylib 1.3.0 (www.raylib.com)
*   raylib is licensed under an unmodified zlib/libpng license (View raylib.h for details)
*
*   Copyright (c) 2015 Ramon Santamaria (@raysan5)
*
********************************************************************************************/

#include "raylib.h"

int main()
{
    // Initialization
    //--------------------------------------------------------------------------------------
    int screenWidth = 800;
    int screenHeight = 450;

    InitWindow(screenWidth, screenHeight, "raylib [text] example - SDF fonts");

    // NOTE: Textures/Fonts MUST be loaded after Window initialization (OpenGL context is required)
    
    const char msg[50] = "Signed Distance Fields";

    // Default font generation from TTF font
    Font fontDefault = { 0 };
    fontDefault.baseSize = 16;
    fontDefault.charsCount = 95;
    // Parameters > font size: 16, no chars array provided (0), chars count: 95 (autogenerate chars array)
    fontDefault.chars = LoadFontData("resources/AnonymousPro-Bold.ttf", 16, 0, 95, false);
    // Parameters > chars count: 95, font size: 16, chars padding in image: 4 px, pack method: 0 (default)
    Image atlas = GenImageFontAtlas(fontDefault.chars, 95, 16, 4, 0);
    fontDefault.texture = LoadTextureFromImage(atlas);
    UnloadImage(atlas);
    
    // SDF font generation from TTF font
    // NOTE: SDF chars data is generated with LoadFontData(), it's just a bool option
    Font fontSDF = { 0 };
    fontSDF.baseSize = 16;
    fontSDF.charsCount = 95;
    // Parameters > font size: 16, no chars array provided (0), chars count: 0 (defaults to 95)
    fontSDF.chars = LoadFontData("resources/AnonymousPro-Bold.ttf", 16, 0, 0, true);
    // Parameters > chars count: 95, font size: 16, chars padding in image: 0 px, pack method: 1 (Skyline algorythm)
    atlas = GenImageFontAtlas(fontSDF.chars, 95, 16, 0, 1);
    fontSDF.texture = LoadTextureFromImage(atlas);
    UnloadImage(atlas);
    
    // Load SDF required shader (we use default vertex shader)
    Shader shader = LoadShader(0, "resources/shaders/sdf.fs");
    SetTextureFilter(fontSDF.texture, FILTER_BILINEAR);    // Required for SDF font

    Vector2 fontPosition = { 40, screenHeight/2 - 50 };
    Vector2 textSize = { 0.0f };
    float fontSize = 16.0f;
    int currentFont = 0;    // 0 - fontDefault, 1 - fontSDF

    SetTargetFPS(60);
    //--------------------------------------------------------------------------------------
    
    // Main game loop
    while (!WindowShouldClose())    // Detect window close button or ESC key
    {
        // Update
        //----------------------------------------------------------------------------------
        fontSize += GetMouseWheelMove()*8.0f;
        
        if (fontSize < 6) fontSize = 6;
        
        if (IsKeyDown(KEY_SPACE)) currentFont = 1;
        else currentFont = 0;
        
        if (currentFont == 0) textSize = MeasureTextEx(fontDefault, msg, fontSize, 0);
        else textSize = MeasureTextEx(fontSDF, msg, fontSize, 0);
        
        fontPosition.x = GetScreenWidth()/2 - textSize.x/2;
        fontPosition.y = GetScreenHeight()/2 - textSize.y/2 + 80;
        //----------------------------------------------------------------------------------
        
        // Draw
        //----------------------------------------------------------------------------------
        BeginDrawing();
        
            ClearBackground(RAYWHITE);
            
            if (currentFont == 1)
            {
                // NOTE: SDF fonts require a custom SDf shader to compute fragment color
                BeginShaderMode(shader);    // Activate SDF font shader
                    DrawTextEx(fontSDF, msg, fontPosition, fontSize, 0, BLACK);
                EndShaderMode();            // Activate our default shader for next drawings
                
                DrawTexture(fontSDF.texture, 10, 10, BLACK);
            }
            else 
            {
                DrawTextEx(fontDefault, msg, fontPosition, fontSize, 0, BLACK);
                DrawTexture(fontDefault.texture, 10, 10, BLACK);
            }

            DrawText("FONT SIZE: 16.0", GetScreenWidth() - 240, 20, 20, DARKGRAY);
            DrawText(FormatText("RENDER SIZE: %02.02f", fontSize), GetScreenWidth() - 240, 50, 20, DARKGRAY);
            DrawText("Use MOUSE WHEEL to SCALE TEXT!", GetScreenWidth() - 240, 90, 10, DARKGRAY);

            DrawText("PRESS SPACE to USE SDF FONT VERSION!", 340, GetScreenHeight() - 30, 20, MAROON);
      
        EndDrawing();
        //----------------------------------------------------------------------------------
    }

    // De-Initialization
    //--------------------------------------------------------------------------------------
    UnloadFont(fontDefault);    // Default font unloading
    UnloadFont(fontSDF);        // SDF font unloading
    
    UnloadShader(shader);       // Unload SDF shader
    
    CloseWindow();              // Close window and OpenGL context
    //--------------------------------------------------------------------------------------
    
    return 0;
}