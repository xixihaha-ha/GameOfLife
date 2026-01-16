#pragma once
#include "raylib.h"
#include <cstdio>
#include <cstring>
#include <cstdlib>

// 全局变量管理输入焦点 (简单起见放在这里，实际项目可以用单例模式)
static int ui_activeInputBox = 0;
static char ui_inputBuffer[32] = { 0 };

// 绘制按钮
inline bool DrawButton(Rectangle rect, const char* text, bool active) {
    Vector2 mousePoint = GetMousePosition();
    bool isHover = CheckCollisionPointRec(mousePoint, rect);
    bool isClicked = false;
    
    if (isHover && IsMouseButtonReleased(MOUSE_LEFT_BUTTON)) isClicked = true;
    
    Color color = LIGHTGRAY;
    if (active) color = SKYBLUE;
    else if (isHover) color = WHITE;
    
    DrawRectangleRec(rect, color);
    DrawRectangleLinesEx(rect, 1, DARKGRAY);
    
    int fontSize = 10;
    int textWidth = MeasureText(text, fontSize);
    DrawText(text, rect.x + (rect.width - textWidth) / 2, rect.y + (rect.height - fontSize)/2, fontSize, BLACK);
    
    return isClicked;
}

// 绘制数字输入框
inline bool DrawNumBox(Rectangle rect, int boxID, int* value, const char* label) {
    if (label) DrawText(label, rect.x, rect.y - 12, 10, LIGHTGRAY);

    Vector2 mousePoint = GetMousePosition();
    bool isHover = CheckCollisionPointRec(mousePoint, rect);
    
    if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
        if (isHover) {
            ui_activeInputBox = boxID;
            sprintf(ui_inputBuffer, "%d", *value); 
        } else if (ui_activeInputBox == boxID) {
            ui_activeInputBox = 0; // 失去焦点
        }
    }

    bool confirmed = false;
    if (ui_activeInputBox == boxID) {
        DrawRectangleRec(rect, WHITE);
        DrawRectangleLinesEx(rect, 2, BLUE); // 激活状态

        int key = GetCharPressed();
        while (key > 0) {
            if ((key >= '0' && key <= '9') && strlen(ui_inputBuffer) < 9) {
                int len = strlen(ui_inputBuffer);
                ui_inputBuffer[len] = (char)key;
                ui_inputBuffer[len + 1] = '\0';
            }
            key = GetCharPressed();
        }
        if (IsKeyPressed(KEY_BACKSPACE)) {
            int len = strlen(ui_inputBuffer);
            if (len > 0) ui_inputBuffer[len - 1] = '\0';
        }
        if (IsKeyPressed(KEY_ENTER)) {
            *value = atoi(ui_inputBuffer); 
            ui_activeInputBox = 0; 
            confirmed = true;
        }
        
        DrawText(ui_inputBuffer, rect.x + 4, rect.y + 4, 20, BLACK);
        if ((int)(GetTime() * 2) % 2 == 0) DrawText("_", rect.x + 4 + MeasureText(ui_inputBuffer, 20), rect.y + 4, 20, BLACK);

    } else {
        DrawRectangleRec(rect, LIGHTGRAY);
        DrawRectangleLinesEx(rect, 1, DARKGRAY);
        DrawText(TextFormat("%d", *value), rect.x + 4, rect.y + 8, 10, BLACK);
    }
    return confirmed;
}

// 获取当前是否有输入框处于激活状态 (用于屏蔽快捷键)
inline bool IsInputActive() {
    return ui_activeInputBox != 0;
}