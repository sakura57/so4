#include "CSFMLSectorMapRenderer.hxx"
#include <unordered_map>

#define MAP_WIDTH_PIXELS 500
#define MAP_HEIGHT_PIXELS 500

#define MAP_CIRCLE_RADIUS 5.0f

#define MAP_SQUARE_SIDE 5.0f

CSFMLSectorMapRenderer::CSFMLSectorMapRenderer()
{
	if(!this->m_sfRenderTexture.create(MAP_WIDTH_PIXELS, MAP_HEIGHT_PIXELS))
	{
		throw new SGException("error creating render texture");
	}

	this->initialize_map(Vector2f(40000.0f, 40000.0f));
}

CSFMLSectorMapRenderer::~CSFMLSectorMapRenderer()
{
}

void CSFMLSectorMapRenderer::initialize_map(Vector2f const &dimensions)
{
	this->clear_map();

	SCOPE_LOCK(this->m_mFieldAccess);

	this->m_mapDimensions = dimensions;
}

void CSFMLSectorMapRenderer::add_map_object(MapObjectShape const shape, Vector2f const &position, MapColor const &color)
{
	SCOPE_LOCK(this->m_mFieldAccess);

	this->m_vMapObjects.emplace_back(MapObject{ shape, get_map_position_from_absolute(position), color });
}

void CSFMLSectorMapRenderer::add_map_zone_circular(Vector2f const &position, float const radius, MapColor const &color)
{
	SCOPE_LOCK(this->m_mFieldAccess);

	this->m_vMapZonesCircular.emplace_back(MapZoneCircular{ get_map_position_from_absolute(position), get_map_distance_from_absolute(radius), color });
}

void CSFMLSectorMapRenderer::add_map_zone_rectangular(Vector2f const &position, Vector2f const &dimensions, MapColor const& color)
{
	SCOPE_LOCK(this->m_mFieldAccess);

	this->m_vMapZonesRectangular.emplace_back(MapZoneRectangular{ get_map_position_from_absolute(position), Vector2f(get_map_distance_from_absolute(dimensions.x), get_map_distance_from_absolute(dimensions.y)), color });
}

void CSFMLSectorMapRenderer::update_player_position(Vector2f const &position)
{
	SCOPE_LOCK(this->m_mFieldAccess);

	this->m_playerPosition = get_map_position_from_absolute(position);
}

void CSFMLSectorMapRenderer::set_map_dimensions(Vector2f const &dimensions)
{
	SCOPE_LOCK(this->m_mFieldAccess);

	this->m_mapDimensions = dimensions;
}

void CSFMLSectorMapRenderer::finalize_map(void)
{
	SCOPE_LOCK(this->m_mFieldAccess);
}

