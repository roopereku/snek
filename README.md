Snake written in c++.

-------------------------

Requires SDL2 and some c++11 compliant compiler to build.
To build, run make, or put the project in your favourite IDE.

-------------------------

Run  Snake -help  to get a list of valid arguments.
For example run  Snake -sensitivity 1 2.5  to set the sensitivity of player 1 to 2.5.
You can also edit the conf file to set arguments.

WARNING:
	Using arguments like -sensitivity 1 -player  will instantly crash the game,
	because currently it's assumed that the syntax is correct.

-------------------------

Currently the available keys are:
	WASD	-	For camera controls
	
    Left/Right  -\
    n/m         --\
	               >	Player controls (Will be customizable later)
    z/x         --/
    j/k         -/
