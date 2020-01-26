#pragma once

#include "SFMLIncludes.hxx"

#include <random>
#include "Util.hxx"
#include "SGLib.hxx"

class GLStar
{
public:
	GLStar() {};
	GLStar(float const, float const);
	~GLStar() {};
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

class GLStarfield
{
public:
	GLStarfield();
	GLStarfield(float const, float const);
	~GLStarfield();

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

	Vector2f prevPos;

	std::vector<GLStar> sml_stars;
	std::vector<GLStar> med_stars;
	std::vector<GLStar> lrg_stars;

	std::mt19937 &re_x;
	std::mt19937 &re_y;
	std::uniform_real_distribution<float> dist_x;
	std::uniform_real_distribution<float> dist_y;
};
