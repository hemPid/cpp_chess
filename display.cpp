#include "display.hpp"


Position BoardLayout::get_tile_coord(float x, float y) {
    Position ret;
    ret.x = std::floor((board_y + 8*64 - y) / 64);
    ret.y = std::floor((x - board_x)/64);
    return ret;
}

Pair BoardLayout::transform_field_coord(Pair input) {
    Pair ret;
    if (game_board.active_player->side == SIDE_WHITE)
    {
        ret.x = input.x;
        ret.y = input.y;
    } else {
        ret.x = 2*board_x + 7*64 - input.x;
        ret.y = 2*board_y + 7*64 - input.y;
    }
    return ret;
}

Position BoardLayout::transform_position_coord(Position input) {
    Position ret;
    if (game_board.active_player->side == SIDE_WHITE)
    {
        ret.x = input.x;
        ret.y = input.y;
    } else {
        ret.x = 7 - input.x;
        ret.y = 7 - input.y;
    }
    return ret;
}

Pair BoardLayout::get_field_coord(int x, int y) {
    Pair ret;
    ret.x = board_x + 64*y;
    ret.y = board_y + 64*(7-x);
    return ret;
}

Pair BoardLayout::get_sprite_coords(SideType side, FigureType figure) {
    Pair ret;
    switch (side)
    {
    case SIDE_BLACK:
        ret.y = 0;
        break;
    case SIDE_WHITE:
        ret.y = 64;
        break;
    }
    switch (figure)
    {
        case FIGURE_KING:
            ret.x = 0;
            break;
        case FIGURE_QUEEN:
            ret.x = 64;
            break;
        case FIGURE_ROOK:
            ret.x = 2*64;
            break;
        case FIGURE_KNIGHT:
            ret.x = 3*64;
            break;
        case FIGURE_BISHOP:
            ret.x = 4*64;
            break;
        case FIGURE_PAWN:
            ret.x = 5*64;
            break;
    }
    return ret;
}

void BoardLayout::highlight_fields(pos_arr fields) {
    highligths.clear();
    selected_pos.x = -1;
    selected_pos.y = -1;
    if (!fields.size())
    {
        return;
    }
    for (Position pos : fields)
    {
        Pair crd = get_field_coord(pos.x, pos.y);
        crd = transform_field_coord(crd);
        hilghlight_shape.setPosition(crd.x, crd.y);
        highligths.push_back(hilghlight_shape);
    }
    
}


BoardLayout::BoardLayout(int x, int y) : board_x(x), board_y(y), game_board(Board(1000)) {
    //init of cells
    sf::RectangleShape tile(sf::Vector2f(tile_size,tile_size));
    int i = 0;
    int j = 0;
    bool is_white = false;
    selected_pos.x = -1;
    selected_pos.y = -1;
    for (int k = 0; k < 64; k++)
    {
        i =  k % 8;
        j = k / 8;
        if (i == 0) is_white = !is_white;
        tile.setPosition(board_x + tile_size*i,
                         board_y + tile_size*j);
        if (is_white)
        {
            tile.setFillColor(white_tile_color);
        } else {
            tile.setFillColor(black_tile_color);
        }
        field_tiles.push_back(tile);
        is_white = !is_white;
    }
    //init of hihghlite shape
    hilghlight_shape = sf::CircleShape(32);
    hilghlight_shape.setFillColor(hilight_color);
    //init of figs images
    fig_img.loadFromFile("tileset.png");
    fig_text.loadFromImage(fig_img);
    figure_sprite.setTexture(fig_text);
    Pair crd;
    Position pos;
    Pair sprite_crd;
    for (ChessFigure * now_f : game_board.white_player.figs) {
        crd = get_sprite_coords(now_f->get_side(), now_f->get_type());
        figure_sprite.setTextureRect(sf::IntRect(crd.x,crd.y,64,64));
        pos = now_f->get_pos();
        sprite_crd = get_field_coord(pos.x, pos.y);
        figure_sprite.setPosition(sprite_crd.x, sprite_crd.y);
        figs_sprites.push_back(figure_sprite);
    }
    for (ChessFigure * now_f : game_board.black_player.figs) {
        crd = get_sprite_coords(now_f->get_side(), now_f->get_type());
        figure_sprite.setTextureRect(sf::IntRect(crd.x,crd.y,64,64));
        pos = now_f->get_pos();
        sprite_crd = get_field_coord(pos.x, pos.y);
        figure_sprite.setPosition(sprite_crd.x, sprite_crd.y);
        figs_sprites.push_back(figure_sprite);
    }
}

void BoardLayout::update_figs_sprites() {
    figs_sprites.clear();
    Pair crd;
    Position pos;
    Pair sprite_crd;
    for (ChessFigure * now_f : game_board.white_player.figs) {
        if (!now_f->is_alive())
        {
            continue;
        }
        crd = get_sprite_coords(now_f->get_side(), now_f->get_type());
        figure_sprite.setTextureRect(sf::IntRect(crd.x,crd.y,64,64));
        pos = now_f->get_pos();
        sprite_crd = get_field_coord(pos.x, pos.y);
        sprite_crd = transform_field_coord(sprite_crd);
        figure_sprite.setPosition(sprite_crd.x, sprite_crd.y);
        figs_sprites.push_back(figure_sprite);
    }
    for (ChessFigure * now_f : game_board.black_player.figs) {
        if (!now_f->is_alive())
        {
            continue;
        }
        crd = get_sprite_coords(now_f->get_side(), now_f->get_type());
        figure_sprite.setTextureRect(sf::IntRect(crd.x,crd.y,64,64));
        pos = now_f->get_pos();
        sprite_crd = get_field_coord(pos.x, pos.y);
        sprite_crd = transform_field_coord(sprite_crd);
        figure_sprite.setPosition(sprite_crd.x, sprite_crd.y);
        figs_sprites.push_back(figure_sprite);
    }
}



void BoardLayout::loop(sf::RenderWindow & window) {
    for (sf::RectangleShape x : field_tiles)
    {
        window.draw(x);
    }
    for (sf::CircleShape x : highligths)
    {
        window.draw(x);
    }
    for (sf::Sprite x : figs_sprites)
    {
        window.draw(x);
    }
}

void BoardLayout::event_loader(sf::Event & event) {
    if (event.type == sf::Event::MouseButtonPressed &&
        event.mouseButton.button == sf::Mouse::Left)
    {
        float x = event.mouseButton.x;
        float y = event.mouseButton.y;
        if (board_x > x || (board_x + 8*tile_size) < x ||
            board_y > y || (board_y + 8*tile_size) < y)
        {
            highligths.clear();
            std::cout << "Nothing clicked" << std::endl;
        } else {
            Position res = get_tile_coord(x, y);
            res = transform_position_coord(res);
            bool found = false;
            for (Position t : highlights_pos)
            {
                if (t.x == res.x && t.y == res.y)
                {
                    found = true;
                    break;
                }
            }
            if (found)
            {
                game_board.move(selected_pos.x, selected_pos.y, res.x, res.y, FIGURE_QUEEN);
                selected_pos.x = -1;
                selected_pos.y = -1;
                highlights_pos.clear();
                highligths.clear();
                game_board.swap_sides();
                update_figs_sprites();
            } else {
                highlights_pos = game_board.get_avaliable_moves(res.x,res.y);
                highlight_fields(highlights_pos);
                if (highlights_pos.size() > 0)
                {
                    selected_pos.x = res.x;
                    selected_pos.y = res.y;
                }
            }
        }
        
    }
    
}