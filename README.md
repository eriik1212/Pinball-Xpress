# Pinball-Xpress

## TEAM MEMBERS:

  * David Boces 
  * Erik Martín
  * Pau Olmos
 
## CONTROLS:

  * Left arrow: Move the left flipper
  * Right arrow: Move the right flipper
  * Down Arrow: Move down the spring to apply force to the ball at the start of the game
  
## DEBUG FEATURES:

  * F1: Show hitboxes and activate/disable debug mode
  * 1: Create a ball al the start position
  * 2: Create a ball at the mouse position
  * Left mouse click: If pressed at the ball, create a Joint and move it arround
  
## GAME DESCRIPTION:
  
  Shoot the ball with a cannon and move your flippers in this western set pinball to try to get the maximum score hitting the greater number of objects as possible.
  
  ### SPECIAL COMBOS
  Try to hit all coloured triangles, it would reward you some way... Oh, the three circles above would have something to say too.
 
_<Hit the 3 coloured triangles and get 10.000 extra points!>_
 
_<Hit the 3 circles above the triangles and get 1 extra ball (plus 15.000 points!)>_

## DEVELOPMENT PROCESS
This Pinball works in Real Time! We added this feature in order to avoid _Super-Speedy Gameplays_. To do so, we used a **fixed delta time** _(16 ms)_ + **time wait** _(dt - timeElapsed)_. With this method, we calculate the milliseconds that take all our program and make it sleep until 16 ms pass.

We made a _Spring Launcher_ for de ball with a **Prismatic Joint**. We added upper and lower limits and enabled a motor force in order to make it look and work like a spring. You just have to retain _Down Arrow_ in your keyboard to prepare it for launching and lose it when you feel appropiate to let the action begin!

To perform two functional _flippers_ we used **Revolute Joint**. We attached a ball and a rectangle (static ball and kinetic rectangle) in order to reproduce a realistic _flipper_ shape and movement. To reach a good and realistic behaviour we added rotation limits to the joint and two vectorial forces to make the flipper go up according to the player input and get to the initial position again.

With this joint we had some tunneling issues even using a bullet type pinball ball, but we solved it easily by increasing the _world.step()_ function iterations in order to develop a bit more precise pinball.

The last joint we made was a **Mouse Joint**. This joint helped us a lot on the debugging features, making able to move the balls around the map using the mouse and the left click. 

Making the combos was easy. The program check over _OnCollision()_ function (you will find it in _ModulePlayer.h_ and _ModulePlayer.cpp_) and look over whether the ball and the combo's elements collided or not, and if so, a _bool_ variable would be activated to "save" that collision. If 3 determinated _bools_ were **true**, it would activate de combo and reset all these _bools_.

Also, we incorporated a sensor to seal the upwards path were the ball arrives to the map once it passed this path. That seal was made in order to avoid the ball getting back on the start launching cannon.

Durning the addition of the loose condition we had a couple of problems related to the score and lives print system, so to save time, we applied a FUYM so the final product looked great and was functional. 

Pressing the space while being on the loose screen will save your score, your highscore, and reset all the points and lives, however, closing the program, will erase all the information of the previous games. 

## LINK TO OUR GITHUB:

  https://github.com/eriik1212/Pinball-Xpress
