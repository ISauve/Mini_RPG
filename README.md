## Mini RPG

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
3. Run with `./game`

#### Current issues
- [ ] Fix major multithreading issue (poorly designed -> deadlocks/crashes)
- [ ] Install/test on Linux/Win

#### Next steps
1. Add a test framework ([googletest](https://github.com/google/googletest)) and integrate with Circle CI
3. Add character & weapon stats
4. Add items that can be interacted with
5. Add a way to generate playable areas (**factory**)
    - YAML file with the details that gets parsed and generated
    - ie enemies & their stats & locations, items on the screen
6. Add save/continue ability 
7. Add a dependency manager/installation script (?)