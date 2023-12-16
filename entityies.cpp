#include "entityies.hpp"
#include <iostream>

//Position meths

Position::Position() : x(0), y(0) {}

Position::Position(int x, int y) : x(x), y(y) {}

//Figure meths

ChessFigure::ChessFigure():
type(FIGURE_NONE), side(SIDE_NONE), pos(Position(-1,-1)), alive(true) {}

ChessFigure::ChessFigure(FigureType type, SideType side, int x, int y) : 
type(type), side(side), pos(Position(x,y)), alive(true) {}

FigureType ChessFigure::get_type() {
    return type;
}

SideType ChessFigure::get_side() {
    return side;
}

Position ChessFigure::get_pos() {
    return pos;
}

void ChessFigure::set_type(FigureType input) {
    type = input;
}

void ChessFigure::set_side(SideType input) {
    side = input;
}

void ChessFigure::set_pos(int x, int y) {
    pos.x = x;
    pos.y = y;
}

bool ChessFigure::is_alive() {
    return alive;
}

void ChessFigure::die() {
    alive = !alive;
}

//ChessFigure's children meths


//Pawns construct
ChessPawn::ChessPawn() : 
ChessFigure(FIGURE_PAWN, SIDE_NONE, -1, -1), double_moved(false) {}

ChessPawn::ChessPawn(SideType side, int x, int y) :
ChessFigure(FIGURE_PAWN, side, x, y), double_moved(false) {}

bool ChessPawn::is_double_moved() {
    return double_moved;
}

void ChessPawn::trig_double_moved() {
    double_moved = !double_moved;
}

//Bishops construct
ChessBishop::ChessBishop() : 
ChessFigure(FIGURE_BISHOP, SIDE_NONE, -1, -1) {}

ChessBishop::ChessBishop(SideType side, int x, int y) :
ChessFigure(FIGURE_BISHOP, side, x, y) {}

//Knights construct
ChessKnight::ChessKnight() : 
ChessFigure(FIGURE_KNIGHT, SIDE_NONE, -1, -1) {}

ChessKnight::ChessKnight(SideType side, int x, int y) :
ChessFigure(FIGURE_KNIGHT, side, x, y) {}

//Queens construct
ChessQueen::ChessQueen() : 
ChessFigure(FIGURE_QUEEN, SIDE_NONE, -1, -1) {}

ChessQueen::ChessQueen(SideType side, int x, int y) :
ChessFigure(FIGURE_QUEEN, side, x, y) {}

//Rooks construct & meths

ChessRook::ChessRook() : 
ChessFigure(FIGURE_ROOK, SIDE_NONE, -1, -1), moved(false) {}

ChessRook::ChessRook(SideType side, int x, int y) :
ChessFigure(FIGURE_ROOK, side, x, y), moved(false) {}

bool ChessRook::is_moved() {
    return moved;
}

void ChessRook::trig_moved() {
    moved = !moved;
}

//Kings construct & meths

ChessKing::ChessKing() : 
ChessFigure(FIGURE_KING, SIDE_NONE, -1, -1), moved(false),
under_check(false), under_mate(false) {}

ChessKing::ChessKing(SideType side, int x, int y) :
ChessFigure(FIGURE_KING, side, x, y), moved(false),
under_check(false), under_mate(false) {}

bool ChessKing::is_moved() {
    return moved;
}

bool ChessKing::is_checked() {
    return under_check;
}

bool ChessKing::is_mated() {
    return under_mate;
}

void ChessKing::trig_moved() {
    moved = !moved;
}

void ChessKing::trig_checked() {
    under_check = !under_check;
}

void ChessKing::trig_mated() {
    under_mate = !under_mate;
}

//moves aviliable
//pawns (not counting the situation, when after move you get checked)
//another pawn cut not garanted

pos_arr ChessPawn::get_possible_fields(board_arr & brd) {
    int s = (side == SIDE_WHITE) ? 1 : -1;
    pos_arr ret;
    Position now = pos;
    now.x+=s*1;
    //forward steps
    if (brd[now.x][now.y] == nullptr)
    {
        ret.push_back(now);
    } else {
        now.x = pos.x;
    }
    if (((side == SIDE_WHITE && pos.x == 1) || (side == SIDE_BLACK && pos.x == 6)) &&
        ret.size() > 0 && brd[now.x + s*1][now.y] == nullptr) {
            now.x += s*1;
            ret.push_back(now);
    }
    //diagonal steps
    now.x = pos.x + s*1;
    now.y += 1;
    if (now.y >= 0 && now.y < 8 &&
        brd[now.x][now.y] != nullptr && brd[now.x][now.y]->get_side() != side)
    {
        ret.push_back(now);
    }
    if (now.y >= 0 && now.y < 8 && brd[pos.x][now.y] != nullptr && 
        brd[pos.x][now.y]->get_side() != side && brd[pos.x][now.y]->get_type() == FIGURE_PAWN)
    {
        ChessPawn * target = dynamic_cast<ChessPawn *>(brd[pos.x][now.y]);
        if (target->is_double_moved())
        {
            ret.push_back(now);
        }
    }
    now.y -= 2;
    if (now.y >= 0 && now.y < 8 &&
        brd[now.x][now.y] != nullptr && brd[now.x][now.y]->get_side() != side)
    {
        ret.push_back(now);
    }
    if (now.y >= 0 && now.y < 8 && brd[pos.x][now.y] != nullptr && 
        brd[pos.x][now.y]->get_side() != side && brd[pos.x][now.y]->get_type() == FIGURE_PAWN)
    {
        ChessPawn * target = dynamic_cast<ChessPawn *>(brd[pos.x][now.y]);
        if (target->is_double_moved())
        {
            ret.push_back(now);
        }
    }
    return ret;
}

