## Overview

### Preamble

This document covers the vision of SG7 and the guiding principles of its development.

The Overview section describes the basic premise of the game and its story.

The Base Features section describes all features that are ideally present for an Alpha release.

The Features for Consideration section describes features which are outside the scope of the base game, but could improve the experience significantly and should be considered for addition once appropriate milestones have been reached.

This document is subject to change. Some sections may be incomplete, or require additional details.

### Concept

SG7 is a sci-fi themed, 2D overhead-view, action-adventure game with RPG-inspired character development elements.

### Setting

The game is set in the distant future. Humans has colonized Mars, as well as several bodies in outer solar system, including the Jovian moon Europa, but have not yet left the solar system.

The player begins his story in the Jovian sectors, specifically in Callisto Beta.

### Story Premise

In the years following the present date (2018), humans successfully colonized Mars, and soon after extended their reach even further, creating technologically marvelous colony installations on Jupiter's moon Europa. The colonies existed in harmony for several hundred years under the guidance of the TSF, Terran Security Force, but eventually, cultural and political rifts grew larger and culminated in the rebellion of the outer colonies. The formation of a new governmental agency, the Europan Authority, was created in the outbreak of the rebellion and laid claim to the Jovian sectors and surrounding territories.

A long and indecisive war was fought between the TSF and the Authority, which ultimately devolved into a stalemate. Eventually, the Europans unveiled their trump card, a massive laser superweapon harnessing the geothermal energy of the moon Io, capable of delivering blasts of energy so massive they could lay waste to an entire planet. In a bid to end the war, the superweapon was fired at one of Mars's moons, Deimos, annihilating the moon and turning it into a cloud of dust and rubble, which was sent tumbling down towards Mars as a meteor shower. Several colony installations were damaged, but casualties were limited. The Europans, however, had made their message clear: Listen to us, or pay the price. Both sides quickly negotiated a peace treaty and ended the war, with the Europans enforcing their primary demands: The Jovian sectors would remain under their sovereign control.

In the years following the war, both the TSF and Authority continue to refuse diplomatic relations. Strict border controls were introduced, with limited trade flowing between the two empires. Tensions, however, remain high, as the TSF scrambles to advance technologically, both to defend against and compete with the Europans.

The game follows the story of the nameless player character, recently enlisted into the TSF's Elite Pilot program. As they struggle with amnesia from a strange medical procedure, they must refamiliarize themselves with the rocky political climate, as well as cope with the harsh reality of their mission, which they may very well begin to question...

## Base Features

### States and State Transitions

The two basic and essential states are Spaceflight and Base-Landed.

In the Spaceflight state, players control their ship directly and may use abilities. They can explore at their own pace in a non-linear fashion.

- Throttle (forward+back) controls
- Rotation controls
- Fire weapons
- Fire missiles
- Deploy mines
- Sector map
- Use learned abilities
- Interact with objects

Physics should be modeled as realistically as possible. Throttle should apply a certain amount of force, propelling the ship forward, adjusted negatively in proportion to the ship's current speed relative to its current course in order to prevent the player from attaining ludicrous velocities taking them far off the map.

In the Base-Landed state, players have access to the facilities of the station on which they are landed. This should include:

- Equipment dealer, where the player can purchase new equipment and outfit their ship
- Junk dealer, where the player can trade in scrap metal
- Crafting station, where the player can use their materials (obtained by mining) to craft upgrades for their ship.
- Mission board, where the player can sign up for missions, which give a monetary payout upon completion
- Upgrade station, where the player can mount, destroy or sell upgrades they've crafted.
- Ship dealer, where the player can purchase new ships.

In both states, the player has access to a number of interfaces to view their character's data and track their progress.

- Character sheet, showing level and experience
- Cargo hold, showing owned equipment
- Material bank, showing materials
- Ability sheet, showing learned abilities
- Skill tree, showing the skill tree and allowing the player to invest skill points to obtain new skills

To land on a station, players interact with a Docking Buoy. This invokes a transition from Spaceflight to Base-Landed. Similarly, players leave the station and re-enter Spaceflight by clicking a button in the station interface.

Travel between sectors can be done with the help of a Jump Buoy.

### Combat

Combat in Spaceflight is largely centered around maneuvering and using the ship's weapons to bring the enemy's HP to zero. Every ship has three pools, Health, Shields, and Energy. Health does not regenerate, while Shields regenerates passively. Damage to the ship hits the Shield pool first, and after Shields reaches zero, Health receives the damage instead. After a time, Shields will be restored to a certain percent value, depending on the particular shield generator the ship has. Energy represents the ship's weapons capacitors, and is used by weapons. When the capacitors reach zero, the player can no longer fire energy-based weapons and must wait for the capacitor to recharge.

Additionally, there is a concept called Elite Pilots. In the lore of the game, Elite Pilots are humans who have received medical implants giving them a direct conduit into the ship's systems. As such, they are essentially "super pilots" and gain access to a range of skills and abilities. For the player, this is modeled as the skill tree. Abilities, such as barrages and heals, are learned by investing skill points into the skill tree, and are intended to make combat more dynamic and interesting as the game progresses. Additionally, the player can meet "boss" NPCs which are also elite pilots, capable of using abilities just like the player. Abilities use a fourth pool called Mind.

Status effects, which can be applied to both players and NPCs, are used to model different effects on a ship. For example, a status effect called Hull Breach might cause a damage-over-time effect, applying damage directly to Hull, bypassing Shields. These status effects can be inflicted by various abilities, or randomly by certain combat situations.

