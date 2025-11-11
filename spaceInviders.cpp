#include<iostream>
#include<U8g2lib.h>
#include<SPI.h>

#define LEFT 15;
#define RIGHT 113;
/*Матрицы для отрисовки все 8 на 8*/

/*Пуля Игрока*/
const uint8_t EMO_PLAYER[] = {
    0b11111111, 
    0b11111111, 
    0b11100111, 
    0b11010011, 
    0b11001011, 
    0b11010011, 
    0b11000011, 
    0b11111111, 
};

/*Пуля противника*/
const uint8_t EMO_ENEMY[] = {
    0b11111111, 
    0b11111111, 
    0b11100111, 
    0b11001011, 
    0b11010011, 
    0b11100111, 
    0b11111111, 
    0b11111111, 
};

/*Игрок */
const uint8_t PLAYER[] = {
    0b11111111, 
    0b11100111, 
    0b01100110, 
    0b01011010, 
    0b01011010, 
    0b00000000, 
    0b10000001, 
    0b10011001, 
};

/*Противник кадр 1*/
const uint8_t ENEMY_1[] = {
    0b10111101, 
    0b11011011, 
    0b11000011, 
    0b10100101, 
    0b00000000, 
    0b01000010, 
    0b01011010, 
    0b11100111, 
};

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
class Play_field {
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
    Play_field() {
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
};

/*класс игпока*/
class Player {
private:
    /*основные параметры*/
    uint8_t index_inarray;
    int lives;
    bool shooting_flag;

    /*логические характеристики для движения и стрельбы*/
    bool button_is_tate;
    bool button_ls;
    bool button_rs;

    /*указатель на игровое поле*/
    Play_field *field; 

    /*конструктор класса чтобы регулировать параметры игры*/
    
public:
    Player(uint8_t index_inarray = 3, uint8_t lives = 3, bool shooting_flag = true, Play_field *field = nullptr) 
        : index_inarray(index_inarray), lives(lives), shooting_flag(shooting_flag), field(field) {}

    /*движение*/
    void move() {
        /*перемещение вправо*/
        if(!digitalRead(PIN_R) && !buttonIState && index_inarray != 10){
            buttonIState = true;
            index_inarray++;

            field->set_player_in_cell(index_inarray);
        } else buttonIState = false;

        /*перемещение влево*/
        if(!digitalRead(PIN_L) && !buttonIState && index_inarray != 0){
            buttonIState = true;
            index_inarray--;

            field->set_player_in_cell(index_inarray);
        } else buttonIState = false;
    };

    /*стрельба*/
    void shoot() {

    }

    void update_lives() {
        lives--;
    }

    /*проверка на количество жизней и обработка попаданий*/
    bool check_lives() {
        return lives > 0 ? true : false; 
        /*true - продолжается игра*/
        /*flase - конец игры*/
        /*использовать в основном цикле игры*/
    }

    /*проверка на возможность открыть огонь*/
    void check_shooting() {
        /*возможность стрелять условно в 3 секнды*/
    }
};

class Enemy {
private:
    /*основные параметры*/
    bool visibility;
    bool fire_flag;

    uint8_t x_cell;
    uint8_t y_cell;
    Play_field *field; 

public:
    Enemy(bool visibility = true, bool fire_flag = true, Play_field *field = nullptr)
        : x(x), y(y), visibility(visibility), fire_flag(fire_flag), field(field) {}

    /*движение*/
    void move(uint8_t x_new, uint8_t y_new) {
        field->set_enemy_in_cell(x_new, y_new, x_cell, y_cell, this);
    }

    void fire() {
        /*создание объектов-пулек*/
    }

    bool isVisible() {
        return visibility;
    }

    bool flag_update() {
        visibility = !visibility;
    }
};

class Bullet_player {
private:
    uint8_t x_cell;/*присваивается при вытсреле и не меняет траектории*/
    uint8_t y;
    uint8_t speed;
    bool active;

    Play_field *field; 

public:
    Bullet_player(uint8_t x_cell, uint8_t y, uint8_t speed, bool active, Play_field *field = nullptr)
    : x_cell(x_cell), y(y), speed(speed), active(active), field(field) {}

    void check_cell() {
        if (!active) return;
        if (y == 8 || y == 17 || y == 26 || y == 35 || y == 44 || y == 53) {
            uint8_t enemy_y = y / 9;
            
            Enemy *tmp_enemy = field->get_enemy_from_cell(x_cell, enemy_y);
            if (tmp_enemy != nullptr && tmp_enemy->isVisible()) {
                tmp_enemy->flag_update();
                destroy();
            }
        }
    }

    void show() {
        if (!active) return;
        u8g2.drawXBMP(x_cell, y, 8, 8, EMO_PLAYER);
    }

    void move() {
        if (!active) return;
            y -= speed;
            check_cell();
        if (y <= 0) {
            destroy();
        }
    }

    void destroy() {
        active = false;
    }
};

class Bullet_enemy {
private:
    uint8_t x_cell;/*присваивается при вытсреле и не меняет траектории*/
    uint8_t y;
    uint8_t speed;
    bool active;

    Play_field *field; 
    Player *player;

public:
    Bullet_enemy(uint8_t x_cell, uint8_t y, uint8_t speed, bool active, Play_field *field = nullptr, Player *player_ptr = nullptr)
    : x_cell(x_cell), y(y), speed(speed), active(active), field(field), player(player_ptr) {}

    void check_cell() {
        if (!active) return;
        if (y == 54) {
            uint8_t enemy_y = y / 9;
            
            if (field->getPlayerPosition() == x_cell) {
                player->update_lives();
                destroy();
            }
        }
    }

    void show() {
        if (!active) return;
        u8g2.drawXBMP(x_cell, y, 8, 8, EMO_ENEMY);
    }

    void move() {
        if (!active) return;
            y += speed;
            check_cell();
        if (y >= 64) {
            destroy();
        }
    }

    void destroy() {
        active = false;
    }
};

int main(void) {
    return 0;
}