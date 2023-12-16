#include "board.hpp"
#include <iostream>
#include <math.h>
#include <SFML/Graphics.hpp>


struct Pair
{
    int x;
    int y;
};


class BoardLayout {
    private:
    int board_x;
    int board_y;
    Board game_board;
    const int tile_size = 64;
    Position selected_pos;
    sf::Color black_tile_color = sf::Color(181,136,99);
    sf::Color white_tile_color = sf::Color(240,217,181);
    sf::Color hilight_color = sf::Color(0, 255, 0, 100);
    sf::Image fig_img;
    sf::Texture fig_text;
    sf::Sprite figure_sprite;
    sf::CircleShape hilghlight_shape;
    std::vector<sf::RectangleShape> field_tiles;
    std::vector<sf::Sprite> figs_sprites;
    std::vector<sf::CircleShape> highligths;
    pos_arr highlights_pos;
    Pair transform_field_coord(Pair input);
    Position transform_position_coord(Position input);
    Pair get_field_coord(int x, int y);
    Pair get_sprite_coords(SideType side, FigureType figure);
    Position get_tile_coord(float x, float y);
    void highlight_fields(pos_arr fields);
    void update_figs_sprites();
    public:
    BoardLayout(int x, int y);
    void loop(sf::RenderWindow & window);
    void event_loader(sf::Event & event);
};