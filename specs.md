# Program specification

## Player
- The player is represented by a shape which is defined in the config file.
- The player must spawn in the center of the screen at the beginning of the game, and after it dies ( collides with an enemy).
- The player moves by a speed S read from the config file using WASD.
- Note: Set the speed as vector resultant and don't just move the player in speed S in x and y directions. You could do this by normalizing vector and then multiplying it by the scalar S.
- The player is confined to move only withing the bound of the window.
- The player wil shoot a bullet toward the mouse pointer when the left mouse botton is clicked. The speed, size, and lifespan of the bullets are read from the config file.

### Special Ability
- You are free to come up with your own "Special move" which is fired by the player when the right mouse button is clicked.
Constraints on Special Abilities
* Multiple entities (bullets etc) spaned by the special wepon
* Enitites have some uique graphic associate with tehm
* A unique game mechanic is introduced via a new componenet
* A 'cooldown timer' must be implemented for the special wepon.
The properties for the special weapon are not in the config file (as per the assignment), so you'll have to create those parameters yourself and add them to the config.

Jotting down idea for this:
* All the enimies pause in the place they are.
* They shiver in fear (instead of rotating, they rotate little clockwise and little anti clock wise)
* Bullets start to shoot from 0 degrees all the way to 360 degree. So the animation would be like the first bullet radially out from 0 degrees and each one leaves at an interval of 0.5 seconds.
* 10 such bullets are shot out.
* These bullets have life span based on bouncing of the boundry and the number of times you've used special ability. So, when you use it the first time, their life is one bounce off the border and they are over ( then fades off). In the second use, it bounces off once then the second bounce kills them and so on. This needs to happen within one life, once dead, the bounce is reset to 1 bounce limit.
* The the player shakes very regoursly as if trying to expell immense amount of energy and slowly stabilizes as all the 10 bullets leave it.

## Enemy(s):
- Enemies will span in a random location on the screen every X frames, where X is defined int he configuration file.
- Enemies must not overlap the sides of the screen at the time of the spawn ( to avoid infinte colision).
- Enemies shapes have a random number of vertices betewwn a given minimum and maximum specified on the config file.
- Enimeies will be given a random colo upon spawning.
- Enemies will be given a random speed upon spawning, between a minimum and maximum value specifed in the config file.
- When an enemy reaches the edge of the window,it should bounce off in the oposite direction in the same speed.
- when (large) enemies collide with a bullet or player, they are destroyed, and N small enemies spaw in its place, where N is the number of vertices of the original enemy. Each small enemy must have the same number of verties and color of the original enemy. These small enties travel outward at angles at a fixed intervals equal to (360 / N). For example, if the original enemy had 6 sides, the 6 smaller enemies will travel outward in intervals of (360/6) = 60 degrees.
- Samll enemies should be given a "resonable" speed (keep this fixed, it'll give a good animation effect).


## Score:
- Each time an enemy spawns, it is given a score component of N*100, where N is the number of verties it has. Small enemies get double this value.
- If a player bullet kills an enemy, the game score is increased by the score component of the enemy killed.
- The score should be displayed with the font specifed by the config file in the top-left corner of the screen.
- Small enemies should be given double the score of it's larger enemy.

## Drawing:
- In the render system, all entities should be given a slow rotation, which makes the game look a little nicer.
- Any special effects give you brownie points.
- Any entity with a life span that is still alive should have it's color alpha channel set to a ration depending on how long it has left to live. For example, if an entity has a 80 frame life span, and 25 frames remaining, it's alpha values should be set to (float)25/80 * 255. The alpha should go from 255 when it is first spawned, to 0 on the last frame it is alive.


## GUI:
You must construct a GUI using ImGui which has the following functionalities.
* The GUI must display options to turn off each system independently with the exceptionof the rendering and GUI Systems.
* For each entity in the game, the GUI must list the ID, tag, and position of that entity. You must display a list of all entities, as well as list of entities by their tag. You must also have some way of destroying a given entity(except the player) by interacting with the UI element associated with it.
* You must be able to change the enemy spawn interval through the GUI.
* You must be able to manually spawn enemies in some way through the GUI.
* You may develop the GUI in any way that contains this functionality, but it must be easy usable and clearly presented to get full marks.


Misc:
* P should pause the game and pressing P should resume it again too. Everything stops, but the rotation should stay on (it looks cool)
*  Esc should exist the game.



# Task Order

This is a recommendation:
1. Save the config file reading until later, after entities are implemented.
2. Implement the Vec2 class, which you will use for all the components.
3. Implement one componenet at a time.
4. Implement basics of the game class:
    a. Construct a player entity using spawnPlayer() function
    b. Implement basic drawing of entities using the Game::sRender() function
    c. Construct some enemies using the spawnEnemy() function.
    d. Construct a bullet using the spawnBullet() function.
5. Implement player movement in Game::sUserInput and Game::sMovement
6. Implement collision in sCollion and entity.destroy() if it's dead
7. Implement the rest of the game's functionality, including config file reading.
8. Implement the GUI functionality.

TEST AT EACH STEP! DO NOT WAIT FOR THE ENTIRE PROGRAM TO BE READ BEFORE YOU TEST IT.