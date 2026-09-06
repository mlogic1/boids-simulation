#include "boid.h"
#include <random>

int boid::__id_counter__ = 0;

boid::boid(
	const float& coherence,
	const float& separation,
	const float& alignment
) :
	sf::CircleShape(10.25f, 3),
	m_id(__id_counter__),
	m_coherence(coherence),
	m_separation(separation),
	m_alignment(alignment)
{
	++__id_counter__;
	std::random_device rd;  
	std::mt19937 gen(rd());  
	std::uniform_real_distribution<> distX(0, 800);
	std::uniform_real_distribution<> distY(0, 600);

	setPosition({
		static_cast<float>(distX(gen)), 
		static_cast<float>(distY(gen))
	});
}

bool boid::operator==(const boid& other) const
{
	return m_id == other.m_id;
}

int boid::getId() const
{
	return m_id;
}

void boid::update()
{

}