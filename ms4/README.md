What to submit
1. Code in your code repository 
2. Unit test cases in your code repository
    It could be exactly the same as what you submitted for milestone 3.A, or it could includes some minor changes.
3. A makefile or something equivalent to make file that allows the TA to easily build your code, together with all the test cases.
4. A readme file, specifying:
(1) how to compile
(2) how to run your code
(3) how to run the unit test cases
(4) please suggest some acceptance tests for the TA to try (i.e., what inputs to use, and what outputs are expected)

### (5) text description of what is implemented. You can refer to the use cases and user stories in your design document.

For our description of what is implemented, we will walk through each use case we planned for Iteration 2 from our original design document (from milestone 2).

##### Character Navigation
- character can navigate with abilities
The Player Character can navigate with abilities such as sprinting, jumping, and double-jumping (after the character has acquired the skill). In addition, the character interacts with enemy collision unique to the direction in which the character collides with the enemy and the speed at which the character collides, resulting in unique movement options for the player depending on how they desire to navigate the map.

- character can dodge
Instead of implementing a dodge function, after reviewing how default movement was implemented in the Unreal Engine already, we decided that implementing a "shield" functionality would be more unique. Dodging would simply entailing adding to the velocity vector of the character, but in our implementation of a "shield" function, we change the character's battlestate such that they are invulnerable to being damaged for a time, but this invulnerability ends if the player character decides to attack or runs out of stamina.

- character can interact with objects
The character has particular interactions with colliding with items (a slight bump, no damage) versus colliding with enemies (strong velocity component aligned with the normal vector against the point and direction of collision). Also, the character can pick up items now with specific effects according to what type of item the character picks up.

- character can climb ladders / swim
We ultimately decided against implementing climbing and swimming due to the available amount of time versus the amount of animation assets we would have to create for these simple movement options, e.g. for implementing climbing alone would require new animation graphics for getting on a ladder, getting off a ladder, and climbing itself, that couldn't simply be derived from the animation assets we already have for the player character. We decided that we would rather spend this time on actually programming the logic of the game instead of working on assets, and so we removed climbing and swimming from the goal of the iteration.

##### Combat
- character experience is updated / level up and update stats
This works completely. An enemy death results in a player character receiving experience if they are the one who has dealt the killing blow. If their experience reaches a certain point, then their level increases, and along with that, their stats (attack power) increases at a fixed rate. Then, they have to kill more enemies (receive more experience) to get to the next level.

- character can attack with special abilities
We decided against implementing this for similar reasons as why we did not choose to implement climbing or swimming - the functionality and game logic would be extremely similar to the attack ability we already have implemented, but it would more assets for the ability animations.

##### Items
- power-up items
We've implemented a power-up item that boosts your attack power for a duration, and when it runs out, your attack power goes back to normal.

- healing items
There are two items that regenerate your health and stamina.

##### Pause game
- menu exists
Pressing P allows you to pause the menu and see various options. Note that the menu is created in Unreal Engine and so there are no test cases for the menu states.

- view character stats
We implemented a Player HUD that allows for continuous monitoring of your stats instead. We thought this would be a more useful stat viewing implementation than having to go from a pause menu to view your stats.

- equip and remove items
Instead of items you have to equip and use from a menu, we decided to have items that instantly affect the character when they are picked up. We decided this would fit the aesthetic and feel of the game better, as we shifted from RPG to more heavily a Platformer style game.

- save and load game
TBA

### (6) who did who: who paired with who; which part is implemented by which pair

`Note that there was a lot less strict pair-confined programming done in this iteration - people just fixed bugs and wrote code as needed.`

Items/Inventory
- Betty/Max
Battle States & Actions
- TJ/David
Movement/Base Classes
- John/Adam
Animations/Assets/Graphics
- Melinda/Steven

### (7) changes: have you made any design changes or unit test changes from earlier milestones?

Any changes from the use cases we delivered were explained on a per-use case basis in Question (5). In general, however, we made certain changes according to how much time we wanted to spend on making assets, and whether we thought certain use cases had functionalities that were similar to things we had already implemented, but simply required new animations.

From the list we produced for Milestone 4A, we implemented the following:
Death state
Killing enemies gives experience
Player experience and stats (Maximum Health, Attack Power, Experience, Level, etc)
Learned Skill(s) (Double jump)
Save/load game
Items (health, stamina, attack power boost upon pick up)

and did not implement the following:
Character navigation (swim, climb, interact with objects)

Explanations for why we chose to not implement what we did not implement can be found above.


# (8) others: whatever you want to let the TA know
