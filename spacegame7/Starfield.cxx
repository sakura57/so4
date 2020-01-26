#include "Starfield.hxx"

///////////////////////////////////STARFIELD DEFS

Star::Star(float const xpos, float const ypos)
{
	this->x_pos = xpos;
	this->y_pos = ypos;
}

float Star::get_x_pos()
{
	return this->x_pos;
}

float Star::get_y_pos()
{
	return this->y_pos;
}

void Star::set_x_pos(float const xpos)
{
	this->x_pos = xpos;
}

void Star::set_y_pos(float const ypos)
{
	this->y_pos = ypos;
}

void Star::add_x_pos(float const xpos)
{
	this->x_pos += xpos;
}

void Star::add_y_pos(float const ypos)
{
	this->y_pos += ypos;
}

Starfield::Starfield()
	: re_x(SG::get_random()), re_y(SG::get_random())
{
	this->initialized = false;
	return;
}

Starfield::Starfield(float const x_res, float const y_res)
	: re_x(SG::get_random()), re_y(SG::get_random())
{
	this->x_size = x_res + 10.0f;
	this->y_size = y_res + 10.0f;

	sf::Uint16 sml_star_sz = 2;
	sf::Uint16 med_star_sz = 3;
	sf::Uint16 lrg_star_sz = 4;

	sf::Image sml_star;
	sf::Image med_star;
	sf::Image lrg_star;

	sml_star.create(sml_star_sz, sml_star_sz, sf::Color::White);
	med_star.create(med_star_sz, med_star_sz, sf::Color::White);
	lrg_star.create(lrg_star_sz, lrg_star_sz, sf::Color::White);

	sml_star_img.loadFromImage(sml_star);
	med_star_img.loadFromImage(med_star);
	lrg_star_img.loadFromImage(lrg_star);

	sml_star_spr.setTexture(sml_star_img);
	med_star_spr.setTexture(med_star_img);
	lrg_star_spr.setTexture(lrg_star_img);

	dist_x = std::uniform_real_distribution<float>(0, x_res);
	dist_y = std::uniform_real_distribution<float>(0, y_res);

	//re_x.seed(time(0));
	//re_y.seed(time(0) + 32);

	int reduce_stars = 12;

	this->max_sml_stars = (x_res / (reduce_stars * 10))*(y_res / (reduce_stars * 10));
	this->max_med_stars = (x_res / (reduce_stars * 10 * 2))*(y_res / (reduce_stars * 10 * 2));
	this->max_lrg_stars = (x_res / (reduce_stars * 10 * 2 * 2))*(y_res / (reduce_stars * 10 * 2 * 2));

	while ((int)this->sml_stars.size() <= this->max_sml_stars)
	{
		sml_stars.push_back(Star(this->dist_x(this->re_x), this->dist_y(this->re_y)));
	}

	while ((int)this->med_stars.size() <= this->max_med_stars)
	{
		med_stars.push_back(Star(this->dist_x(this->re_x), this->dist_y(this->re_y)));
	}

	while ((int)this->lrg_stars.size() <= this->max_lrg_stars)
	{
		lrg_stars.push_back(Star(this->dist_x(this->re_x), this->dist_y(this->re_y)));
	}
}

Starfield::~Starfield()
{
	
}

