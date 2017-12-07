# Mini RPG
[![CircleCI](https://circleci.com/gh/ISauve/Mini_RPG.png?style=shield&circle-token=774174d09b31158629a5abf5e26d5f8bd70f62e8)](https://circleci.com/gh/ISauve/Mini_RPG)

## _(To do: Add gif here)_

### Installation
1. Clone this repository
2. Install the dependencies ([SFML 2.4.2](https://www.sfml-dev.org/download/sfml/2.4.2/) & [Jsoncpp 1.8.3](https://github.com/open-source-parsers/jsoncpp/releases))
    - On OS X:  `brew update`; `brew install sfml`; `brew install jsoncpp`
3. Compile the game with `make`
4. Run with `./bin/game`

### Running tests locally
1. `git submodule init`
2. `git submodule update`
3. `make test`
4. `./bin/gtest`

---

## To-Do List

#### Gameplay (bold = in progress/next target)
- [x] Character & weapon stats
- [x] Ability to view character/weapon stats
- [x] Support for items that can be interacted with
- [x] A way to generate playable areas via a configuration file
- [x] Weapons & their "active" states should be in the configuration files
- [ ] **Switch configurations to YAML (http://brewformulas.org/YamlCpp)**
- [ ] **Refactor item class to use flyweight pattern & to have weapon/tool subclasses**
- [ ] Add proper error handling (especially for input files)
- [ ] Move animations into configuration files
    - [ ] Related: Have animations be player fields which can be read & interpreted by the view
    - https://gamedev.stackexchange.com/questions/124132/2d-top-down-rpg-animation-architecture-problem
- [ ] Player's speed should also affect timeout before being able to attack again
- [ ] Weapons should have a specific animation location based on character orienation
    - [ ] Related: Direction of attack should matter (at least for the animation)
        - Facing L/R -> attacking L/R
        - Facing down/up -> attacking in direction of enemy
        - Need 4 images of each weapon: active/inactive L/R
- [ ] Friendly characters
- [ ] Money (as a way to upgrade stats & weapons)
- [ ] Merchants
- [ ] Bag for storing items
- [ ] Ability to view bag
- [ ] Shortcuts to items: top of screen should have slots which you can designate as "active" items
- [ ] Item drop ability
    - enemies own items, have a probability of being dropped, killing enemy -> generate drop rate on items, items 
    above a certain threshold get dropped
- [ ] Decorations/background
- [ ] Screen boundaries 
- [ ] Ability to cross between screens (in certain areas)
- [ ] Resting area (can save & possibly heal)
- [ ] Build a short dungeon
    - [ ] Should start off outside, can "train" (aka kill to get money to upgrade weapons/armor/items)
        - Enemies can drop items as well & there should be some hidden items that you can pick up (ie money/hearts in tall grass)
    - [ ] Little village with merchant & resting area
    - [ ] Can enter dungeon from village once you've killed the "practice" boss who drops a key
    
#### Game setup
- [ ] Test on Win/Linux
- [x] Add a test framework ([googletest](https://github.com/google/googletest)) and integrate with Circle CI
- [ ] Write tests
- [ ] Add a dependency manager/installation script (?)
- [ ] Release compiled executables for diff platforms

---

## Game Design

_I made this game for fun, but also to practice multithreading in C++ and implementing various OOP design patterns. 
The game was thus designed with those goals in mind._

### General

There are 3 threads which handle the different parts of the game.  
1. The Drawing Thread, used by the View, handles rendering the game window (at a rate of 10 fps)  
    - the View is an observer of the Model, so it listens for updates which can change the View's state
    - it also actively queries the model for information about the game state: for example, it fetches information about 
      the characters in the area so as to know where to draw them
    
> The `View` class is a subclass of the `Observer` class, and the `Model` is a subclass of the `Subject` class 
  (Observer pattern)
    
2. The Event Thread, used by the Controller, handles interpreting user input (key presses, mouse clicks, etc)
    - to get a consistent player movement speed, the event thread checks for new input exactly once every 100ms
3. The Game Thread, used by the Model, holds all the information about the current game state
    - it receives notifications from the Controller to alter the game state (checks for these once every 50ms)
    - it owns all the characters/items in the game and their behaviours

All the communications between the threads occur via shared thread-safe queues, designed to 
act like Golang Channels.

### Items

There are 2 types of items in the game: props and tools. Every possible item has a corresponding 
`.yaml` configuration file describing it. At the beginning of the game, the `ItemReader` class is responsible
for reading all these files, converting them into `ItemReference` instances and then storing all these instances.

> The `ItemReader` class is restricted so that only one instance of it can exist (Singleton pattern)

Every instance of any item in the game holds a pointer to one of these item references, 
which it then gets its stats from. To instantiate an item at run-time, the `Prop` and `Tool` classes send the item's name
to the `ItemReader` instance, which then finds appropriate item reference and return a pointer to it.

> Items don't store their own data: items of the same type all hold a pointer to the same `ItemReference` instance which
 holds its shared stats - for example, a weapon's image, strength, weight, and associated animations. Individual items 
 are only responsible for storing instance-specific data, like location, number of uses left, etc. (Flyweight design pattern)

**Props**
> The `Prop` class is a subclass of the `Sprite` class

Props are images of items which are seen on the game window (ie lying on the floor, being sold by a merchant, etc).
Some props simply trigger an event when they are interacted with (ie money or a heart getting picked up). Other
props can get acquired by a character, at which point they become a tool. All props are owned by the Model.

**Tools**
> The `Weapon`, `Armor` and `Item` classes are all subclasses of the abstract `Tool` class  
> The instantiation of a `Tool` is deferred to its subclasses (Factory method pattern)

Tools are items which can be actively used by characters - things like potions, weapons, armor, etc.
Tools can be used, sold, or dropped, and even occasionally drawn (ie when a player attacks, the equipped weapon's corresponding animation is rendered). All props are owned by a Character.

All tools can be turned into props for displaying - for example, if a player wants to look at the contents of their bag,
all their tools get turned into props & displayed. Most props can become tools - the exception being props that can't be 
acquired (ie a chest). Props and tools of the same type point to the same `ItemReference`.

