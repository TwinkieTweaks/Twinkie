# Twinkie
Twinkie is a QOL modification for TrackMania Forever.

## Special thanks
To [GreffMASTER](https://github.com/GreffMASTER) and [brokenphilip](https://github.com/brokenphilip) for helping me during development. brokenphilip has his own [tool for TMUF](https://github.com/BulbToys/TMUF) and GreffMASTER [as well (for TM1.0)](https://github.com/GreffMASTER/TMStuff) if you're interested in more cool mods for old TM games.
GreffMASTER also provided me with a helpful list of class members and their offsets for TMF <3

## Features
- Ghost Editor, allows you to access the ghost editor without the cheat code
- Dashboard, an input display and **NEW!** gear display with customizable options
- **NEW!** Checkpoint counter, does what it says right on the tin
- Medals window, for easily viewing medals

## Limitations
- Only works if the game is *launched* in windowed mode, putting it in fullscreen after works
- For some people, the F3 keybind to close the UI bar doesn't work
- Only works with Vanilla Standalone TMF

## Screenshots
![In stadium (debug build)](https://github.com/user-attachments/assets/59a81970-0b01-4d9d-a4c2-f97216e42e24)
![In bay](https://github.com/user-attachments/assets/d37c048b-22d5-4495-8617-54776fb1d462)

## Create separate builds for different TMF variants
![image](https://github.com/user-attachments/assets/25da4bde-8146-4740-94d6-d7e75660a87a)
The visual studio solution already comes with various build options for some TMF variants. Note that this only renames the DLL and puts it in the corresponding directory.
To actually make the build function differently for different games, then head over to the `Twink.h` file where you will find some commented `#define` statements, uncomment however you like to make the build you want:
- `#define BUILD_DEBUG`: build with debugging features like the PlayerInformation module
- `#define BUILD_{GAME}`: build for a specific TMF variant
- `#define BUILD_EXTERNAL_CONSOLE`: make Twinkie create an external console upon hooking to the game