void CSFMLSectorMapRenderer::draw_map(void)
{
	SCOPE_LOCK(this->m_mFieldAccess);

	this->m_sfRenderTexture.clear();

	//draw rectangular zones
	for(MapZoneRectangular& zone : this->m_vMapZonesRectangular)
	{
		sf::RectangleShape shape(sf::Vector2f(zone.dimensions.x, zone.dimensions.y));

		shape.setOutlineColor(sf::Color(zone.color.r / 2, zone.color.g / 2, zone.color.b / 2, 182));
		shape.setFillColor(sf::Color(zone.color.r, zone.color.g, zone.color.b, 82));
		shape.setOutlineThickness(1.0f);

		shape.setOrigin(zone.dimensions.x / 2, zone.dimensions.y / 2);

		shape.setPosition(zone.position.x, zone.position.y);

		this->m_sfRenderTexture.draw(shape);
	}

	//draw circular zones
	for(MapZoneCircular& zone : this->m_vMapZonesCircular)
	{
		sf::CircleShape shape(zone.radius);

		shape.setOutlineColor(sf::Color(zone.color.r / 2, zone.color.g / 2, zone.color.b / 2, 182));
		shape.setFillColor(sf::Color(zone.color.r, zone.color.g, zone.color.b, 82));
		shape.setOutlineThickness(1.0f);

		shape.setOrigin(zone.radius, zone.radius);

		shape.setPosition(zone.position.x, zone.position.y);

		this->m_sfRenderTexture.draw(shape);
	}

	//draw objects
	for(MapObject& mapObject : this->m_vMapObjects)
	{
		switch(mapObject.shape)
		{
			case MapObjectShape::CIRCLE:
			{
				sf::CircleShape shape(MAP_CIRCLE_RADIUS);

				shape.setOutlineColor(sf::Color(mapObject.color.r / 2, mapObject.color.g / 2, mapObject.color.b / 2, 232));
				shape.setFillColor(sf::Color(mapObject.color.r, mapObject.color.g, mapObject.color.b, 127));
				shape.setOutlineThickness(1.0f);

				shape.setOrigin(MAP_CIRCLE_RADIUS * cos(M_PI / 4), MAP_CIRCLE_RADIUS * sin(M_PI / 4));
				shape.setPosition(mapObject.position.x, mapObject.position.y);

				this->m_sfRenderTexture.draw(shape);

				break;
			}
			case MapObjectShape::SQUARE:
			{
				sf::RectangleShape shape(sf::Vector2f(MAP_SQUARE_SIDE, MAP_SQUARE_SIDE));

				shape.setOutlineColor(sf::Color(mapObject.color.r / 2, mapObject.color.g / 2, mapObject.color.b / 2, 232));
				shape.setFillColor(sf::Color(mapObject.color.r, mapObject.color.g, mapObject.color.b, 127));
				shape.setOutlineThickness(1.0f);

				shape.setOrigin(MAP_SQUARE_SIDE / 2, MAP_SQUARE_SIDE / 2);

				shape.setPosition(mapObject.position.x, mapObject.position.y);

				this->m_sfRenderTexture.draw(shape);

				break;
			}
			case MapObjectShape::TRIANGLE:
			{
				//TODO: let there be triangles

				throw SGException("Sorry, triangles are not supported yet");
			}
		}
	}

	//finally, draw the player
	sf::RectangleShape shape(sf::Vector2f(MAP_SQUARE_SIDE, MAP_SQUARE_SIDE));

	shape.setFillColor(sf::Color(0, 255, 0, 232));
	shape.setOrigin(MAP_SQUARE_SIDE / 2, MAP_SQUARE_SIDE / 2);
	shape.setPosition(sf::Vector2f(this->m_playerPosition.x, this->m_playerPosition.y));

	this->m_sfRenderTexture.draw(shape);


	//finally, complete the drawing cycle onto the RenderTexture
	this->m_sfRenderTexture.display();
}

void CSFMLSectorMapRenderer::clear_map(void)
{
	SCOPE_LOCK(this->m_mFieldAccess);

	this->m_vMapObjects.clear();
	this->m_vMapZonesCircular.clear();
	this->m_vMapZonesRectangular.clear();

	this->m_mapDimensions = Vector2f(40000.0f, 40000.0f);

	this->m_playerPosition = Vector2f(0.0f, 0.0f);
}

Vector2f CSFMLSectorMapRenderer::get_map_position_from_absolute(Vector2f const &abs_pos)
{
	Vector2f map_pos;

	map_pos.x = (abs_pos.x + this->m_mapDimensions.x / 2) * MAP_WIDTH_PIXELS / this->m_mapDimensions.x;
	map_pos.y = (abs_pos.y + this->m_mapDimensions.y / 2) * MAP_HEIGHT_PIXELS / this->m_mapDimensions.y;

	return map_pos;
}

float CSFMLSectorMapRenderer::get_map_distance_from_absolute(float const abs_dist)
{
	float flAverageScreenDimension = (MAP_WIDTH_PIXELS + MAP_HEIGHT_PIXELS) / 2.0f;
	float flAverageMapDimension = (this->m_mapDimensions.x + this->m_mapDimensions.y) / 2.0f;

	return abs_dist * flAverageScreenDimension / flAverageMapDimension;
}

Vector2f CSFMLSectorMapRenderer::get_absolute_position_from_map(Vector2f const &map_pos)
{
	Vector2f abs_pos;

	abs_pos.x = map_pos.x * this->m_mapDimensions.x / MAP_WIDTH_PIXELS - (this->m_mapDimensions.x / 2);
	abs_pos.y = map_pos.y * this->m_mapDimensions.y / MAP_HEIGHT_PIXELS - (this->m_mapDimensions.y / 2);

	return abs_pos;
}

const sf::Texture& CSFMLSectorMapRenderer::get_map_texture(void)
{
	SCOPE_LOCK(this->m_mFieldAccess);

	return this->m_sfRenderTexture.getTexture();
}

void* CSFMLSectorMapRenderer::get_map_texture_handle(void)
{
	SCOPE_LOCK(this->m_mFieldAccess);

	return (void *)this->m_sfRenderTexture.getTexture().getNativeHandle();
}

void CSFMLSectorMapRenderer::enable_player_icon(bool iconEnabled)
{
	SCOPE_LOCK(this->m_mFieldAccess);

	this->m_bPlayerIconEnabled = iconEnabled;
}