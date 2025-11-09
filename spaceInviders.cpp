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

/*Ячейка игрового поля*/
typedef struct enemy_cell {
    uint8_t x;
    uint8_t y;
    bool include_enemy;
    enemy *enemy_pointer;

    enemy_cell(uint8_t x = 0, uint8_t y = 0, bool include_enemy = false, enemy* enemy_ptr = nullptr) 
        : x(x), y(y), include_enemy(include_enemy), enemy_pointer(enemy_ptr) {}
} enemy_cell;

typedef struct player_cell {
    uint8_t x;
    uint8_t y;
    bool include_player;

    player_cell(uint8_t x = 0, uint8_t y = 0, bool include_enemy = false, enemy* enemy_ptr = nullptr) 
        : x(x), y(y), include_player(include_player) {}
} player_cell;

/*Масиивы перемещения */
player_cell array_player_cell[11];
enemy_cell array_enemy_cell[6][11]; 

/*инициализация массива перемещения игроока*/
void init_player_array(player_cell array_player_cell[], int size) {
    for (int i = 0; i < size; i++) {
        array_player_cell[i] = player_cell(i * (8 + 1), 6 * (8 + 1), false); /*позиция по x y*/
    }
}
init_player_array(array_player_cell, 11);

/*инициализация массива перемещения противников*/
void init_enemy_array(enemy_cell array_enemy_cell[6][11], int size_x, int size_y) {
    for (int i = 0; i < size_y; i++) {
        for (int j = 0; j < size_x; j++) {
            array_enemy_cell[i][j] = enemy_cell(j * (8 + 1), i * (8 + 1), false); /*позиция по x y*/
        }
        
    }
};
init_enemy_array(array_enemy_cell, 11, 6);


int main(void) {
    return 0;
}