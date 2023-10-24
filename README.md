# Snake Game
*Mini C++ Project with Ncurses Library in Kookmin Univ.*

## Introduction
play snake game on your computer!  

### Views
- First Page

<img src="https://github.com/ytjdud/snake-game/assets/102743371/f1db8e5c-a89a-47d0-b79d-c0c22c208d2b"  height="400"/>

- Game Stage

<img src="https://github.com/ytjdud/snake-game/assets/102743371/6ed01c79-7483-4206-a7a0-c4af6e3259f0"  height="400"/>

- If you complete the mission on the stage, you can play next stage.
- There are four stages.
- If you fail the mission on the stage, the game's over.  
<div>
    <img src="https://github.com/ytjdud/snake-game/assets/102743371/845314e9-e5ae-46b7-9b3c-aa435d85e18a"  height="300"/>
    <img src="https://github.com/ytjdud/snake-game/assets/102743371/03e8d282-52a0-4a8a-9ed0-12e8572be90f"  height="300"/>
</div>  



### Stages
<div>
    <img src="https://github.com/ytjdud/snake-game/assets/102743371/11d2fb91-9a98-4285-8195-2fc6c20c5b80"  height="300"/>
    <img src="https://github.com/ytjdud/snake-game/assets/102743371/5473d7d4-c6de-4391-a548-927bf6d77c44"  height="300"/>
    <img src="https://github.com/ytjdud/snake-game/assets/102743371/74a30fae-9a24-4d94-a44d-a9c83351827a"  height="300"/>
    <img src="https://github.com/ytjdud/snake-game/assets/102743371/3cfd2b78-9b9f-408e-a3db-7158457baf45"  height="300"/>
</div>


### Rules
**Snake**  
(*head*) 🔘💠💠💠💠💠 (*tail*)
1. The snake **cannot** move in the direction of its tail.
2. Moving in the direction of the tail results in failure (game over).
3. The snake cannot pass through its own body.
4. The snake cannot pass through walls.

**Items**
- For Growth Items ✪ : Increase the length of the snake's tail by 1.
- For Poison Items ✖: Decrease the length of the snake's tail by 1. 
  - If the tail's length becomes less than 3, it's a failure.
- Items appear at random positions where there is no part of the snake's body.
- After a certain time (e.g., 5 seconds), items disappear and reappear at different locations.
- A maximum of 3 items can appear at the same time.

**Gate**  
When the snake enters a gate, it exits through another gate.

**Scoring**
- Calculate the maximum length of the snake during the game: (current length) / (maximum length).
- Count the number of Growth Items collected during the game.
- Count the number of Poison Items collected during the game.
- Count the number of times gates were used during the game.
- Calculate the game time (in seconds).



## How to Start the Game
open terminal and type in your terminal :
```
$ sh play.sh 
```
or
```
$ bash play 
```