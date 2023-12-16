#include "board.hpp"
#include <iostream>

Player::Player(int time, SideType side) : time_left(time), side(side) {
    if (side == SIDE_NONE)
    {
        std::cout << "wtf" << std::endl;
        throw;
    }
    
    int main_hor = (side == SIDE_WHITE) ? 0 : 7;
    int sec_hor = (side == SIDE_WHITE) ? 1 : 6;
    //pawns init
    pawns = std::vector<ChessPawn>(8);
    for (size_t i = 0; i != 8; ++i)
    {
        pawns[i] = ChessPawn(side, sec_hor, i);
        figs.push_back(&pawns[i]);
    }
    //rooks init
    rooks = std::vector<ChessRook>(2);
    rooks[0] = ChessRook(side, main_hor, 0);
    rooks[1] = ChessRook(side, main_hor, 7);
    figs.push_back(&rooks[0]);
    figs.push_back(&rooks[1]);
    //knights init
    knights = std::vector<ChessKnight>(2);

    knights[0] = ChessKnight(side, main_hor, 1);
    knights[1] = ChessKnight(side, main_hor, 6);
    figs.push_back(&knights[0]);
    figs.push_back(&knights[1]);
    //bishops init
    bishops = std::vector<ChessBishop>(2);
    bishops[0] = ChessBishop(side, main_hor, 2);
    bishops[1] = ChessBishop(side, main_hor, 5);
    figs.push_back(&bishops[0]);
    figs.push_back(&bishops[1]);
    //queen init
    queens = std::vector<ChessQueen>(1);
    queens[0] = ChessQueen(side, main_hor, 3);
    figs.push_back(&queens[0]);
    //king init
    king = ChessKing(side, main_hor, 4);
    figs.push_back(&king);
}


//Board
Board::Board(int time) : state(GAME_GOING), 
white_player(Player(time, SIDE_WHITE)), black_player(Player(time, SIDE_BLACK)) {
    active_player = &white_player;
    inactive_player = &black_player;
    brd = std::vector<std::vector<ChessFigure*>>(8, std::vector<ChessFigure*>(8, nullptr));
    for (int i = 0; i < white_player.figs.size(); ++i) {
        Position pos = white_player.figs[i]->get_pos();
        brd[pos.x][pos.y] = white_player.figs[i];
    }
    for (int i = 0; i < black_player.figs.size(); ++i) {
        Position pos = black_player.figs[i]->get_pos();
        brd[pos.x][pos.y] = black_player.figs[i];
    }
}


bool Board::is_checked() {
    Position victim_pos = active_player->king.get_pos();
    for (ChessFigure * fig : inactive_player->figs)
    {
        if (!fig->is_alive())
        {
            continue;
        }
        
        pos_arr moves = fig->get_possible_fields(brd);
        for (Position tile : moves)
        {
            if (tile.x == victim_pos.x && tile.y == victim_pos.y)
            {
                return true;
            }
        }
    }
    return false;
}

void Board::spawn_fig(FigureType type, int x, int y) {
    int i;
    switch (type)
    {
    case FIGURE_PAWN:
        active_player->pawns.push_back(ChessPawn());
        i = active_player->pawns.size() - 1;
        active_player->pawns[i] = ChessPawn(active_player->side, x, y);
        active_player->figs.push_back(&(active_player->pawns[i]));
        brd[x][y] = &(active_player->pawns[i]);
        break;
    case FIGURE_ROOK:
        active_player->rooks.push_back(ChessRook());
        i = active_player->rooks.size() - 1;
        active_player->rooks[i] = ChessRook(active_player->side, x, y);
        active_player->figs.push_back(&(active_player->rooks[i]));
        brd[x][y] = &(active_player->rooks[i]);
        break;
    case FIGURE_BISHOP:
        active_player->bishops.push_back(ChessBishop());
        i = active_player->bishops.size() - 1;
        active_player->bishops[i] = ChessBishop(active_player->side, x, y);
        active_player->figs.push_back(&(active_player->bishops[i]));
        brd[x][y] = &(active_player->bishops[i]);
        break;
    case FIGURE_KNIGHT:
        active_player->knights.push_back(ChessKnight());
        i = active_player->knights.size() - 1;
        active_player->knights[i] = ChessKnight(active_player->side, x, y);
        active_player->figs.push_back(&(active_player->knights[i]));
        brd[x][y] = &(active_player->knights[i]);
        break;
    case FIGURE_QUEEN:
        active_player->queens.push_back(ChessQueen());
        i = active_player->queens.size() - 1;
        active_player->queens[i] = ChessQueen(active_player->side, x, y);
        active_player->figs.push_back(&(active_player->queens[i]));
        brd[x][y] = &(active_player->queens[i]);
        break;
    }
}


