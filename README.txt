Ripl README

1. What the application does?

The application is a game named "Ripl".  The player controls the movement of a
block, and attempts to reach the end of the damn in order to stop it
from draining.  In essence, they attempt to collide with the bank at the end
of the dam, and use their block to save the dam.

If the block collides with the bank edges, it becomes 'dead' and is
now an additional obstacle in the players way (they can collide with old
blocks).  If too many of the blocks don't make it to the end, then the dam
will drain of all water.  However, if they player is successful, the water
level will rise, indicating victory.

To control the block, the player physically tilts their tablet.  The block will
move in the direction of the tilt. This is all very intuitive.


2. How to use the application?

Load the project and solution onto a DirectX11 capable, Windows 8 tablet.
Open the project solution named 'RiplGame' in Visual Studio 2013, and run the
Ripl in 'Release' mode on the 'Local Machine'.

When Ripl loads, the player is presented with 'Start', 'Options' and 'Help',
the interaction from each button press is explained below:

Start 	- loads the player straight into the game, and they can start playing!
Options - allows the player to alter the difficulty level; the higher the
		  difficulty, the more blocks required to reach the end to win.
		- difficulty 1 = 6 blocks, 2 to end to win
		- difficulty 2 = 6 blocks, 4 to end to win
		- difficulty 3 = 8 blocks, 6 to end to win
Help	- explains the controls and gameplay to the player.

Additionally, the is a back button in the top left corner which will take the
player back to the Home screen.

Once starting the game, the player controls the block by tilting the tablet in
the direction which they want the block to travel.  The game works best if the
tablets' initial orientation is that similar to lying flat on a table face-up.

Once a block either reaches the end, or collides into the bank, the player view
is returned to the initial position, and provided with a new block.
Once the player has either successfully/unsuccessfully moved enough blocks to
the end of the dam, the water will change level.
Successful = water level rises
Unsuccessful = water level drops

To begin a new game, the player will need to manually exit the application,
either from pressing the Windows Home button, swiping to another application,
or something similar.

3. How you modelled objects and entities?

Terrain - created a bitmap image in paint, and used generated an algorithm to
		  convert this to a heightmap.
		- converted heightmap to vertices and indices, and then drew them.
Water   - made a triangle list, converted vertices and indices, and drew them.
Block   - made a function generating vertices/indices of a cube of given size.
		- made 1 block and stored it in the buffer and drew 1.
		- whenever an additional block was created, a copy was made of block 1.

4. How you handled graphics and camera motion?

Camera 	- we locked the position of the camera to a static location above the
		  current block, and made it follow the block wherever it moved.
		- once a new block was created, we moved the block back to the starting
		  location to begin again.

Graphics- loaded all models into vertices/indices list and loaded them into
		  buffers.
		- firstly, we render the static items every update tick (landscape and
		  water); and
		- second, we rendered the dynamic objects (each cube used so far, and
		  the current cube).


5. Statement about any code/APIs you have sourced/used from the internet that
is not your own.

Heightmap function: referenced www.braynzarsoft.net/index.php?p=D3D11HM for
	ideas of how to created a heightmap from bitmap, and then generated my own
	bitmap to heightmap function.

Accelerometer functions: referenced Microsoft MSDN site:
	http://msdn.microsoft.com/en-au/library/windows/apps/xaml/hh465276.aspx
	and used ideas to generate our tilt controls which move the block.

Collision detection: referenced Stackoverflow for ideas at:
	http://stackoverflow.com/questions/3232318/sphere-sphere-collision-detection-reaction
	came up with idea to use sphere collision by ourselves, looked for help
	in the implementation.

Camera movement: referenced Microsoft MSDN:
	http://msdn.microsoft.com/en-au/library/windows/apps/hh452773.aspx
	implemented ideas from the recommended Microsoft algorithm.

Lighting: referenced 3DGEP at:
	3dgep.com/texturing-lighting-directx-11/
	this was used for ideas on creating various forms of lighting,
	and preparing materials for reflections and ambient lighting.