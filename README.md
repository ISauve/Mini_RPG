## Mini RPG
[![CircleCI](https://circleci.com/gh/ISauve/Mini_RPG.png?style=shield&circle-token=774174d09b31158629a5abf5e26d5f8bd70f62e8)](https://circleci.com/gh/ISauve/Mini_RPG)

A C++ game I wrote to practice multithreading. It uses OpenGL with SFML.

Design patterns used:
- MVC
- Observer
- (Future: Factory)

#### Installation
1. Install the dependencies (SFML)
    - `brew update`
    - `brew install sfml`
2. Compile the game with `make`
3. Run with `./bin/game`

#### Current issues
- [ ] Install/test on Linux/Win

#### Next steps
1. Add a test framework ([googletest](https://github.com/google/googletest)) and integrate with Circle CI
2. Add character & weapon stats
3. Add items that can be interacted with
4. Add a way to generate playable areas (**factory**)
    - YAML file with the details that gets parsed and generated
    - ie enemies & their stats & locations, items on the screen
5. Add save/continue ability 
6. Add a dependency manager/installation script (?)