pos_arr Board::get_avaliable_moves(int x, int y) {
    ChessFigure * target = brd[x][y];
    pos_arr ret;
    if (target == nullptr) { return ret; }
    if (active_player->side != target->get_side()) { return ret; }
    
    pos_arr tmp_moves = target->get_possible_fields(brd);
    Position tmp_pos = target->get_pos();
    for (Position move : tmp_moves)
    {
        ChessFigure * tmp = brd[move.x][move.y];
        if (tmp != nullptr)
        {
            tmp->die();
        }
        
        brd[move.x][move.y] = target;
        brd[x][y] =  nullptr;
        if (target->get_type() == FIGURE_KING)
        {
            target->set_pos(move.x, move.y);
        }
        if (!is_checked())
        {
            ret.push_back(move);
        }
        brd[move.x][move.y] = tmp;
        brd[x][y] = target;
        if (tmp != nullptr)
        {
            tmp->die();
        }
        if (target->get_type() == FIGURE_KING)
        {
            target->set_pos(tmp_pos.x, tmp_pos.y);
        }
    }
    if (target->get_type() == FIGURE_KING && !is_checked())
    {
        ChessKing * t = dynamic_cast<ChessKing *>(target);
        int t_hor = t->get_pos().x;
        if (t->is_moved()) { return ret; }
        bool left_way_free = (brd[t_hor][1] == nullptr &&
                              brd[t_hor][2] == nullptr &&
                              brd[t_hor][3] == nullptr);
        bool right_way_free = (brd[t_hor][5] == nullptr &&
                              brd[t_hor][6] == nullptr);
        bool left_first_avaliable = false;
        bool right_first_avaliable = false;
        for (Position p : ret)
        {
            if (p.x == t_hor && p.y == 3)
            {
                left_first_avaliable = true;
            }
            if (p.x == t_hor && p.y == 5)
            {
                right_first_avaliable = true;
            }
        }
        ChessRook * left_rook = &(active_player->rooks[0]);
        ChessRook * right_rook = &(active_player->rooks[1]);
        bool left_rook_stayed = (left_rook->is_alive() &&
                                !left_rook->is_moved());
        
        bool right_rook_stayed = (right_rook->is_alive() &&
                                !right_rook->is_moved());
        if (left_way_free && left_first_avaliable && left_rook_stayed)
        {
            left_rook->set_pos(t_hor,3);
            t->set_pos(t_hor,2);
            brd[t_hor][4] = nullptr;
            brd[t_hor][3] = left_rook;
            brd[t_hor][2] = t;
            brd[t_hor][0] = nullptr;
            if (!is_checked())
            {
                ret.push_back(Position(t_hor, 2));
            }
            left_rook->set_pos(t_hor,0);
            t->set_pos(t_hor,4);
            brd[t_hor][4] = t;
            brd[t_hor][3] = nullptr;
            brd[t_hor][2] = nullptr;
            brd[t_hor][0] = left_rook;
        }
        if (right_way_free && right_first_avaliable && right_rook_stayed)
        {
            right_rook->set_pos(t_hor,5);
            t->set_pos(t_hor,6);
            brd[t_hor][4] = nullptr;
            brd[t_hor][5] = right_rook;
            brd[t_hor][6] = t;
            brd[t_hor][7] = nullptr;
            if (!is_checked())
            {
                ret.push_back(Position(t_hor, 6));
            }
            right_rook->set_pos(t_hor,7);
            t->set_pos(t_hor,4);
            brd[t_hor][4] = t;
            brd[t_hor][5] = nullptr;
            brd[t_hor][6] = nullptr;
            brd[t_hor][7] = right_rook;
        }
    }
    return ret;
}


void Board::move(int x1, int y1, int x2, int y2, FigureType to_change) {
    //earasing doublemooves
    for (int i = 0; i < active_player->pawns.size(); ++i)
    {
        if (active_player->pawns[i].is_double_moved())
        {
            active_player->pawns[i].trig_double_moved();
        }
    }
    ChessFigure * t1 = brd[x1][y1];
    if (t1->get_type() == FIGURE_KING)
    {
        ChessKing * kng = dynamic_cast<ChessKing *>(t1);
        if (!kng->is_moved())
        {
            kng->trig_moved();
        }
        if (y1 - y2 == 2)
        {
            active_player->rooks[0].set_pos(x2, 3);
            kng->set_pos(x2,2);
            std::swap(brd[x1][y1], brd[x2][2]);
            std::swap(brd[x1][0], brd[x1][3]);
            return;
        }
        if (y2 - y1 == 2)
        {
            active_player->rooks[1].set_pos(x2, 5);
            kng->set_pos(x2,6);
            std::swap(brd[x1][y1], brd[x2][6]);
            std::swap(brd[x1][7], brd[x1][5]);
            return;
        }
    }
    if (t1->get_type() == FIGURE_ROOK)
    {
        ChessRook * rk = dynamic_cast<ChessRook *>(t1);
        if (!rk->is_moved())
        {
            rk->trig_moved();
        }
    }
    if (t1->get_type() == FIGURE_PAWN)
    {
        if (x2 - x1 == 2 || x1 - x2 == 2)
        {
            ChessPawn * pw = dynamic_cast<ChessPawn *>(t1);
            pw->trig_double_moved();
        }
        if (y2 - y1 != 0 && brd[x2][y2] == nullptr)
        {
            brd[x1][y2]->die();
            brd[x1][y2] = nullptr;
        }
    }
    if (brd[x2][y2] != nullptr)
    {
        brd[x2][y2]->die();
    }
    t1->set_pos(x2,y2);
    brd[x2][y2] = t1;
    brd[x1][y1] = nullptr;
    if (t1->get_type() == FIGURE_PAWN)
    {
        if (x2 == 0 || x2 == 7)
        {
            t1->die();
            spawn_fig(to_change, x2, y2);
        }
    }
    
}

void Board::swap_sides() {
    std::swap(active_player, inactive_player);
    for (ChessFigure * x : active_player->figs)
    {
        Position pos = x->get_pos();
        pos_arr res = get_avaliable_moves(pos.x, pos.y);
        if (res.size() > 1) { return; }
    }
    state = (inactive_player->side == SIDE_WHITE) ? WHITE_WINS : BLACK_WINS;
    std::cout << ((inactive_player->side == SIDE_WHITE) ? "WHITE" : "BLACK") << " WINS!!!" << std::endl;
}