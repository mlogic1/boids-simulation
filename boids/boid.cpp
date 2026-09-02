#include "boid.h"
#include <random>

boid::boid(
	const sf::Vector2i& worldBounds,
	const float& coherence,
	const float& separation,
	const float& alignment
) :
	m_worldBounds(worldBounds),
	m_coherence(coherence),
	m_separation(separation),
	m_alignment(alignment)
{
	std::random_device rd;  
	std::mt19937 gen(rd());  
	std::uniform_int_distribution<> distX(0, worldBounds.x);
	std::uniform_int_distribution<> distY(0, worldBounds.y);

	m_pos.x = distX(gen);
	m_pos.y = distY(gen);
}

void boid::draw(sf::RenderTarget& target, sf::RenderStates states) const
{

}
