# Sprites guide
- This is a brief guide to show you how to use the sprites. Sprites have two components: 
    + An remapped image file that contains the sprites
    + A text file with the same name that contains the coordinates for each sprite 

- Each line in the text file corresponds to a sprite, from left to right, and from top to bottom, with the following format:

Sprite ID - X Position - Y Position - Width - Height

(X and Y position is anchored on the top left corner of the sprite)

- You can use the ID to keep track of the sprite you want to use, then target its position by letting `std::cin` read the anchor positions and adding to it the width and height to get the size of the rectangle bounding the sprite.

# Update 6/7
- I have reorganized a few spritesheets (sorry for the inconvenience) to remove duplicates and better align with the game's design:
    + Merge four icons sheets into one (remove all duplicates)
    + Merge two enemies sheets into one (bowser and small enemies): Bowser's sprites now go from index  1 - 20 (include fireballs at index 19 and 20), followed by other enemies.
    + Reorganize characters: merge luigi and mario sheets into 3 states: normal, star, fire. Position remains the same, only the index has changed.
        + The second column of normal mario/luigi sheets starts at line 22
        + The second column of star/fire mario/luigi sheets starts at line 36

- You can now load the Texture for the sprite you need by specifying the line (index) where the sprite's position is (Example: getEnemy(22)).
- For mario and luigi, provide an additional parameter for the state you are using:
    + n    :    normal
    + s    :    star
    + f    :    fire
    + e    :    electric (luigi only)
- For music and sound, specify the exact file name as it appears in the directory (Example: getMusic("boss")).
### The manager hasn't been tested extensively, so let me know if anything happens. Thanks a lot.

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

# Tiled JSON workflow
- Make the map in Tiled, assigning properties (block type, size, position, isSolid, spriteID) to each tile and icons; export the map to JSON. 
- Resource manager reads and loads textures upon app start-up. File handler imports the JSON file and make a vector of Block structs with properties.
- A Block factory calls both of the above classes. It then creates a new Block based on the properties of each block and assign it the right texture and push it into a vector.
- Map drawer then receives that vector and begin drawing the map based on position and size with a simple loop when called.
