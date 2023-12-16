#include "entityies.hpp"

enum StateType {GAME_GOING, WHITE_WINS, BLACK_WINS};

class Player {
    public:
    int time_left;
    SideType side;
    std::vector<ChessPawn> pawns;
    std::vector<ChessBishop> bishops;
    std::vector<ChessKnight> knights;
    std::vector<ChessRook> rooks;
    std::vector<ChessQueen> queens;
    ChessKing king;
    std::vector<ChessFigure *> figs;
    Player(int time, SideType side);
};

class Board {
    public:
    StateType state;
    board_arr brd;
    Player white_player;
    Player black_player;
    Player * active_player;
    Player * inactive_player;
    Board(int time);
    void spawn_fig(FigureType type, int x, int y);
    bool is_checked(); //Check if it's check for an active player
    pos_arr get_avaliable_moves(int x, int y);
    void move(int x1, int y1, int x2, int y2, FigureType to_change);
    void swap_sides();
};