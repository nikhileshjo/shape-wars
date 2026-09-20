# Game Engine
Here, we disucuss about each function and what approaches we take to accomplish them
## init function
**Input**: config file address in the form of a standard string.\
**Output**: void
**Processing**:\
* The init expects the config file to be in a certain format, you can refer to the by looking at the [config file](../bin/config.txt). Though, you can change the line order, but you can't change the order of the numbers in within the lines. Below is the key for each of them:
### Window
Window W H FL FS DB
* W = window width
* H = window height
* FL = frame limit
* FS = Fullscreen; 1 = Fullscreen; 0 = Windowed
* DB = Debugger; 1 = debugger activated; 0 = debugger deactivated

### Font
Font file_path FS R G B
* file_path = file path with no spaces
* FS = Font size
* R , G, B = Font color in RGB

### Player
Player SR CR S FR FG FB OR OG OB OT V
* SR = Shape radius
* CR = Collision Radius
* S = Player Speed
* FR FG FB = Fill color in RGB
* OR OG OB = Outer/boundry color in RGB
* OT = Outline thickness
* V = shape vertices

### Enemy
Enemy SR CR SMIN SMAX OR OG OB OT VMIN VMAX L SP

* SR = Shape radius
* CR = Collision Radius
* SMIN SMAX = random speed selction range; min/max speed
* OR OG OB = Outer/bourder color in RGB
* OT = Outer thickness
* VMIN VMAX = random vertex count selction range; min/max vertex count
* L = Small enemy life span in seconds
* SP = Spawn Interval in seconds

### Bullet
Bullet SR CR S FR FG FB OR OG OB OT V L
* SR = Shape radius
* CR = Collision Radius
* S = Bullet Speed
* FR FG FB = Fill color in RGB
* OR OG OB = Outer/boundry color in RGB
* OT = Outline thickness
* V = shape vertices
* L = Bullet life span in seconds

### Keybinds
Keybinds UP LEFT DOWN RIGHT PAUSE
* Assign letters to set costom keybinds
* WASD are defaults
* P for pause

This function also sets up some basic things for the game to run smoothly:
* Setting up text, this configures SFML text according to the text file provided to the program. `std::optional<sf::Text>` is used to declare `m_text` as a member variable because `sf::Text` doesn't have a default constructor, so all it's uses are like pointers.
* Set up the sfml renderer as `m_window` which is accessible through out the script.
* We also set a frame refresh limit.
* One thing to note is, the default behaviour of SFML `pollEvent()` is to register a long press as multiple keypress, we set it to false and register it as a single event and register a key release as another event.
* Spawn player with configured settings.
* If the config is set to debugger activated, debugger is setup too.

# spawnPlayer()
* This function resets the player position in the center of the screen if a player already exists, if not, a player is spawned in the center of the screen. Thiw way we ensure that only one player exists at any given point in the game.
* At the end of the pointer used to refer to the player entity is set to null pointer for safety event though it won't really matter as we are using shared pointers for most the part in this application.

# sUserInput()
* This function is responsible for taking any inputs taken from the user.
* We get all our inputs through `m_window.pollEvent()` which returns a pointer.
* We can use the inbuilt function `getIf<sf::Event::KeyPressed>()` to fetch any events that are created. Their code is stored in scancode that can be converted to int for our convinence.
* We then make use C++ implicit converion of aphabets to int into our advantage to do someChar - 'a', which gives us the scancode equivalent int. The scancode are a = 0, b=1, c=2 and so on till z = 25, so if we did 'a' - 'a' we get a 0.
* We note down both keypress and key release, both trigger an event.
* We also note down mouse inputs using `sf::Event::MouseButtonPressed>()`. It's similar to key inputs, and left click is 0 and 1 is right. We can even note down the position of the click which we use to note down the dirction of bullet shooting.

# sMovement()
* For movements, we check for the flags set by `sUserInput()`, if a key bind is true, we add 1 to that direction. Example, if we press W, it means up, so we add a (-1) to the y coordinate of velocity. And similarly for any other directions.
* We then normalize the resultant to make a unit vector pointer to that direction. Which then is made into a resulting speed of the player using scalar multiplication.
* Enemy speed calculations are done in a similar way, but velocity is decided based on if they "touch" the screen boundry, if they did, we reverse the dirction of the coordinate of which side of the screen they touched.
* Bullet movements are based on the inital veleocity they are given at the time of spawing. And they keep moving into infinity and can cross the border of the screen according to this function.

# spawnEnemy()
* This is very similar to player spawning, but we randomize a few things unlike a player which has all it's attributes fixed.
* We randomize: the number of corners an enemy has, the postion and the speed.
* For each of them we have a range
1. Corners: given in the config file
2. Speed: given in the config file
3. Position: [Collision radius + 1, screen boundry - collision radius - 1]. We set this as the range so that we don't spawn enemies ouside the screen boundery and the collision radius ensures that no part of it is ouside the screen boundry. The additional 1 just ensures that nothing touches the screen boundry either, just to be on the safer side.
* How to assign a certain range to random numbers? We use `rand()` to produce random non-negative integers. To ensure that they are in range, we use this formula: `answer = (rand() % (maxRange - minRange)) + minRange`

# sEnemySpawner()
* This is a `spawnEnemy()` wrapper that triggers it every few fixed intervals given in the config file. We base it off with frame rate.
* We set a counter of : frameRate * spawnIntevalSeconds
* In every frame it reduces the count by 1, once it reaches 0, we spawn an enemy.

# player()
* This is a helper function, the syntax to get the player is pretty long it's the same entity everytimes, so we write a function for it fore easier access.

# sDebugger()


# sCollision()
* There are 2 kinds of collisions **enemy-player collision** and **enemy-bullet collision**.
* Both work in the same way, the only difference is, what happens at collision. If enemy-player collision occurs both the player and enemy get destroyed and the score is reset to 0 and when enemy-bullet collide the smaller enmies are spawned and the bigger enemy is destoyed and the score is increased.
* To calculate the collision we use the formula: `(entity1_CollisionRadius + entity2_CollisionRadius)^2 <= (x2-x1)^2 + (y2-y1)^2` This is distance formula between 2 points in 2D space. We square them to avoid square root which is more expensive to compute.

# sRender()
* We render all the enties to the screen using this function.
* All the angles and position are set to the shapes here (we **calculate** them in sMovement()).
* We also set the transparencey of enties that have a life span based on the percentage of remaining life span over total life span.
* We render score at the top left corner.
* We render imgui GUI if our debugger is set to true.

# spawnBullet()
* This requires the entity pointer and the vector postion of the click that was performed to calculate the the origin and the postion we want the bullets to travel towards.
* Very similar to spawning a player or an enemy, but these have a life span added to them.
* Bullets are spawned at the postion of the player.
* For the direction and speed of the bullet, we take the difference between click location and player location and normalize the result then multiply by the configured speed.

# sLifeSpan()
* This is a counter that just reduces the life span by 1.
* once the counter sets to 0, the entity is destroyed.

# spawnSmallEnemies()
* This is similar to any other entity spawns we discussed above.
* Their count is set by the number of corners the bigger enemy has
* they are positioned at the same location as the enemy.
* Their velocity direction is set by the 360/number of corners. Then we calculate the unit vector pointing to a certain direction then multiply by a fixed speed.

# run()
* run is a wrapper around all the functions, it ensures that every thing is triggered in the right order.
* it runs until the window is open.
* most of the orders don't really matter as they are compensated in the next frame if there are any changes but we always ensure that update() runs at the start and render runs last.