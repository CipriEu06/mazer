# gemcu's maze solver (1.0 / 06.08.23)
SLD2 is REQUIRED!

in file "maze.txt":
- 1st line: size of tiles in pixels (scale);
- 2nd line: number of rows (n), number of columns (m) - max. 64; 
- next n lines: m symbols:
		- '0' for empty space
		- '-' for wall
- last line: start x coord, start y coord, finish x coord, finish y coord

- maze is bordered on the GUI.

after running "mazer.exe", file "sol.txt" will contain an n*m grid of symbols:
	- 'o' for empty space
	- '-' for wall
	- '*' for tiles that are part of the shortest path.
