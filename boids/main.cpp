#include "boid.h"
#include <SFML/Graphics.hpp>
#include <stdio.h>

// https://eater.net/boids
// http://www.kfish.org/boids/pseudocode.html

// some constants that define the simulation
constexpr int NUM_BOIDS = 65;
sf::Vector2u windowSize(1600, 1200); // also world size

float dt = .0f;

int main(int argc, char *argv[])
{
	std::vector<boid> boids;
	sf::Clock clock;
	
	sf::RenderWindow window( sf::VideoMode( windowSize ), "Boids!" );
	float coherance = .0f, separation = .0f, alignment = .0f;
	for (int i = 0; i < NUM_BOIDS;++i){
		boids.emplace_back(sf::Vector2f(windowSize.x, windowSize.y));
	}

	boids[0].enableDebugFeatures(true);

	while ( window.isOpen() )
	{
		dt = clock.restart().asSeconds();
		while ( const std::optional event = window.pollEvent() )
		{
			if ( event->is<sf::Event::Closed>() )
				window.close();
		}

		for (boid& b : boids){
			b.update(dt, boids);
		}

		window.clear();
		for (const boid& b : boids){
			window.draw( b );
		}
		
		window.display();
	}
	return 0;
}