#include "CInstanceFactory.hxx"
#include "CSFMLRenderPipeline.hxx"

CSFMLRenderPipeline::CSFMLRenderPipeline()
	:	m_pActiveCamera(nullptr),
		m_uiPastAllocatedAppearances(64),
		m_vecTextures(64),
		m_vecSprites(64),
		m_vecScales(64),
		m_vecAppearanceValid(64, false),
		m_vecAppearanceAdditive(64, false)
{
	this->m_vecAppearanceValid[0] = true;
	//this->m_blurShader.loadFromFile(CGameDataManager::get_full_data_file_path("shaders\\blur_object.vert").c_str(), CGameDataManager::get_full_data_file_path("shaders\\blur_object.frag").c_str());
	//this->m_blurShader.setUniform("blur_radius", 0.05f);
}

CSFMLRenderPipeline::~CSFMLRenderPipeline()
{
	
}

void CSFMLRenderPipeline::load_image(IRenderPipeline::ImageOptions const &imageOpts, char const *szImageFile)
{
	if(imageOpts.uiAppearanceId == 0)
	{
		throw SGException("Cannot load image into appearance slot 0, slot is reserved");
	}

	std::lock_guard<std::mutex> lock(this->m_mFieldAccess);

	std::string szFullImagePath = SG::get_game_data_manager()->get_full_data_file_path(szImageFile);

	if(imageOpts.uiAppearanceId >= this->m_uiPastAllocatedAppearances)
	{
		this->m_vecTextures.resize(imageOpts.uiAppearanceId + 2);
		this->m_vecSprites.resize(imageOpts.uiAppearanceId + 2);
		this->m_vecScales.resize(imageOpts.uiAppearanceId + 2);
		this->m_vecAppearanceValid.resize(imageOpts.uiAppearanceId + 2);
		this->m_vecAppearanceAdditive.resize(imageOpts.uiAppearanceId + 2);

		this->m_uiPastAllocatedAppearances += 2;
	}

	sf::Texture &pTexture = this->m_vecTextures.at(imageOpts.uiAppearanceId);
	sf::Sprite &pSprite = this->m_vecSprites.at(imageOpts.uiAppearanceId);

	if(!pTexture.loadFromFile(szFullImagePath))
	{
		throw SGException("SFML failed to load image file");
	}

	this->m_vecScales[imageOpts.uiAppearanceId] = sf::Vector2f(imageOpts.vScale.x, imageOpts.vScale.y);

	pSprite.setTexture(pTexture);
	pSprite.setOrigin(imageOpts.vCenter.x, imageOpts.vCenter.y);
	pSprite.setScale(imageOpts.vScale.x, imageOpts.vScale.y);

	this->m_vecAppearanceValid[imageOpts.uiAppearanceId] = true;
	this->m_vecAppearanceAdditive[imageOpts.uiAppearanceId] = imageOpts.bAdditive;
}

/*
 * renders a given appearance ID
 * may throw an SGException if the appearance is invalid.
 */
void CSFMLRenderPipeline::render_image(AppearanceId const uiObjectAppearance, sf::RenderWindow &sfWindow,
	Vector2f const &vPosition, float const flRotation, RenderParameters const &renderParms)
{
	if(uiObjectAppearance == APPEARANCE_NONE)
	{
		//if the object has no appearance, just return without
		//rendering anything

		return;
	}

	std::lock_guard<std::mutex> lock(this->m_mFieldAccess);

	if(!this->m_vecAppearanceValid[uiObjectAppearance])
	{
		throw SGException("Tried to render object with invalid appearance");
	}

	sf::Sprite &pSprite = this->m_vecSprites.at(uiObjectAppearance);
	sf::RenderStates states;

	pSprite.setScale(
		this->m_vecScales[uiObjectAppearance].x * renderParms.vScale.x,
		this->m_vecScales[uiObjectAppearance].y * renderParms.vScale.y
	);
	pSprite.setColor(sf::Color(255, 255, 255, (sf::Uint8)renderParms.fAlpha));

	pSprite.setPosition(vPosition.x, vPosition.y);
	pSprite.setRotation(flRotation + 90.0f);

	if(this->m_vecAppearanceAdditive[uiObjectAppearance])
	{
		states.blendMode = sf::BlendAdd;

		//this->m_blurShader.setUniform("texture", sf::Shader::CurrentTexture);
		//this->m_blurShader.setUniform("blur_radius", 0.005f);
		//states.shader = &this->m_blurShader;

		//sfWindow.draw(pSprite, states);

		states.shader = nullptr;
	}
	else
	{
		states.blendMode = sf::BlendAlpha;
		states.shader = nullptr;
	}

	//finally, draw the object
	sfWindow.draw(pSprite, states);
}

