#pragma once

#include "SFMLIncludes.hxx"

#include <random>
#include "Util.hxx"
#include "SGLib.hxx"

class Star
{
public:
	Star() {}
	Star(float const, float const);
	~Star() {}
	float get_x_pos();
	float get_y_pos();
	void set_x_pos(float const);
	void set_y_pos(float const);
	void add_x_pos(float const);
	void add_y_pos(float const);

private:
	float x_pos;
	float y_pos;
};

class Starfield
{
public:
	Starfield();
	Starfield(float const, float const);
	~Starfield();

	bool init_succeeded(void);
	void update_starfield(Vector2f const &vel);
	void draw_starfield(sf::RenderWindow * sWindow);

private:
	bool initialized;
	int max_sml_stars;
	int max_med_stars;
	int max_lrg_stars;

	float x_size;
	float y_size;

	std::vector<Star> sml_stars;
	std::vector<Star> med_stars;
	std::vector<Star> lrg_stars;

	std::mt19937 &re_x;
	std::mt19937 &re_y;
	std::uniform_real_distribution<float> dist_x;
	std::uniform_real_distribution<float> dist_y;

	sf::Texture sml_star_img;
	sf::Texture med_star_img;
	sf::Texture lrg_star_img;

	sf::Sprite sml_star_spr;
	sf::Sprite med_star_spr;
	sf::Sprite lrg_star_spr;
};
