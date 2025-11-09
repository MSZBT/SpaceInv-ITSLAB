#include<iostream>
#include<U8g2lib.h>
#include<SPI.h>

#define LEFT 15;
#define RIGHT 113;
/*Матрицы для отрисовки все 8 на 8*/

/*Пуля Игрока*/

/*Пуля противника*/

/*Игрок */

/*Противник кадр 1*/

/*Противник кадр 2*/

/*Ячейка игрового поля для противника*/
typedef struct enemy_cell {
    uint8_t x;
    uint8_t y;
    enemy *enemy_pointer;

    enemy_cell(uint8_t x = 0, uint8_t y = 0, bool include_enemy = false, enemy* enemy_ptr = nullptr) 
        : x(x), y(y), enemy_pointer(enemy_ptr) {}
} enemy_cell;

/*Ячейка игрового поля для игрока*/
typedef struct player_cell {
    uint8_t x;
    uint8_t y;
    bool include_player;

    player_cell(uint8_t x = 0, uint8_t y = 0, bool include_enemy = false, enemy* enemy_ptr = nullptr) 
        : x(x), y(y), include_player(include_player) {}
} player_cell;

/*Класс поля для рендеринга и получения информации о нахождении объекта в ячейке*/
class play_field {
private:
    /*размеры в колоннах и строках*/
    static const uint8_t player_row = 1;
    static const uint8_t player_col = 11;
    static const uint8_t enemy_row = 6;
    static const uint8_t enemy_col = 11;

    
    /*Масиивы перемещения */
    player_cell array_player_cell[player_col];
    enemy_cell array_enemy_cell[enemy_row][enemy_col]; 
public:
    /*инициализация массивов перемещения*/
    play_field() {
        init_player_array();
        init_enemy_array();
    }


    /*инициализация массива перемещения игроока*/
    void init_player_array() {
        for (int i = 0; i < player_col; i++) {
            array_player_cell[i] = player_cell(i * (8 + 1), 6 * (8 + 1), false); /*позиция по x y*/
        }
    }


    /*инициализация массива перемещения противников*/
    void init_enemy_array() {
        for (int i = 0; i < enemy_row; i++) {
            for (int j = 0; j < enemy_col; j++) {
                array_enemy_cell[i][j] = enemy_cell(j * (8 + 1), i * (8 + 1), false); /*позиция по x y*/
            }
            
        }
    }


    /*установить игрока в ячейку*/
    void set_player_in_cell(uint8_t index) {
        for (int i = 0; i < player_col; i++) {
            array_player_cell[i].include_player = false;
        }
        array_player_cell[index].include_player = true;
    }


    /*установить противника в ячейку*/
    void set_enemy_in_cell(uint8_t x, uint8_t y, uint8_t x_from, uint8_t y_from, enemy *enemy_pointer) {
        array_enemy_cell[x][y].enemy_pointer = enemy_pointer;
        array_enemy_cell[x_from][y_from].enemy_pointer = nullptr;
    }


    /*получить ячейку игрока для обработки попадания*/
    uint8_t getPlayerPosition() {
        for (int i = 0; i < player_col; i++) {
            if (array_player_cell[i].include_player) return i;
        }
        return 0; 
    }


    /*получить противника обработки попадания*/
    enemy* get_enemy_from_cell(uint8_t x, uint8_t y) {
        if (x < enemy_row && y < enemy_col) {
            return array_enemy_cell[x][y].enemy_pointer;
        }
        return nullptr;
    }


    void draw_field() {
        draw_enemies();
        draw_player();
    }


    /*отрисовка вргаов*/
    void draw_enemies() {
        for (int i = 0; i < enemy_row; i++) {
            for (int j = 0; j < enemy_col; j++) {
                if (array_enemy_cell[i][j].enemy_pointer != nullptr) {
                    u8g2.drawXBMP(array_enemy_cell[i][j].x, array_enemy_cell[i][j].y, 8, 8, ENEMY_BITMAP);
                }
            }
        }
    }


    /*отрисовка игрока*/
    void draw_player() {
        for (int i = 0; i < player_col; i++) {
            if (array_player_cell[i].include_player) {
                u8g2.drawXBMP(array_player_cell[i].x, array_player_cell[i].y, 8, 8, PLAYER_BITMAP);
                break;
            }
        }
    }
}

int main(void) {
    return 0;
}