/*
 * renders a given object based on its appearance id.
 * may throw an SGException if the appearance is invalid.
 */
void CSFMLRenderPipeline::render_object(IWorldObject *pObject, sf::RenderWindow &sfWindow, RenderParameters const &renderParms)
{
	AppearanceId uiObjectAppearance = pObject->get_appearance();

	this->render_image(uiObjectAppearance, sfWindow, pObject->get_position(), pObject->get_rotation(), renderParms);
}

/*
 * return the active camera. if there is no active camera,
 * this function returns nullptr, and release_active_camera
 * should NOT be called.
 */
ICamera * CSFMLRenderPipeline::acquire_active_camera(void)
{
	this->m_mFieldAccess.lock();

	if(this->m_pActiveCamera == nullptr)
	{
		this->m_mFieldAccess.unlock();
		return nullptr;
	}

	return this->m_pActiveCamera;
}

/*
* release the camera
*/
void CSFMLRenderPipeline::release_active_camera(void)
{
	this->m_mFieldAccess.unlock();
}


/*
 * set_active_camera() should set the camera to the specified
 * camera. NOTE: Callee is responsible for destroying the previous
 * camera! If a camera is already active when this function is called,
 * an exception is thrown!
 */
void CSFMLRenderPipeline::set_active_camera(ICamera *pCamera)
{
	std::lock_guard<std::mutex> lock(this->m_mFieldAccess);

	if(this->m_pActiveCamera != nullptr)
	{
		throw SGException("Attempted to set the active camera without destroying the previous camera");
	}

	//we wish to receive notifications from the camera,
	//for instance, if the camera is destroyed.
	pCamera->listener_add(this);

	this->m_pActiveCamera = pCamera;
}


void CSFMLRenderPipeline::remove_active_camera(void)
{
	std::lock_guard<std::mutex> lock(this->m_mFieldAccess);

	//we no longer wish to receive notifications from you
	this->m_pActiveCamera->listener_remove(this);

	this->m_pActiveCamera = nullptr;
}

bool CSFMLRenderPipeline::can_render_objects(void)
{
	std::lock_guard<std::mutex> lock(this->m_mFieldAccess);

	bool bCanRenderObjects = true;

	//if there is no active camera,
	//nobody should be attempting to render
	//objects
	if(this->m_pActiveCamera == nullptr)
	{
		return false;
	}

	return bCanRenderObjects;
}

/*
 * we probably received a DEPENDENT_DESTROYED notification
 * from the active camera, in which case we should unsubscribe
 * from the camera
 */
void CSFMLRenderPipeline::listen_notify(Notification const notification, IListenable *pListenable)
{
	std::lock_guard<std::mutex> lock(this->m_mFieldAccess);

	ICamera *pListenableCamera = nullptr;

	switch(notification.m_type)
	{
	case Notification::Type::DEPENDENT_DESTROYED:
		pListenableCamera = dynamic_cast<ICamera*>(pListenable);

		if(pListenableCamera == this->m_pActiveCamera)
		{
			//if we invoke listener_remove this actually causes
			//an iterator invalidation problem from the function that calls
			//this function when the camera
			//notifies the pipeline of its destruction
			//pListenableCamera->listener_remove(this);
			this->m_pActiveCamera = nullptr;
		}
		else
		{
			throw SGException("Received notification from unexpected object");
		}

		break;
	default:
		throw SGException("Received unexpected notification");
	}
}