//bishops (not counting the situation, when after move you het checked)

pos_arr ChessBishop::get_possible_fields(board_arr & brd) {
    pos_arr ret;
    Position now = pos;
    for (int s1 = -1; s1 <= 1; s1 += 2)
    {
        for (int s2 = -1; s2 <= 1; s2 += 2)
        {
            now.x = pos.x;
            now.y = pos.y;
            for (int len = 1; len < 7; ++len)
            {
                now.x = pos.x + s1*len;
                now.y = pos.y + s2*len;
                if (now.x < 0 || now.x > 7 ||
                    now.y < 0 || now.y > 7)
                {
                    break;
                }
                if (brd[now.x][now.y] != nullptr)
                {
                    if (brd[now.x][now.y]->get_side() != side)
                    {
                        ret.push_back(now);
                    }
                    break;
                }
                ret.push_back(now);
            }
        }
    }
    return ret;
}

//Knights (not counting the situation, when after move you get checked)

pos_arr ChessKnight::get_possible_fields(board_arr & brd) {
    pos_arr ret;
    Position now = pos;
    for (int dx = -2; dx <= 2; ++dx)
    {
        if (dx == 0) { continue; }
        for (int dy = -2; dy <= 2; ++dy)
        {
            if (dy == 0 || dx == dy || dx == -1*dy) { continue; }
            now.x = pos.x + dx;
            now.y = pos.y + dy;
            if (now.x >= 0 && now.x <= 7 && now.y >=0 && now.y <= 7 &&
               (brd[now.x][now.y] == nullptr || brd[now.x][now.y]->get_side() != side))
            {
                ret.push_back(now);
            }
        }
    }
    return ret;
}


//Rooks (not counting the situation, when after move you get checked)

pos_arr ChessRook::get_possible_fields(board_arr & brd) {
    pos_arr ret;
    Position now = pos;
    for (int s1 = -1; s1 <= 1; ++s1)
    {
        for (int s2 = -1; s2 <= 1; ++s2)
        {
            if (s1*s2 != 0 || s1 == s2) { continue; }
            for (int len = 1; len <= 7; ++len)
            {
                now.x = pos.x + len*s1;
                now.y = pos.y + len*s2;
                if (now.x < 0 || now.x > 7 || now.y < 0 || now.y > 7) { continue; }
                if (brd[now.x][now.y] != nullptr)
                {
                    if (brd[now.x][now.y]->get_side() != side)
                    {
                        ret.push_back(now);
                    }
                    break;
                }
                ret.push_back(now);
            }
        }
        
    }
    return ret;
}


//Queens (not counting the situation, when after move you get checked)

pos_arr ChessQueen::get_possible_fields(board_arr & brd) {
    pos_arr ret;
    Position now = pos;
    for (int s1 = -1; s1 <= 1; ++s1)
    {
        for (int s2 = -1; s2 <= 1; ++s2)
        {
            if (s1 == 0 && s2 == 0) { continue; }
            for (int len = 1; len <= 7; ++len)
            {
                now.x = pos.x + len*s1;
                now.y = pos.y + len*s2;
                if (now.x < 0 || now.x > 7 || now.y < 0 || now.y > 7) { continue; }
                if (brd[now.x][now.y] != nullptr)
                {
                    if (brd[now.x][now.y]->get_side() != side)
                    {
                        ret.push_back(now);
                    }
                    break;
                }
                ret.push_back(now);
            }
        }
    }
    return ret;
}

//Kings (not counting the situation, when after move you get checked). Castling not supported

pos_arr ChessKing::get_possible_fields(board_arr & brd) {
    pos_arr ret;
    Position now = pos;
    for (int s1 = -1; s1 <= 1; ++s1)
    {
        for (int s2 = -1; s2 <= 1; ++s2)
        {
            if (s1 == 0 && s2 == 0) { continue; }
            now.x = pos.x + s1;
            now.y = pos.y + s2;
            if (now.x < 0 || now.x > 7 || now.y < 0 || now.y > 7) { continue; }
            if (brd[now.x][now.y] == nullptr || brd[now.x][now.y]->get_side() != side)
            {
                ret.push_back(now);
            }
        }
    }
    return ret;
}