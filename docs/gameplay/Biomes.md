# World and the Biomes
Ideas for different biome types and general world generation.

## Biomes
Biomes are defined by their humidity, elevation and temperature, from the generation process. Depending on these values, a specific biome type is chosen. 

- Humidity controls the wetness or dryness of the biome. For example, the `Desert` will have a humidity of `1` whilst the `Jungle` will have a humidity of `10`.

- Elevation controls the height of the biome. For example, the `Desert` will have an elevation of `1-3` but the `Mesa` will have an elevation of `7-10`.

- Temperature controls the biome variants. For example, the `Swamp` will have a temperature of `3` but the `Jungle` will have a temperature of `5`.

There are no seasons, so any cold biomes will always have snow on them. Likewise and hot biomes will always remain hot.

## Structures
Separation of natural and man-made structures. Natural structures generate as-is with no extra logic. Man-made structures generate based on "puzzle" pieces which have pre-defined acceptable arrangements.

A special block in the schematic file would be needed to tell the game when to dig-out existing terrain blocks.

## Ores
A selection of coal,copper,tin,iron,gold,diamond ores. Tools would progressively increase in strength/durability. Higher-end tools would be made from alloys, such as `diamond-infused-steel` for instance. These would have special workbench blocks and more in-depth mechanics.
