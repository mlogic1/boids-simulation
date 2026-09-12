#include "boid.h"
#include <random>
#include <iostream>

int boid::__id_counter__ = 0;

constexpr float PERCEPTION_RADIUS = 100.0f;
constexpr float MAX_SPEED = 2.0f;
constexpr float MAX_STEERING_FORCE = 0.1f;

constexpr float SIMULATION_SPEED = 55.0f;

static void setMag(sf::Vector2f& v, float targetMag)
{
	float mag = std::sqrt(v.x*v.x + v.y*v.y);

	if (mag == 0) return; 

	v.x = (v.x / mag) * targetMag;
	v.y = (v.y / mag) * targetMag;
}

static void limit(sf::Vector2f& v, float maxMag)
{
	float magSq = v.x*v.x + v.y*v.y;
	if (magSq > maxMag * maxMag)
	{
		float mag = std::sqrt(magSq);
		v.x = (v.x / mag) * maxMag;
		v.y = (v.y / mag) * maxMag;
	}
}

static std::vector<const boid*> detectNeighbours(const boid& b, const std::vector<boid>& boids)
{
	std::vector<const boid*> neighbours;
	for (const boid& n : boids)
	{
		if (b == n){	// ignore self
			continue;
		}
		
		sf::Vector2f delta = n.getPosition() - b.getPosition();
		float dist2 = delta.x * delta.x + delta.y * delta.y;
		constexpr float radius2 = PERCEPTION_RADIUS * PERCEPTION_RADIUS;

		if (dist2 < radius2){
			neighbours.push_back(&n);
		}
		
	}
	return neighbours;
}

boid::boid(
	const sf::Vector2f& worldSize
) :
	m_shape(10.25f, 3),
	m_id(__id_counter__),
	m_worldSize(worldSize),
	m_perceptionCircle(PERCEPTION_RADIUS)
{
	++__id_counter__;
	std::random_device rd;  
	std::mt19937 gen(rd());
	std::uniform_real_distribution<> distX(0, 800);
	std::uniform_real_distribution<> distY(0, 600);

	std::uniform_real_distribution<float> accl(0.5f, 2.2f);
	std::uniform_real_distribution<float> distF(0.0f, 2.0f * M_PI);
	setPosition(sf::Vector2f{m_worldSize.x / 2.0f, m_worldSize.y / 2.0f});

	setPosition(sf::Vector2f(distX(gen), distY(gen)));
	
	m_shape.setOrigin({ m_shape.getRadius(), m_shape.getRadius() });
	m_perceptionCircle.setOrigin({ m_perceptionCircle.getRadius(), m_perceptionCircle.getRadius() });
	float randomAngle = distF(gen);
	m_velocity = sf::Vector2f(std::cosf(randomAngle), std::sin(randomAngle));
	m_velocity *= accl(gen);

	// debug - perception circle - to be moved to debug class
	m_perceptionCircle.setFillColor(sf::Color::Transparent);
	m_perceptionCircle.setOutlineColor(sf::Color::Cyan);
	m_perceptionCircle.setOutlineThickness(1.2f);
}

bool boid::operator==(const boid& other) const
{
	return m_id == other.m_id;
}

int boid::getId() const
{
	return m_id;
}

void boid::worldBounds()
{
	sf::Vector2f boidPos = getPosition();

	if (boidPos.x < 0){
		boidPos.x = m_worldSize.x;
		setPosition(boidPos);
	}

	if (boidPos.x > m_worldSize.x){
		boidPos.x = 0;
		setPosition(boidPos);
	}

	if (boidPos.y < 0){
		boidPos.y = m_worldSize.y;
		setPosition(boidPos);
	}

	if (boidPos.y > m_worldSize.y){
		boidPos.y = 0;
		setPosition(boidPos);
	}
}

sf::Vector2f boid::align(std::vector<const boid*> neighbours)
{
	sf::Vector2f desired(.0f, .0f);
	for (const boid* neighbour : neighbours)
	{
		desired += neighbour->m_velocity;
	}

	if (neighbours.size() > 0)
	{
		desired /= (float)neighbours.size(); // avg
		setMag(desired, MAX_SPEED); 
		desired -= m_velocity;
		limit(desired, MAX_STEERING_FORCE);
	}

	return desired;
}

sf::Vector2f boid::cohesion(std::vector<const boid*> neighbours)
{
	sf::Vector2f desired(.0f, .0f);

	for (const boid* neighbour : neighbours)
	{
		desired += neighbour->getPosition();
	}

	if (neighbours.size() > 0)
	{
		desired /= (float)neighbours.size();
		desired -= getPosition();
		setMag(desired, MAX_SPEED);
		desired -= m_velocity;
		limit(desired, MAX_STEERING_FORCE);
	}

	return desired;
}

sf::Vector2f boid::separation(std::vector<const boid*> neighbours)
{
	sf::Vector2f desired(.0f, .0f);
	
	const sf::Vector2f boidPos = getPosition();
	for (const boid* neighbour : neighbours)
	{
		sf::Vector2f diff = boidPos - neighbour->getPosition();
		float distSq = diff.x * diff.x + diff.y * diff.y;

		if (distSq > 0.0001f) // avoid zero / near-zero
		{
			// inverse-square falloff
			float d = std::max(distSq, 25.f); // avoid huge forces
			diff /= d;
			desired += diff;
		}
	}

	if (neighbours.size() > 0)
	{
		// desired /= (float)neighbours.size();
		setMag(desired, MAX_SPEED);
		desired -= m_velocity;
		limit(desired, MAX_STEERING_FORCE);
	}

	return desired;
}

// called each frame
void boid::update(float dt, const std::vector<boid>& flock)
{
	worldBounds();
	std::vector<const boid*> neighbours = detectNeighbours(*this, flock);

	// TODO: implement scaling
	sf::Vector2f alignment = align(neighbours);
	sf::Vector2f coh = cohesion(neighbours);
	sf::Vector2f sep = separation(neighbours);
	m_acceleration = alignment + coh + sep;

	m_velocity += m_acceleration * (dt * SIMULATION_SPEED);
	limit(m_velocity, MAX_SPEED);
	
	sf::Vector2f newPosition = getPosition() + m_velocity  * (dt * SIMULATION_SPEED);
	setPosition(newPosition);

	m_shape.setPosition(newPosition);
	float angle = std::atan2(m_velocity.y, m_velocity.x);
	m_shape.setRotation(sf::radians(angle) + sf::degrees(90.0f));


	// debug - perception circle
	m_perceptionCircle.setPosition(newPosition);
}

void boid::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
	target.draw(m_shape, states);

	if (m_debugFeatures)
	{
		// draw forward vector
		// const float angle = getRotation().asRadians();
		//  sf::Vector2f forward(std::cosf(angle), std::sinf(angle));

		sf::Vector2f startPoint = getPosition();
		sf::Vector2f endPoint = startPoint + m_velocity * 3.5f;
		

		target.draw(m_perceptionCircle);

		// direction vector
		std::vector<sf::Vertex> vertices;
		vertices.push_back(sf::Vertex{startPoint, sf::Color::Red});
		vertices.push_back(sf::Vertex{endPoint,   sf::Color::Red});

		target.draw(vertices.data(), vertices.size(), sf::PrimitiveType::Lines);
	}
}

void boid::enableDebugFeatures(bool enable)
{
	m_debugFeatures = enable;
}