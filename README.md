## Mini RPG
[![CircleCI](https://circleci.com/gh/ISauve/Mini_RPG.png?style=shield&circle-token=774174d09b31158629a5abf5e26d5f8bd70f62e8)](https://circleci.com/gh/ISauve/Mini_RPG)

A C++ game I wrote to practice multithreading. It uses OpenGL with SFML.

Design patterns used:
- MVC
- Observer

#### Installation
1. Install the dependencies (SFML)
    - `brew update`
    - `brew install sfml`
2. Compile the game with `make`
3. Run with `./bin/game`

#### Running tests
1. `git submodule init`
2. `git submodule update`
3. `make test`
4. `./bin/gtest`

#### TODO
- [ ] Test on Win/Linux
- [x] Add a test framework ([googletest](https://github.com/google/googletest)) and integrate with Circle CI
- [ ] Add character & weapon stats
- [ ] Add items that can be interacted with
- [ ] Add a way to generate playable areas
    - YAML file with the details that gets parsed and generated
    - ie enemies & their stats & locations, items on the screen
- [ ] Add save/continue ability 
- [ ] Add a dependency manager/installation script (?)
- [ ] Refactor sprites to use factory pattern
- [ ] Try to remove all direct accesses among threads/replace locks with channels