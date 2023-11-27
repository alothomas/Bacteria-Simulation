# Object-Oriented Programming Project (SSV)

## Group 90 README (THOMAS Aloïs)

*************************************************

This simulation of a Petri dish aims to model the life of bacteria over time in a Petri dish and observe which parameters of the bacteria allow them to better survive or die.

The Petri dish is composed of a collection of simulative bacteria with different properties (color, antibiotic resistance, speed, etc.).

In our project, we decided to implement four types of bacteria: SimpleBacterium, CannibalBacterium, TwitchingBacterium, SwarmBacterium. 
These bacteria are all modeled by:
- Energy
- Mutable parameters (antibiotic resistance)
- Mutable color
- Direction

To these common characteristics, other parameters specific to each type of bacteria are added (presence of a tentacle, toxicity to a type of nutrient, etc.).
Each type of bacteria evolves over time differently based on different rules. This has been modeled using different movement models, nutrient or bacteria consumption models.

The Petri dish also contains a collection of nutrients, whose nutritional effects differ depending on the type of bacteria, and a collection of antibiotics whose toxicity to bacteria is configurable.

During a simulation, all these elements interact. Bacteria lose energy by moving, being in the area of effect of an antibiotic, or if they are eaten by a cannibal bacterium. They can divide if they have enough energy. Similarly, nutrients decrease in quantity due to consumption by bacteria, and antibiotics see their dose decrease over time.

## COMPILATION AND EXECUTION:

This project uses cMake for compilation.
- A modifiable cMakeLists.txt file is available in /src to add other execution targets.

This application also has a config.hpp file where configuration elements can be added according to the JSON format.

## MAIN TARGETS:

- application: This is the final application.
- simpleBactTest: This is a test for simple bacteria.
- swarmTest: Test for bacteria with group behavior.
- twitchingTest: Test for bacteria with tentacles.

## CONFIGURATION FILE

A configuration file is available in /res in the form of an app.json. Each configurable element of the simulation is in this file. For example, the antibiotic has bounds for its quantity.

This file is available for any addition of configurable elements. Also, note the presence of an app.orig.json file that provides the initial configuration of the simulation without antibiotics and antibiotic resistance.

## COMMANDS

The commands for using the application according to the executed target are displayed in the simulation window.
Note that additional commands can be added in /res in a help file.

## DESIGN MODIFICATIONS

It is worth noting the addition of an Antibiotic class (key A) and a CannibalBacterium subclass of SimpleBacterium (key H). Additionally, the JSON has been modified with the addition of antibiotics and a factor of antibiotic resistance for each type of bacteria (mutable). The rest of the code follows the project statement recommendations.

## Observations

To observe antibiotic resistance on TwitchingBacterium:

In the configuration file:
- Set the time factor to 10.
- Set the antibiotic resistance of TwitchingBacterium as follows: initial: 10, rate: 0.9, sigma: 8 or 10.

In the application, set the temperature to over 30 degrees and wait for the creation of about a hundred nutrients. Then, pause, add 1 or 2 antibiotics in different locations, and add around 30 tentacle bacteria. Remove the pause and observe the antibiotic resistance curve. It should increase from around 10 to 15 or more.

General observation on TwitchingBacterium:
- Long-term tendency to decrease tentacle speed to 0 and tentacle length to 100 with the initial parameters.
- This effect can be countered by decreasing the sigma of the tentacle speed configuration.

