#include "boid.h"
#include <SFML/Graphics.hpp>
#include <stdio.h>


// https://eater.net/boids
// http://www.kfish.org/boids/pseudocode.html

// some constants that define the simulation
constexpr int NUM_BOIDS = 40;
sf::Vector2u windowSize(800, 600); // also world size

constexpr float NEIGHBOUR_RADIUS = 180.0f; // range in which another boid is considered a neighbour
float r_cohesion = .05f;

constexpr float b_velocity = 110.0f;
constexpr float turnSpeed = 2.0f;
float dt = .0f;

std::vector<const boid*> detectNeighbours(const boid& b, const std::vector<boid>& boids)
{
	std::vector<const boid*> neighbours;
	for (const boid& n : boids)
	{
		if (b == n){	// ignore self
			continue;
		}
		
		sf::Vector2f delta = n.getPosition() - b.getPosition();
		float dist2 = delta.x * delta.x + delta.y * delta.y;
		constexpr float radius2 = NEIGHBOUR_RADIUS * NEIGHBOUR_RADIUS;

		if (dist2 < radius2){
			neighbours.push_back(&n);
		}
		
	}
	return neighbours;
}

void updateBoids(std::vector<boid>& boids)
{
	// rule 1
	// for each boid, detect nearby neighbous, calculate percieved center
	for (boid& b : boids)
	{
		auto neighbours = detectNeighbours(b, boids);

		if (neighbours.empty())
		{
			continue;
		}

		// when all the neighbours have been detected, find the percived center of mass (if there is any neigbours)
		sf::Vector2f com{.0f, 0.f};

		for (const boid* neighbour : neighbours)
		{
			com += neighbour->getPosition();
		}

		com.x /= (float)neighbours.size();
		com.y /= (float)neighbours.size();
		
		// desired direction
		sf::Vector2f desired = com - b.getPosition();
		
		float targetAngle = std::atan2(desired.y, desired.x);

		// smooth turning
		float currentAngle = b.getRotation().asRadians();
		float delta = std::remainder(targetAngle - currentAngle, 2.f * M_PI);

		delta *= r_cohesion; // add cohesion

		float maxStep = turnSpeed * dt;
		float newAngle = currentAngle + std::clamp(delta, -maxStep, maxStep);

		b.setRotation(sf::radians(newAngle));

		// forward movement
		sf::Vector2f forward(std::cos(newAngle), std::sin(newAngle));
		b.setPosition(b.getPosition() + forward * b_velocity * dt);
		

	}
	
	// TODO: rule 2

	// TODO: rule 3
}



int main(int argc, char *argv[])
{
	std::vector<boid> boids;
	sf::Clock clock;
	
	sf::RenderWindow window( sf::VideoMode( windowSize ), "Boids!" );
	float coherance = .0f, separation = .0f, alignment = .0f;
	for (int i = 0; i < NUM_BOIDS;++i){
		boids.emplace_back(coherance, separation, alignment);
	}

	while ( window.isOpen() )
	{
		dt = clock.restart().asSeconds();
		while ( const std::optional event = window.pollEvent() )
		{
			if ( event->is<sf::Event::Closed>() )
				window.close();
		}

		updateBoids(boids);
		for (boid& b : boids){
			b.update();
		}

		window.clear();
		// window.draw( shape );
		for (const boid& b : boids){
			window.draw( b );
		}
		
		window.display();
	}
	return 0;
}