#include <SFML/Graphics.hpp>
#include "boid.h"

// https://eater.net/boids

int main(int argc, char *argv[])
{
	sf::Vector2u windowSize(600, 400); // also world size
	sf::RenderWindow window( sf::VideoMode( windowSize ), "Boids!" );
	sf::CircleShape shape( 100.f );
	shape.setFillColor( sf::Color::Green );
	float coherance = .0f, separation = .0f, alignment = .0f;
	boid b({ (int)windowSize.x, (int)windowSize.y}, coherance, separation, alignment);

	while ( window.isOpen() )
	{
		while ( const std::optional event = window.pollEvent() )
		{
			if ( event->is<sf::Event::Closed>() )
				window.close();
		}

		window.clear();
		window.draw( shape );
		window.display();
	}
	return 0;
}