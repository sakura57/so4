#pragma once

#include <list>
#include <mutex>
#include <vector>

#include "SFMLIncludes.hxx"

#include "ITransientStateStructure.hxx"
#include "CParticle.hxx"

typedef unsigned int AnimationId;

#define NUM_PARTICLE_CLASSES 64

struct Animation
{
	sf::Texture sfTexture;
	AnimationId id;
	float duration;
	int frames;
	Vector2f frameSize;
	Vector2f origin;
	float scale;
	float alpha;
	int additive;
};

class CParticleManager : public ITransientStateStructure
{
public:
	CParticleManager();

	~CParticleManager()
	{
		for(Animation const *anim : m_vAnimations)
		{
			delete anim;
		}
	};

	void update_particles(sf::RenderWindow &, float const);
	void do_particles(sf::RenderWindow &, float const);

	void add_animation(AnimationId const, Animation const *anim);

	void add_particle(AnimationId const, Vector2f const &, Vector2f const &, float const, float const);

	size_t get_num_particles(void)
	{
		std::lock_guard<std::mutex> lock(this->m_mFieldAccess);

		return this->m_lParticles.size();
	};

	//inherited from ITransientStateStructure
	virtual void shifting_out(void) final;

private:
	std::mutex m_mFieldAccess;

	std::list<CParticle> m_lParticles;
	std::vector<sf::Sprite> m_vAnimationSprites;
	std::vector<Animation const *> m_vAnimations;
	sf::RenderTexture m_sfParticleTarget;
	//sf::Shader m_sfBlurShader;

	bool m_bTargetCreated;
};