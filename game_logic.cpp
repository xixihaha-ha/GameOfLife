#include "game_logic.h"
#include <cstdlib>
#include <ctime>

// 构造函数：初始化世界
GameWorld::GameWorld(int width, int height) {
    // 设置初始大小，但让 Resize 函数去处理具体的内存分配
    Resize(width, height);
}

// 调整网格大小 (带安全限制)
void GameWorld::Resize(int newCols, int newRows) {
    // 1. 安全限制：防止过小或过大
    if (newCols < 10) newCols = 10;
    if (newCols > 400) newCols = 400;
    if (newRows < 10) newRows = 10;
    if (newRows > 400) newRows = 400;

    cols = newCols;
    rows = newRows;

    // 2. 重置并重新分配内存
    // assign 会自动调整 vector 大小并用 0 填充
    grid.assign(cols, std::vector<int>(rows, 0));
    nextGrid.assign(cols, std::vector<int>(rows, 0));
}

// 随机化网格
void GameWorld::Randomize(int density, unsigned int seed) {
    std::srand(seed); // 设置随机种子
    for (int i = 0; i < cols; i++) {
        for (int j = 0; j < rows; j++) {
            // 根据密度决定死活
            grid[i][j] = (std::rand() % 100 < density) ? 1 : 0;
        }
    }
}

// 清空网格 (全死)
void GameWorld::Clear() {
    for (int i = 0; i < cols; i++) {
        for (int j = 0; j < rows; j++) {
            grid[i][j] = 0;
        }
    }
}

// 设置细胞状态 (带边界检查)
void GameWorld::SetCell(int x, int y, int state) {
    if (x >= 0 && x < cols && y >= 0 && y < rows) {
        grid[x][y] = state;
    }
}

// 获取细胞状态 (带边界检查)
int GameWorld::GetCell(int x, int y) const {
    if (x >= 0 && x < cols && y >= 0 && y < rows) {
        return grid[x][y];
    }
    return 0; // 越界默认为死
}

// 核心辅助函数：计算邻居数量 (包含循环世界逻辑)
int GameWorld::CountNeighbors(int x, int y) {
    int sum = 0;
    for (int i = -1; i < 2; i++) {
        for (int j = -1; j < 2; j++) {
            // 这里的 % 运算实现了"左右互通、上下互通"
            // +cols 是为了处理负数坐标的情况 (例如 -1 + 40 = 39)
            int col = (x + i + cols) % cols;
            int row = (y + j + rows) % rows;
            sum += grid[col][row];
        }
    }
    // 上面的循环把中心点自己也加进去了，所以要减掉
    sum -= grid[x][y];
    return sum;
}

// 核心演化算法：计算下一代
void GameWorld::Update() {
    for (int i = 0; i < cols; i++) {
        for (int j = 0; j < rows; j++) {
            int state = grid[i][j];
            int neighbors = CountNeighbors(i, j);

            // 规则 1: 繁衍 (死 -> 活)
            if (state == 0 && neighbors == 3) {
                nextGrid[i][j] = 1;
            }
            // 规则 2: 死亡 (孤独 <2 或 拥挤 >3)
            else if (state == 1 && (neighbors < 2 || neighbors > 3)) {
                nextGrid[i][j] = 0;
            }
            // 规则 3: 存活 (保持原状)
            else {
                nextGrid[i][j] = state;
            }
        }
    }
    // 将下一代的状态更新到当前网格
    grid = nextGrid;
}

// --- 预设图案绘制 ---

// 绘制滑翔机
void GameWorld::DrawGlider(int x, int y) {
    SetCell(x + 1, y + 0, 1); 
    SetCell(x + 2, y + 1, 1);
    SetCell(x + 0, y + 2, 1); 
    SetCell(x + 1, y + 2, 1); 
    SetCell(x + 2, y + 2, 1);
}

// 绘制高斯帕滑翔机枪 (完整版)
void GameWorld::DrawGosperGun(int x, int y) {
    // 空间检查：如果地图太小放不下，就不画了，防止极其丑陋
    if (cols < 40 || rows < 20) return;

    // 左方块 (2x2)
    SetCell(x, y + 4, 1); SetCell(x, y + 5, 1);
    SetCell(x + 1, y + 4, 1); SetCell(x + 1, y + 5, 1);

    // 左侧结构 (类似于一个C形)
    SetCell(x + 10, y + 4, 1); SetCell(x + 10, y + 5, 1); SetCell(x + 10, y + 6, 1);
    SetCell(x + 11, y + 3, 1); SetCell(x + 11, y + 7, 1);
    SetCell(x + 12, y + 2, 1); SetCell(x + 12, y + 8, 1);
    SetCell(x + 13, y + 2, 1); SetCell(x + 13, y + 8, 1);
    SetCell(x + 14, y + 5, 1);
    SetCell(x + 15, y + 3, 1); SetCell(x + 15, y + 7, 1);
    SetCell(x + 16, y + 4, 1); SetCell(x + 16, y + 5, 1); SetCell(x + 16, y + 6, 1);
    SetCell(x + 17, y + 5, 1);

    // 右侧结构 (朝向左边的箭头)
    SetCell(x + 20, y + 2, 1); SetCell(x + 20, y + 3, 1); SetCell(x + 20, y + 4, 1);
    SetCell(x + 21, y + 2, 1); SetCell(x + 21, y + 3, 1); SetCell(x + 21, y + 4, 1);
    SetCell(x + 22, y + 1, 1); SetCell(x + 22, y + 5, 1);
    SetCell(x + 24, y + 0, 1); SetCell(x + 24, y + 1, 1); SetCell(x + 24, y + 5, 1); SetCell(x + 24, y + 6, 1);

    // 右方块 (2x2)
    SetCell(x + 34, y + 2, 1); SetCell(x + 34, y + 3, 1);
    SetCell(x + 35, y + 2, 1); SetCell(x + 35, y + 3, 1);
}