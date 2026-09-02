#pragma once

#include <SFML/Graphics.hpp>

class boid : public sf::Drawable
{
	public:
		boid(
			const sf::Vector2i& worldBounds, 
			const float& coherence,
			const float& separation,
			const float& alignment
		);
		void update();
		virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const override;
		

	private:
		const sf::Vector2i& m_worldBounds;

		sf::Vector2i m_pos;
		const float& m_coherence;
		const float& m_separation;
		const float& m_alignment;
};
