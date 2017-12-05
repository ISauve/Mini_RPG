## Mini RPG
[![CircleCI](https://circleci.com/gh/ISauve/Mini_RPG.png?style=shield&circle-token=774174d09b31158629a5abf5e26d5f8bd70f62e8)](https://circleci.com/gh/ISauve/Mini_RPG)

## (Add gif here)

#### Installation
1. Clone this repository
2. Install the dependencies ([SFML 2.4.2](https://www.sfml-dev.org/download/sfml/2.4.2/) & [Jsoncpp 1.8.3](https://github.com/open-source-parsers/jsoncpp/releases))
    - On OS X:  `brew update`; `brew install sfml`; `brew install jsoncpp`
3. Compile the game with `make`
4. Run with `./bin/game`

#### Running tests locally
1. `git submodule init`
2. `git submodule update`
3. `make test`
4. `./bin/gtest`

### To-do List

#### Game setup
- [ ] Test on Win/Linux
- [x] Add a test framework ([googletest](https://github.com/google/googletest)) and integrate with Circle CI
- [ ] Write tests
- [ ] Add save/continue ability
- [ ] Add a dependency manager/installation script (?)
- [ ] Release compiled executables for diff platforms

#### Gameplay 
- [x] Add character & weapon stats
- [x] Add ability to view character/weapon stats
- [x] Add items that can be interacted with
- [x] Add a way to generate playable areas
- [x] Add weapons & their "active" states to the config file
- [ ] Player's speed should also affect timeout before being able to attack again
- [ ] Direction of attack should matter (at least for the animation)
    - Facing L/R -> attacking L/R
    - Facing down/up -> attacking in direction of enemy
    - Need 4 images of each weapon: active/inactive L/R
- [ ] Add friendly characters
- [ ] Add money/experience/some way to upgrade stats & weapons
- [ ] Add backgrounds/environment
- [ ] Add boundaries to screens
- [ ] Add ability to cross between screens (in certain areas)
- [ ] Build a short dungeon