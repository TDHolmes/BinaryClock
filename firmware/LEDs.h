#define ROW_8_R    0b000000000001
#define ROW_8_G    0b000000000010
#define ROW_8_B    0b000000000100
#define ROW_4_R    0b000000001000
#define ROW_4_G    0b000000010000
#define ROW_4_B    0b000000100000
#define ROW_2_R    0b000001000000
#define ROW_2_G    0b000010000000
#define ROW_2_B    0b000100000000
#define ROW_1_R    0b001000000000
#define ROW_1_G    0b010000000000
#define ROW_1_B    0b100000000000

#define COL_H_TENS 0b000001
#define COL_H_ONES 0b000010
#define COL_M_TENS 0b000100
#define COL_M_ONES 0b001000
#define COL_S_TENS 0b010000
#define COL_S_ONES 0b100000

void update_LEDs(int row, int column, int red, int green, int blue, int color_bit);