# Age of Towers (Multi-threaded Network Game)
Repository for the AGCP 3rd project.
<br>This project aims to implement a network based-game with multi-threading.

<br>Some parts of the code have been rushed for the university deadline. Different parts of the code are spread in different places but they should be part of the 2D engine to run the game things in top of it.
<br> I've achieved a mark of 100.0 for this project, so for the moment I'm not going to fix any of these problems.

<img src="https://trello-attachments.s3.amazonaws.com/5f747bf33d1087695fb61509/60aec584b0bd8064374540cb/afe9b41d773400c84543c6eb516e50e8/Captura_de_pantalla_2021-05-27_145710.jpg" width="850">

## What is this game and how I play it?
To play the game go to the 'bin' folder and execute the server, then execute both clients and the game will start.
<br>
<br>To build the Visual Studio project execute the 'compile_vs2019.bat' and it will create and open the solution if you have Visual Studio 2019 installed.
<br>
<br>To know more about the game idea, mechanics and controls look at the GDD + TDD .pdf in the 'doc' folder.

## Youtube demo video
https://youtu.be/NolwQ4UhtMk

## Dependencies
### External own code
All the network, multi-threading and game logic code has been done specifically for this university project.
<br>
<br>
Part of a 2D engine did on SDL in 2018 has been implemented to focus on the game and network aspects. Another AI project done in 2020 has also been added to manage all the units behaviour, also to not waste time in implementing these from scratch.
Both of these implemented projects were done by me as college modules' projects in ESAT Valencia.


### External code
- ImGUI for UI
- GLM for mathemathics
- SDL2 / SDL image / SDL ttf / SDL ImGUI