void Starfield::update_starfield(Vector2f const &ovel)
{
	int i;
	//double theta = -atan2(vel.y, vel.x)-(M_PI/2.0);
	Vector2f vel = ovel;
	vel.x *= 10.0f;
	vel.y *= 10.0f;
	double theta = atan2(vel.y, vel.x) - (M_PI);
	double mag = sqrt((vel.y*vel.y) + (vel.x*vel.x));
	for (i = 0; i<sml_stars.size(); i++)
	{
		if (sml_stars.at(i).get_x_pos() > (this->x_size + 10.0f))
		{
			sml_stars.at(i).set_x_pos(1);
			sml_stars.at(i).set_y_pos(this->dist_y(this->re_y));
		}
		else if (sml_stars.at(i).get_x_pos() < -10.0f)
		{
			sml_stars.at(i).set_x_pos(this->x_size - 1);
			sml_stars.at(i).set_y_pos(this->dist_y(this->re_y));
		}
		else if (sml_stars.at(i).get_y_pos() > (this->y_size + 10.0f))
		{
			sml_stars.at(i).set_x_pos(this->dist_x(this->re_x));
			sml_stars.at(i).set_y_pos(1);
		}
		else if (sml_stars.at(i).get_y_pos() < -10.0f)
		{
			sml_stars.at(i).set_x_pos(this->dist_x(this->re_x));
			sml_stars.at(i).set_y_pos(this->y_size - 1);
		}
	}

	for (i = 0; i<med_stars.size(); i++)
	{
		if (med_stars.at(i).get_x_pos() >(this->x_size + 10.0f))
		{
			med_stars.at(i).set_x_pos(1);
			med_stars.at(i).set_y_pos(this->dist_y(this->re_y));
		}
		else if (med_stars.at(i).get_x_pos() < -10.0f)
		{
			med_stars.at(i).set_x_pos(this->x_size - 1);
			med_stars.at(i).set_y_pos(this->dist_y(this->re_y));
		}
		else if (med_stars.at(i).get_y_pos() > (this->y_size + 10.0f))
		{
			med_stars.at(i).set_x_pos(this->dist_x(this->re_x));
			med_stars.at(i).set_y_pos(1);
		}
		else if (med_stars.at(i).get_y_pos() < -10.0f)
		{
			med_stars.at(i).set_x_pos(this->dist_x(this->re_x));
			med_stars.at(i).set_y_pos(this->y_size - 1);
		}
	}

	for (i = 0; i<lrg_stars.size(); i++)
	{
		if (lrg_stars.at(i).get_x_pos() >(this->x_size + 10.0f))
		{
			lrg_stars.at(i).set_x_pos(1);
			lrg_stars.at(i).set_y_pos(this->dist_y(this->re_y));
		}
		else if (lrg_stars.at(i).get_x_pos() < -10.0f)
		{
			lrg_stars.at(i).set_x_pos(this->x_size - 1);
			lrg_stars.at(i).set_y_pos(this->dist_y(this->re_y));
		}
		else if (lrg_stars.at(i).get_y_pos() > (this->y_size + 10.0f))
		{
			lrg_stars.at(i).set_x_pos(this->dist_x(this->re_x));
			lrg_stars.at(i).set_y_pos(1);
		}
		else if (lrg_stars.at(i).get_y_pos() < -10.0f)
		{
			lrg_stars.at(i).set_x_pos(this->dist_x(this->re_x));
			lrg_stars.at(i).set_y_pos(this->y_size - 1);
		}
	}

	for_each(this->sml_stars.begin(), this->sml_stars.end(), [&](Star& p_Star)
	{ p_Star.add_x_pos(0.0025*mag*cos(theta)); p_Star.add_y_pos(0.0025*mag*sin(theta)); });

	for_each(this->med_stars.begin(), this->med_stars.end(), [&](Star& p_Star)
	{ p_Star.add_x_pos(0.004*mag*cos(theta)); p_Star.add_y_pos(0.004*mag*sin(theta)); });

	for_each(this->lrg_stars.begin(), this->lrg_stars.end(), [&](Star& p_Star)
	{ p_Star.add_x_pos(0.00675*mag*cos(theta)); p_Star.add_y_pos(0.00675*mag*sin(theta)); });
}

bool Starfield::init_succeeded()
{
	return this->initialized;
}

void Starfield::draw_starfield(sf::RenderWindow *sWindow)
{
	int i;
	for (i = 0; i<sml_stars.size(); i++)
	{
		this->sml_star_spr.setPosition(this->sml_stars.at(i).get_x_pos(), this->sml_stars.at(i).get_y_pos());
		sWindow->draw(this->sml_star_spr);
	}
	for (i = 0; i<med_stars.size(); i++)
	{
		this->med_star_spr.setPosition(this->med_stars.at(i).get_x_pos(), this->med_stars.at(i).get_y_pos());
		sWindow->draw(this->med_star_spr);
	}
	for (i = 0; i<lrg_stars.size(); i++)
	{
		this->lrg_star_spr.setPosition(this->lrg_stars.at(i).get_x_pos(), this->lrg_stars.at(i).get_y_pos());
		sWindow->draw(this->lrg_star_spr);
	}
}
