# Sprites guide
- This is a brief guide to show you how to use the sprites. Sprites have two components: 
    + An remapped image file that contains the sprites
    + A text file with the same name that contains the coordinates for each sprite 

- Each line in the text file corresponds to a sprite, from left to right, and from top to bottom, with the following format:

Sprite ID - X Position - Y Position - Width - Height

(X and Y position is anchored on the top left corner of the sprite)

- You can use the ID to keep track of the sprite you want to use, then target its position by letting `std::cin` read the anchor positions and adding to it the width and height to get the size of the rectangle bounding the sprite.

# How I think map data can be saved
## Reorganizing sprite sheets
- I intend to keep only the portions of the icon and tile sets that are relevant to the scope of the game.
- This means keeping 4-5 types of block and basic structures like water pipes or question blocks and star icons and such. 
- After reducing the number of icons and tiles, I would make some hash maps that gives each enemy/tile/icon an ID as key and its position in the sprite sheet as value (currently working on this). 

## Storing map
- If the map size is going to be fixed we can make a 2D array that stores IDs of the blocks relative to their position in the map.
- For example, a dirt block at (160, 300) will have ID 'd' will be at `tilesMap[15][29]` (assuming each block is 10x10).
- There should be 2 hash maps, one to store objects and the other to store enemies (different IDs assignment conventions). 
- File handling simply stores and loads the array of IDs to and from a bin file and the game editor can read the IDs to know what type of block to put in what position in the map. 

## Edit map
- Placing the blocks means getting mouse position and divide coordinates by the size of each block to get the array index, at which we can assign the IDs.
- Similarly, we can remove the blocks placed using the same logic.
- For objects with sizes larger than 1 block, each block it occupies is filled with its IDs 
- For example, a water pipe with size 40x50 pixels means the array filling 20 cells with ID 'w'(assuming each block is 10x10).
- This means collision handling can be checked by checking if the cell about to be filled has already had any ID in it (so that the pipe cannot be placed into the ground for instance). 

## Save map
- Game editor will simply pass two arrays containing the position of blocks and enemies to the file handler which then saves it into a bin file.
- File handler will also have functions to convert the binary data back to usable array and give it back to the game editor.