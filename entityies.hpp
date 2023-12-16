#pragma once
#include <vector>

enum SideType {
    SIDE_WHITE, SIDE_BLACK, SIDE_NONE
};

enum FigureType {
    FIGURE_PAWN, FIGURE_ROOK, FIGURE_BISHOP, FIGURE_KNIGHT, FIGURE_QUEEN, FIGURE_KING, FIGURE_NONE
};

struct Position
{
    int x;
    int y;
    Position();
    Position(int x, int y);
};

class ChessFigure {
    protected:
    FigureType type;
    SideType side;
    Position pos;
    bool alive;
    public:
    ChessFigure();
    ChessFigure(FigureType type, SideType side, int x, int y);
    FigureType get_type();
    SideType get_side();
    Position get_pos();
    void set_type(FigureType input);
    void set_side(SideType input);
    void set_pos(int x, int y);//may be set_pos(Position)
    bool is_alive();
    void die();
    virtual std::vector<Position> get_possible_fields(std::vector<std::vector<ChessFigure*>> & brd) = 0;
};

using pos_arr=std::vector<Position>;
using board_arr=std::vector<std::vector<ChessFigure*>>;

class ChessPawn final : public ChessFigure {
    private:
    bool double_moved;
    public:
    ChessPawn();
    ChessPawn(SideType side, int x, int y);
    bool is_double_moved();
    void trig_double_moved();
    pos_arr get_possible_fields(board_arr & brd) override;
};


class ChessBishop final : public ChessFigure {
    public:
    ChessBishop();
    ChessBishop(SideType side, int x, int y);
    pos_arr get_possible_fields(board_arr & brd) override;
};


class ChessKnight final : public ChessFigure {
    public:
    ChessKnight();
    ChessKnight(SideType side, int x, int y);
    pos_arr get_possible_fields(board_arr & brd) override;
};


class ChessQueen final : public ChessFigure {
    public:
    ChessQueen();
    ChessQueen(SideType side, int x, int y);
    pos_arr get_possible_fields(board_arr & brd) override;
};


class ChessRook final : public ChessFigure {
    private:
    bool moved;
    public:
    ChessRook();
    ChessRook(SideType side, int x, int y);
    bool is_moved();
    void trig_moved();
    pos_arr get_possible_fields(board_arr & brd) override;
};


class ChessKing final : public ChessFigure {
    private:
    bool moved;
    bool under_check;
    bool under_mate;
    public:
    ChessKing();
    ChessKing(SideType side, int x, int y);
    bool is_moved();
    bool is_checked();
    bool is_mated();
    void trig_moved();
    void trig_checked();
    void trig_mated();
    pos_arr get_possible_fields(board_arr & brd) override;
};