#pragma once

#include <SFML/Graphics.hpp>

class boid : public sf::Drawable, public sf::Transformable
{
	public:
		boid(const sf::Vector2f& worldSize);

		bool operator==(const boid& other) const;
		int getId() const;
		void update(float dt, const std::vector<boid>& flock);
		void draw(sf::RenderTarget& target, sf::RenderStates states) const override;

		void enableDebugFeatures(bool enable);

	private:
		// void separation(std::vector<const boid*> neighbours);
		sf::Vector2f align(std::vector<const boid*> neighbours);
		sf::Vector2f cohesion(std::vector<const boid*> neighbours);
	
		void worldBounds();

	private:
		sf::CircleShape m_shape;
		const int m_id;
		sf::Vector2f m_worldSize;

		sf::Vector2f m_velocity;
		sf::Vector2f m_acceleration;

		// debug vars
		bool m_debugFeatures{false};
		sf::CircleShape m_perceptionCircle;

		// end debug vars

		static int __id_counter__;
};