The main source of progression in the game is destroying other ships, which grants XP. Additionally, destroying ships has a chance to drop loot, such as equipment, depending on the NPC killed. The player also receives a random amount of Scrap Metal, which they can sell aboard stations, giving the player an indirect cash bonus for every NPC killed.

### Scripts and Missions

To facilitate writing missions, and eventually a main story campaign, the game includes a scripting engine. The script engine should provide a wide range of functions to this end.

- Manipulate the player character, give them XP, money, etc
- Instantly land on a base, or transition to another sector
- Querying properties of a ship, for example, health or position
- Spawning new NPCs
- Giving instructions to NPC AI, for example, goto a position, engage the player
- Random number generation
- Persistent variables, enabling "memory" of prior events
- Set and remove mission waypoints
- Send comm messages to the player, or initiate dialogue sequences
- Play sound effects
- Display graphical animations and effects

In additions, sectors are modeled as scripts.

- Draw a sector map which can be viewed while in space
- Create docking buoys
- Create jump buoys
- Create static background objects


### Leveling, experience, and skills

With each enemy destroyed in combat, the player gains experience. With experience, the player will gain levels. Each level gained grants an additional skill point, which can be invested into a skill in the skill tree. Skills can grant several types of bonuses:

- Stats, which improve certain aspects of the player's effectiveness, for example, a stat called Laser Proficiency could improve damage dealt by lasers
- Abilities, which can be used while in space to accomplish various things, for example, applying status effects to enemy ships, dealing instant damage, healing the user, etc.
- Crafting recipes, which could be restricted to a certain section of the skill tree called for example Engineering, allowing the player to craft new upgrades.

### Asteroid Fields and Asteroid Mining

During their exploration, the player may encounter Asteroid Fields. These contain a number of randomly placed asteroids which can be mined by the player, assuming the player has a special piece of equipment (Mining Laser).

Mining an asteroid gives the player a number of a particular Resource. Resources come in several different types, broken down by Category (Copper, Iron, Methane) which themselves are subsets of a Type (Metallic, Inorganic). Each Material possesses a range of attributes, which are decided randomly upon asteroid field generation.

Attributes model different properties of the material, for example, Malleability or Electrical Conductivity. These attributes are used during the crafting process to determine the quality of the final product.

### Upgrades and Crafting

Using materials obtained from asteroid mining and potentially other sources, the player can craft upgrades for their ship. These upgrades provide substantial bonuses, for example, an upgrade called Capacitor Supercharger Mk I might boost weapons capacitor regeneration rate by 50%, and maximum capacitor capacity by 50%.

Each crafting recipe might require several different material categories of varying amounts. Different attributes of the material can affect different values of the final product. For example, the aforementioned Capacitor Supercharger Mk I upgrade might have its regen rate output connected to the materials' Electrical Conductivity attribute, while its capacity output is connected to Integrity. Thus, if the player uses materials with low Electrical Conductivity, but high Integrity, the resulting capacitor supercharger might only have a regen rate boost of 15%, but with a capacity boost of 48%.

This makes resources with better attributes much more valuable, and will encourage players to explore to find asteroid fields with higher-quality resources. On the other hand, players who don't wish to invest significant time into crafting may still make a compromise, still being able to craft basic upgrades which suit their particular needs.

## Features for Consideration

### Base Interiors

Instead of providing a simple UI interface in the Base-Landed state, the game could provide a new mode with a 2D overhead view, where the player controls their character sprite. They may move around the station interior, go to different locations aboard the station including equipment dealer, junk trader, bar, etc., and speak directly with other characters. This would also enable the possibility for station boarding and combat aboard stations.

Controls while in Base-Landed:

- WASD to move quad-directionally
- Can move between rooms which represent different parts of the station, eg equipment dealer, bar, etc.
- Can interact with NPCs who have different jobs aboard the station, talk with patrons in the bar
- View character sheet, inventory, all interfaces that are available while in space

### Dialogue

In the base game, players are exposed to most of the game's lore in the form of base and sector descriptions and comm messages during missions. A system for interactive dialogue would better enable the player to learn the game's universe, as much as they please and at their own pace. In addition, it would allow for greater character depth of NPCs, and give more "weight" to the story narrative.

The player initiates dialogue by pressing the interact key (T) while another ship is selected. This pauses the game, while dialogue occurs between the two ships. The player receives an opening message from the other ship, after which the player is given the opportunity to select from several predefined responses. Each option generates a different response from the NPC, and makes a new set of responses available to choose from. This is continued until dialogue ends.

Certain dialogue options may set certain persistent variables. As these variables can also be accessed by scripts, dialogue options could potentially have effects on the story, or even the direction the story takes.

### Dynamic Economy

In the base game, there is only one commodity that can be traded, Scrap Metal at the Junk Trader aboard stations. However, the ability to purchase different commodities aboard stations, each with its own price range, would make the game's economy much more interesting.

Commodities would represent goods not particularly useful to a starship engineer (hence, it's not a material) and not particularly useful to a pilot (it's not a piece of equipment). Some examples of commodities could be Small Arms, CO2 Scrubbers, Scrap Metal, or Consumer Goods.

The Junk Trader interface would be replaced by a Commodity Trader while stationside. Upon visiting the commodity trader on a station, the player has the opportunity to purchase any commodity for sale on that station, as well as sell commodities they have in their cargo hold. Each station tracks its own price for a particular commodity. That price is impacted directly by sell actions and buy actions made by the player, and is subject to random fluctuations.

A dynamic economy could enable the player to take on a trader/space trucker role, in addition to the combat/dogfighting and exploration roles present in the base game. Additionally, it would enable more variety in the Spacefaring branch of the skill tree.