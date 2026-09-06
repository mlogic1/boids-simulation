#pragma once

#include <SFML/Graphics.hpp>

class boid : public sf::CircleShape
{
	public:
		boid(
			const float& coherence,
			const float& separation,
			const float& alignment
		);

		bool operator==(const boid& other) const;
		int getId() const;
		void update();

	private:
		const int m_id;
		const float& m_coherence;
		const float& m_separation;
		const float& m_alignment;

		static int __id_counter__;
};