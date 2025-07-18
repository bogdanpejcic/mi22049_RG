# {NAME-OF-THE-PROJECT}

49/2022 - Bogdan Pejčić
Airplane is flying in daytime conditions. After pressing N on the keyboard,
the scene changes to night, along with the lighting, which switches from directional to spot.
After pressing the M key, the camera turns to face the model and travels over 3 seconds to
a position in front of it, maintaining its focus on the model throughout the journey. During this movement,
other camera controls and GUI interactions are disabled.
If you want to use your cursor (e.g., to enter full screen), open the GUI.

## Controls

F2 -> Opens GUI
N -> Changes scene from day to night and changes lighting
M -> Returns camera to starting position in 2 seconds

W -> Camera moves forward  
A -> Camera moves left
S -> Camera moves backword
D -> Camera moves right

KEY_UP -> Camera turns up
KEY_DOWN -> Camera turns down
KEY_LEFT -> Camera turns left
KEY_RIGHT -> Camera turns right

ESC -> Terminate
...  
{BUTTONN} -> {ACTIONN}

## Features

### Fundamental:

[ YES ] Model with lighting
[ YES ] Two types of lighting with customizable colors and movement through GUI or ACTIONS
[ YES ] {ACTION_X} --- AFTER_M_SECONDS---Triggers---> {EVENT_A} ---> AFTER_N_SECONDS---Triggers---> {EVENT_B}
{Press M} ---> Triggers---> {Camera looks at the jet} ---> AFTER_3_SECONDS----> {Camera at starting position}

### Group A:

[ ] Frame-buffers with post-processing   
[ ] Instancing  
[ ] Off-screen Anti-Aliasing  
[ ] Parallax Mapping

### Group B:

[ ] Bloom with the use of HDR  
[ ] Deferred Shading  
[ ] Point Shadows  
[ ] SSAO

### Engine improvement:

[ ] ...

## Models:

[link1]   
[link2]

## Textures:

[link1]  
[link2]

## Skyboxes:

https://opengameart.org/content/cloudy-skyboxes-0

## Fonts:

https://www.jetbrains.com/lp/mono

{OTHER-RESOURCES}
