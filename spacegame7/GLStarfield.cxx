#include "GLStarfield.hxx"

///////////////////////////////////STARFIELD DEFS

GLStar::GLStar(float const xpos, float const ypos)
{
	this->x_pos = xpos;
	this->y_pos = ypos;
}

float GLStar::get_x_pos()
{
	return this->x_pos;
}

float GLStar::get_y_pos()
{
	return this->y_pos;
}

void GLStar::set_x_pos(float const xpos)
{
	this->x_pos = xpos;
}

void GLStar::set_y_pos(float const ypos)
{
	this->y_pos = ypos;
}

void GLStar::add_x_pos(float const xpos)
{
	this->x_pos += xpos;
}

void GLStar::add_y_pos(float const ypos)
{
	this->y_pos += ypos;
}

GLStarfield::GLStarfield()
	: re_x(SG::get_random()), re_y(SG::get_random())
{
	this->initialized = false;
	return;
}

GLStarfield::GLStarfield(float const x_res, float const y_res)
	: re_x(SG::get_random()), re_y(SG::get_random()),
	prevPos(0.0f, 0.0f)
{
	this->x_size = x_res + 10.0f;
	this->y_size = y_res + 10.0f;

	sf::Uint16 sml_star_sz = 2;
	sf::Uint16 med_star_sz = 3;
	sf::Uint16 lrg_star_sz = 4;

	dist_x = std::uniform_real_distribution<float>(0, x_res);
	dist_y = std::uniform_real_distribution<float>(0, y_res);

	//re_x.seed(time(0));
	//re_y.seed(time(0) + 32);

	int reduce_stars = 50;

	this->max_sml_stars = (x_res / (reduce_stars * 10))*(y_res / (reduce_stars * 10));
	this->max_med_stars = (x_res / (reduce_stars * 10 * 2))*(y_res / (reduce_stars * 10 * 2));
	this->max_lrg_stars = (x_res / (reduce_stars * 10 * 2 * 2))*(y_res / (reduce_stars * 10 * 2 * 2));

	while ((int)this->sml_stars.size() <= this->max_sml_stars)
	{
		sml_stars.push_back(GLStar(this->dist_x(this->re_x), this->dist_y(this->re_y)));
	}

	while ((int)this->med_stars.size() <= this->max_med_stars)
	{
		med_stars.push_back(GLStar(this->dist_x(this->re_x), this->dist_y(this->re_y)));
	}

	while ((int)this->lrg_stars.size() <= this->max_lrg_stars)
	{
		lrg_stars.push_back(GLStar(this->dist_x(this->re_x), this->dist_y(this->re_y)));
	}
}

GLStarfield::~GLStarfield()
{

}

void GLStarfield::update_starfield(Vector2f const &newpos)
{
	int i;

	Vector2f delta = this->prevPos - newpos;
	this->prevPos = newpos;

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

	for_each(this->sml_stars.begin(), this->sml_stars.end(), [&](GLStar& p_GLStar)
	{ p_GLStar.add_x_pos(delta.x); p_GLStar.add_y_pos(delta.y); });

	for_each(this->med_stars.begin(), this->med_stars.end(), [&](GLStar& p_GLStar)
	{ p_GLStar.add_x_pos(delta.x); p_GLStar.add_y_pos(delta.y); });

	for_each(this->lrg_stars.begin(), this->lrg_stars.end(), [&](GLStar& p_GLStar)
	{ p_GLStar.add_x_pos(delta.x); p_GLStar.add_y_pos(delta.y); });
}

bool GLStarfield::init_succeeded()
{
	return this->initialized;
}

void GLStarfield::draw_starfield(sf::RenderWindow *sWindow)
{
	glPushMatrix();

	glTranslatef(this->prevPos.x - this->x_size / 2.0f, this->prevPos.y - this->y_size / 2.0f, 0.0f);

	glDisable(GL_TEXTURE_2D);

	int i;
	for (i = 0; i<sml_stars.size(); i++)
	{
		//this->sml_star_spr.setPosition(this->sml_stars.at(i).get_x_pos(), this->sml_stars.at(i).get_y_pos());
		glPushMatrix();
		glTranslatef(this->sml_stars.at(i).get_x_pos(), this->sml_stars.at(i).get_y_pos(), -7000.0f);
		glColor3f(1.0f, 1.0f, 1.0f);
		glBegin(GL_POLYGON);
		glVertex3f(-8.0f, -8.0f, 0.0f);
		glVertex3f(-8.0f, 8.0f, 0.0f);
		glVertex3f(8.0f, 8.0f, 0.0f);
		glVertex3f(8.0f, -8.0f, 0.0f);
		glEnd();
		glPopMatrix();
	}
	for (i = 0; i<med_stars.size(); i++)
	{
		//this->med_star_spr.setPosition(this->med_stars.at(i).get_x_pos(), this->med_stars.at(i).get_y_pos());
		glPushMatrix();
		glTranslatef(this->med_stars.at(i).get_x_pos(), this->med_stars.at(i).get_y_pos(), -5000.0f);
		glColor3f(1.0f, 1.0f, 1.0f);
		glBegin(GL_POLYGON);
		glVertex3f(-8.0f, -8.0f, 0.0f);
		glVertex3f(-8.0f, 8.0f, 0.0f);
		glVertex3f(8.0f, 8.0f, 0.0f);
		glVertex3f(8.0f, -8.0f, 0.0f);
		glEnd();
		glPopMatrix();
	}
	for (i = 0; i<lrg_stars.size(); i++)
	{
		//this->lrg_star_spr.setPosition(this->lrg_stars.at(i).get_x_pos(), this->lrg_stars.at(i).get_y_pos());
		glPushMatrix();
		glTranslatef(this->lrg_stars.at(i).get_x_pos(), this->lrg_stars.at(i).get_y_pos(), -4000.0f);
		glColor3f(1.0f, 1.0f, 1.0f);
		glBegin(GL_POLYGON);
		//glTexCoord2f(0.0f, 1.0f);
		glVertex3f(-8.0f, -8.0f, 0.0f);
		//glTexCoord2f(0.0f, 0.0f);
		glVertex3f(-8.0f, 8.0f, 0.0f);
		//glTexCoord2f(1.0f, 0.0f);
		glVertex3f(8.0f, 8.0f, 0.0f);
		//glTexCoord2f(1.0f, 1.0f);
		glVertex3f(8.0f, -8.0f, 0.0f);
		glEnd();
		glPopMatrix();
	}

	glPopMatrix();
}
