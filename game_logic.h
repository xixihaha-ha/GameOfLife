#pragma once // 防止重复引用
#include <vector>

// 游戏逻辑类：只管数据和规则，不管画图
class GameWorld {
public:
    // 构造函数
    GameWorld(int width, int height);

    // 核心操作
    void Resize(int newCols, int newRows); // 调整大小
    void Randomize(int density, unsigned int seed); // 随机化
    void Clear(); // 清空
    void Update(); // 演化下一代 (核心算法)

    // 数据访问 (Getter/Setter)
    void SetCell(int x, int y, int state);
    int GetCell(int x, int y) const;
    int GetCols() const { return cols; }
    int GetRows() const { return rows; }

    // 经典图案生成
    void DrawGlider(int x, int y);
    void DrawGosperGun(int x, int y);

private:
    int cols;
    int rows;
    std::vector<std::vector<int>> grid;
    std::vector<std::vector<int>> nextGrid;

    // 内部辅助函数
    int CountNeighbors(int x, int y);
};