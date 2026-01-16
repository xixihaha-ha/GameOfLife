#include "raylib.h"
#include "game_logic.h"  // 引入逻辑
#include "ui_utils.h"    // 引入界面组件
#include <ctime>

// 窗口常量
const int windowWidth = 800;
const int windowHeight = 760;
const int uiHeight = 150;
const int gameHeight = windowHeight - uiHeight;

int main() {
    InitWindow(windowWidth, windowHeight, "Game of Life - Refactored");
    
    // 1. 创建游戏世界对象 (初始 40x30)
    GameWorld game(40, 30);
    
    int speed = 10;
    bool isPaused = true;
    unsigned int currentSeed = (unsigned int)time(NULL);
    
    game.Randomize(20, currentSeed); // 初始随机

    // 临时变量用于绑定 UI 输入框
    int ui_cols = game.GetCols();
    int ui_rows = game.GetRows();

    while (!WindowShouldClose()) {
        SetTargetFPS(speed);

        // --- 逻辑控制层 ---
        // 只有当没有输入文字时，才响应快捷键
        if (!IsInputActive()) {
            if (IsKeyPressed(KEY_SPACE)) isPaused = !isPaused;
            if (IsKeyPressed(KEY_R)) game.Randomize(20, currentSeed);
            if (IsKeyPressed(KEY_C)) { game.Clear(); isPaused = true; }
            
            // 鼠标画图逻辑
            if (IsMouseButtonDown(MOUSE_LEFT_BUTTON) || IsMouseButtonDown(MOUSE_RIGHT_BUTTON)) {
                Vector2 pos = GetMousePosition();
                if (pos.y < gameHeight && pos.y >= 0) {
                    // 动态计算格子大小
                    float cellW = (float)windowWidth / game.GetCols();
                    float cellH = (float)gameHeight / game.GetRows();
                    
                    int x = pos.x / cellW;
                    int y = pos.y / cellH;
                    
                    // 调用逻辑层的 SetCell
                    if (IsMouseButtonDown(MOUSE_LEFT_BUTTON)) game.SetCell(x, y, 1);
                    else game.SetCell(x, y, 0);
                }
            }
        }

        // 演化
        if (!isPaused) {
            game.Update();
        }

        // --- 渲染层 ---
        BeginDrawing();
        ClearBackground(BLACK);

        // 1. 绘制网格 (View)
        int cols = game.GetCols();
        int rows = game.GetRows();
        float cellW = (float)windowWidth / cols;
        float cellH = (float)gameHeight / rows;

        for (int i = 0; i < cols; i++) {
            for (int j = 0; j < rows; j++) {
                if (game.GetCell(i, j) == 1) {
                    DrawRectangle(i * cellW, j * cellH, cellW - 0.5f, cellH - 0.5f, GREEN);
                }
            }
        }

        // 2. 绘制 UI 面板
        DrawRectangle(0, gameHeight, windowWidth, uiHeight, DARKGRAY);
        DrawLine(0, gameHeight, windowWidth, gameHeight, WHITE);

        // 第一行
        float row1_y = gameHeight + 15;
        DrawText("Main:", 20, row1_y + 5, 15, YELLOW);
        if (DrawButton({70, row1_y, 60, 25}, isPaused ? "START" : "PAUSE", !isPaused)) isPaused = !isPaused;
        if (DrawButton({140, row1_y, 60, 25}, "RANDOM", false)) game.Randomize(20, (unsigned int)time(NULL));
        if (DrawButton({210, row1_y, 60, 25}, "CLEAR", false)) { game.Clear(); isPaused = true; }
        
        DrawText("Speed:", 400, row1_y + 5, 15, YELLOW);
        if (DrawButton({460, row1_y, 30, 25}, ">", speed == 10)) speed = 10;
        if (DrawButton({500, row1_y, 30, 25}, ">>", speed == 30)) speed = 30;
        if (DrawButton({540, row1_y, 30, 25}, ">>>", speed == 60)) speed = 60;

        // 第二行 (分辨率)
        float row2_y = gameHeight + 60;
        DrawText("Resolution:", 20, row2_y + 5, 15, YELLOW);
        if (DrawButton({110, row2_y, 30, 25}, "S", cols == 40)) { 
            game.Resize(40, 30); game.Randomize(20, currentSeed); 
            ui_cols = 40; ui_rows = 30; // 同步 UI 显示
        }
        if (DrawButton({150, row2_y, 30, 25}, "M", cols == 80)) { 
            game.Resize(80, 60); game.Randomize(20, currentSeed);
            ui_cols = 80; ui_rows = 60;
        }
        if (DrawButton({190, row2_y, 30, 25}, "L", cols == 160)) { 
            game.Resize(160, 120); game.Randomize(20, currentSeed);
            ui_cols = 160; ui_rows = 120;
        }

        // 自定义输入框
        // 注意：这里我们绑定 ui_cols 变量，确认后再调用 game.Resize
        if (DrawNumBox({290, row2_y, 50, 25}, 1, &ui_cols, "W")) { 
            game.Resize(ui_cols, ui_rows); game.Randomize(20, currentSeed);
        }
        if (DrawNumBox({350, row2_y, 50, 25}, 2, &ui_rows, "H")) { 
            game.Resize(ui_cols, ui_rows); game.Randomize(20, currentSeed);
        }

        // 第三行 (种子 & 预设)
        float row3_y = gameHeight + 105;
        DrawText("Seed:", 20, row3_y + 5, 15, YELLOW);
        if (DrawNumBox({70, row3_y, 100, 25}, 3, (int*)&currentSeed, NULL)) {
            game.Randomize(20, currentSeed);
        }

        DrawText("Presets:", 200, row3_y + 5, 15, YELLOW);
        if (DrawButton({270, row3_y, 60, 25}, "Glider", false)) { 
            game.Clear(); game.DrawGlider(5, 5); isPaused = true; 
        }
        if (DrawButton({340, row3_y, 60, 25}, "Gun", false)) { 
            game.Clear(); 
            if (game.GetCols() < 60) { // 自动扩容逻辑
                 game.Resize(60, 40); ui_cols=60; ui_rows=40; 
            }
            game.DrawGosperGun(5, 5); isPaused = true; 
        }

        EndDrawing();
    }
    CloseWindow();
    return 0;
}