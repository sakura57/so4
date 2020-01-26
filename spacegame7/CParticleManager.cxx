#include "CParticleManager.hxx"

#include <iostream>

CParticleManager::CParticleManager()
{
	//this->m_sfBlurShader.loadFromFile("data\\shaders\\blur_particle.vert", "data\\shaders\\blur_particle.frag");
}

void CParticleManager::update_particles(sf::RenderWindow &sfWindow, float const fDelta)
{
	std::lock_guard<std::mutex> lock(this->m_mFieldAccess);

	std::list<CParticle>::iterator i = this->m_lParticles.begin();

	while (i != this->m_lParticles.end())
	{
		AnimationId id = i->get_animation_id();

		bool bExpired = i->get_elapsed_time() > this->m_vAnimations[id]->duration;

		if (bExpired)
		{
			this->m_lParticles.erase(i++);
		}
		else
		{
			i->update(fDelta);

			++i;
		}
	}
}

void CParticleManager::do_particles(sf::RenderWindow &sfWindow, float const fDelta)
{
	std::lock_guard<std::mutex> lock(this->m_mFieldAccess);

	if(!this->m_bTargetCreated)
	{
		sf::Vector2u size = sfWindow.getSize();

		this->m_sfParticleTarget.create(size.x, size.y);

		this->m_bTargetCreated = true;
	}

	std::list<CParticle>::iterator i = this->m_lParticles.begin();

	sf::RenderStates states;

	int iParticlesRendered = 0;

	while(i != this->m_lParticles.end())
	{
		AnimationId id = i->get_animation_id();

		bool bExpired = i->get_elapsed_time() > this->m_vAnimations[id]->duration;

		if (!bExpired)
		{
			int phase;

			if(this->m_vAnimations[id]->frames != 0)
			{
				phase = (int)((i->get_elapsed_time() / this->m_vAnimations[id]->duration) * this->m_vAnimations[id]->frames);
			}
			else
			{
				phase = 0;
			}

			int frameWidth = (int) this->m_vAnimations[id]->frameSize.x;
			int frameHeight = (int) this->m_vAnimations[id]->frameSize.y;

			Vector2f vPosition = i->get_position();

			this->m_vAnimationSprites[id].setTextureRect(sf::IntRect(frameWidth*phase, 0, frameWidth, frameHeight));
			this->m_vAnimationSprites[id].setOrigin(this->m_vAnimations[id]->origin.x, this->m_vAnimations[id]->origin.y);
			this->m_vAnimationSprites[id].setPosition(vPosition.x, vPosition.y);
			this->m_vAnimationSprites[id].setScale(this->m_vAnimations[id]->scale, this->m_vAnimations[id]->scale);
			this->m_vAnimationSprites[id].setRotation(i->get_rotation() + 90.0f);
			this->m_vAnimationSprites[id].setColor(sf::Color(255.0f, 255.0f, 255.0f, this->m_vAnimations[id]->alpha));

			if (this->m_vAnimations[id]->additive)
			{
				states.blendMode = sf::BlendAdd;
			}
			else
			{
				states.blendMode = sf::BlendAlpha;
			}
			
			states.shader = nullptr;
			sfWindow.draw(this->m_vAnimationSprites[id], states);

			++iParticlesRendered;
			++i;
		}
		else
		{
			this->m_lParticles.erase(i++);
		}
	}
}

void CParticleManager::add_animation(AnimationId const animId, Animation const *anim)
{
	std::lock_guard<std::mutex> lock(this->m_mFieldAccess);

	if(this->m_vAnimations.size() <= animId)
	{
		this->m_vAnimations.resize(animId + 1);
		this->m_vAnimationSprites.resize(animId + 1);
	}

	this->m_vAnimations[animId] = anim;
	this->m_vAnimationSprites[animId] = sf::Sprite(this->m_vAnimations.back()->sfTexture);
}

void CParticleManager::add_particle(AnimationId const anim, Vector2f const &vPos, Vector2f const &vVel, float const fRotation, float const flAngularVel)
{
	std::lock_guard<std::mutex> lock(this->m_mFieldAccess);

	this->m_lParticles.emplace_back(CParticle(vPos, vVel, anim, fRotation, flAngularVel));
}

void CParticleManager::shifting_out(void)
{
	std::lock_guard<std::mutex> lock(this->m_mFieldAccess);

	this->m_lParticles.clear();
}
