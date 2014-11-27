#Smooth Dirt | A CS220 Project | Milestone 4B

### (1) how to compile
Prior to compiling, you need to download Unreal Engine 4.5.1.  You can get it for free through github or pay $20 for a personal license.  Once you have UE4 installed you can continue with these instructions.

To compile and run the game (in-editor) follow these steps:

1) Right-Click on the `Platformer.uproject` file and select `generate [XCode/Visual Studio] project`

2) Open the generated project in [XCode/Visual Studio]

3) Change the target to the Game Editor option and compile 

  3a) you can ignore the asset missing message if it appears
  
4) Once the editor opens, you can play the game by pressing `Play`

### (2) how to run your code

Once in the editor, you can hit the `Play` button to play  the game from the viewport.

Alternatively, you can run the appropriate executable from the `Packages` diectory.

### (3) how to run the unit test cases
Unit testing is only setup for Mac in XCode, but uses GTEST so you should be able to run them on windows as well.  For unit testing, we copied out the code we wanted to test, so that we didn't have to worry about conflicting with the unreal engine code.

1) In Xcode, create a new target `Command Line Program` called `Unit Tests`

2) Add all the code in the `UnitTests` directory to the project

3) Make sure those files are only included in the `Unit Tests` target

4) Edit the `UnitTests` scheme 

  4a) Under Run->Environment Variables add
      `DYLD_FRAMEWORK_PATH` with the value being the absolute path to the `gtest.framework` stored in the `UnitTests`
      
5) Make sure that the `main.cpp` looks like the one here:

```
int main(int argc, char** argv)
{
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
```

6) Now you can build the `UnitTests` target

###(4) please suggest some acceptance tests for the TA to try (i.e., what inputs to use, and what outputs are expected)

* Left & Right Arrow Keys = move left and right
* Up Arrow & Space = Jump
* D Key = attack
* S Key = Sprint
* Left Shift = Shield
* P Key = enter pause menu

* Killing an Enemy => Character gains experience
* Gaining an amount of experience => Character gains a level
* Character health hits 0 => Character enters death state

You will be able to play the game, kill enemies, pick up HP potions, pick up stamina potions, pick up attack buffs for a specified duration, level up, gain experience, gain attack damage, kill enemies, jump on platforms, die to enemies, be damaged by an enemy, deal damage to an enemy, pause the game, save the game, load the game, exit the game, overwrite the save file, kill the boss, and win the game.

### (5) text description of what is implemented. You can refer to the use cases and user stories in your design document.

**For our description of what is implemented, we will walk through each use case we planned for Iteration 2 from our original design document (from milestone 2).**

##### Character Navigation
- **character can navigate with abilities**  
The Player Character can navigate with abilities such as sprinting, jumping, and multi-jumping (after the character has acquired the skill). In addition, the character interacts with enemy collision unique to the direction in which the character collides with the enemy and the speed at which the character collides, resulting in unique movement options for the player depending on how they desire to navigate the map.

- **character can dodge**  
Instead of implementing a dodge function, after reviewing how default movement was implemented in the Unreal Engine already, we decided that implementing a "shield" functionality would be more unique. Dodging would simply entailing adding to the velocity vector of the character, but in our implementation of a "shield" function, we change the character's battlestate such that they are invulnerable to being damaged for a time, but this invulnerability ends if the player character decides to attack or runs out of stamina.

- **character can interact with objects**  
The character has particular interactions with colliding with items (a slight bump, no damage) versus colliding with enemies (strong velocity component aligned with the normal vector against the point and direction of collision). Also, the character can pick up items now with specific effects according to what type of item the character picks up.

- **character can climb ladders / swim**  
We ultimately decided against implementing climbing and swimming due to the available amount of time versus the amount of animation assets we would have to create for these simple movement options, e.g. for implementing climbing alone would require new animation graphics for getting on a ladder, getting off a ladder, and climbing itself, that couldn't simply be derived from the animation assets we already have for the player character. We decided to prioritize programming the logic of the game instead of working on assets, so we removed climbing and swimming from the goal of the iteration.

##### Combat
- **character experience is updated / level up and update stats**  
Combat works as planned. An enemy death results in a player character receiving experience if they are the one who has dealt the killing blow. If their experience reaches a certain point, then their level increases, and along with that, their stats (attack power) increases at a fixed rate. Then, they have to kill more enemies and receive more experience to get to the next level.

- **character can attack with special abilities**  
We decided against implementing this for similar reasons as why we did not choose to implement climbing or swimming - the functionality and game logic would be extremely similar to the attack ability we already have implemented, but it would require more assets for the ability animations.

- **player death**  
When the player's health is depleted, the player enters a death state, in which the player is unable to perform any actions in the game. The player can then access the pause menu and select the option to start a new game, continue from the last save checkpoint, or exit the game.

##### Items
- **power-up items**  
We've implemented a power-up item that boosts your attack power for a duration, and when it runs out, your attack power returns to normal.

- **healing items**  
There are two items can regenerate either your health or stamina upon pick-up.

##### Pause game
- **menu exists**  
Pressing P allows you to pause the menu and choose from various options. Note that the menu is created in Unreal Engine and so there are no test cases for the menu states.

- **view character stats**  
We implemented a Player HUD that allows for continuous monitoring of your statistics instead. We thought this would be a more useful statistics-viewing implementation than having to go from a pause menu to view your statistics.

- **equip and remove items**  a
Instead of items you have to equip and use from a menu, we decided to have items that instantly affect the character when they are picked up. We decided this would fit the aesthetic and feel of the game better, as we shifted from RPG to a more platformer-style game.

- **save and load game**  
Opening the game executable will start a new game by default. The game will automatically save the player's information (level, experience, etc) and location every time the player reaches the next level as a checkpoint. Only one save file is stored at a time and the player can load this save file by accessing the menu and selecting the "Load Last Save" option. The player may also restart the game by selecting "New Game, which will transport the player to the starting position and replace their information with the basic starting information.

### (6) who did who: who paired with who; which part is implemented by which pair

**Note that there was a lot less strict pair-confined programming done in this iteration - people just fixed bugs and wrote code as needed.**

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
- Death state
- Killing enemies gives experience
- Player experience and stats (Maximum Health, Attack Power, Experience, Level, etc)
- Learned Skill(s) (Double jump)
- Save/load game
- Items (health, stamina, attack power boost upon pick up)

and did not implement the following:  
- Character navigation (swim, climb, interact with objects)

Explanations for why we chose to not implement what we did not implement can be found above.

### (8) others: whatever you want to let the TA know
