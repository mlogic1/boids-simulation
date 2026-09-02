cmake_minimum_required(VERSION 3.15)

project(boids VERSION 1.0)

add_executable(boids
    main.cpp
	boid.cpp
)

target_link_libraries(boids PRIVATE
    sfml-system
    sfml-window
    sfml-graphics
)
