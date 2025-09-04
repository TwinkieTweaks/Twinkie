# Twinkie
[![GitHub release (latest by date including pre-releases)](https://img.shields.io/github/v/release/TwinkieTweaks/Twinkie?include_prereleases&color=f132e3)](https://github.com/TwinkieTweaks/Twinkie/releases)
[![GitHub all releases](https://img.shields.io/github/downloads/TwinkieTweaks/Twinkie/total?color=f132e3)](https://github.com/TwinkieTweaks/Twinkie/releases)
[![GitHub contributors](https://img.shields.io/github/contributors/TwinkieTweaks/Twinkie?color=f132e3)](https://github.com/TwinkieTweaks/Twinkie/graphs/contributors)\
Twinkie is a QOL modification for TrackMania Forever, a practical alternative to Openplanet, but for TMNF and TMUF.\
It features a modular (or plugin-like) system for overlays and patches, that makes it practical to customize every feature available.

# Setup
To use Twinkie, you will need to install the [Trackmania Modloader](https://tomashu.dev/software/tmloader), and after that, go grab the latest release from [the Releases page](https://github.com/TwinkieTweaks/Twinkie/releases), and install Twinkie using the `TwinkieInstallerModloader.exe` file. Alternatively, if you're using a portable Modloader install, you can go grab the `TwinkieModloader.dll` as well, for a manual setup.

## Discord
We have a discord server at https://discord.gg/kRZ4MdCkVf

## Special thanks
To [GreffMASTER](https://github.com/GreffMASTER) and [brokenphilip](https://github.com/brokenphilip) for helping me during development. brokenphilip has his own [tool for TMUF](https://github.com/BulbToys/TMUF) and GreffMASTER [as well (for TM1.0)](https://github.com/GreffMASTER/TMStuff) if you're interested in more cool mods for old TM games.\
GreffMASTER also provided me with a helpful list of class members and their offsets for TMF, alongside brokenphilip helping me along my RE journey for this project <3

## Limitations
- Medal.tv requires a work-around to function properly (see below)
- Hiding the in-game HUD might also hide the Twinkie UI or make it look weird. To fix this, try changing your shader quality to something different.

## Medal.tv
Due to Medal's uncooperativeness, you might have to do these following steps to make Twinkie work alongside it: 
- Make sure your Medal app is up to date first. 
- Then, in Settings > Advanced and in the "Advanced | Recording section, turn on "Force Window Capture". 
Keep in mind that **this is an experimental setting**, **it might not work at all** or **it might make your clips stuttery** but it is worth trying.

## Features
- Ghost Editor, allows you to access the ghost editor without the cheat code.
- Dashboard, an input display and gear display (tachometer, and gear number indicator) with customizable options
- Checkpoint counter, does what it says right on the tin
- Medals window, for easily viewing medals
- Tweaker, for customizing your render distance (even though [TMUnlimiter already does that](https://github.com/tomek0055/TMUnlimiter/wiki/Game-Settings))
- **Grinding Stats**, a module that keeps track of your progress for each map
- **Always official**, that brings up the Official Mode menu when you reset (only for United accounts)
- ~~**Nickname patch**~~ _**DO NOT USE THIS! This module causes interference with server databases, costing server owners a lot of time patching it.**_

## Screenshots
![In stadium, playing solo on a Premium build](https://github.com/user-attachments/assets/54d06017-bbf3-48ee-b8e1-a07288d3d672)
![In coast/bay envimix, playing multiplayer](https://github.com/user-attachments/assets/5391ee54-9d2f-40a6-9c26-218cd95e2bf1)

## Contribute
To know how to make new modules, check out [`CreatingModules.md`](https://github.com/TwinkieTweaks/Twinkie/blob/master/CreatingModules.md)

## Create separate builds for different TMF variants
The visual studio solution already comes with various build options for some TMF variants.\
![image](https://github.com/user-attachments/assets/25da4bde-8146-4740-94d6-d7e75660a87a)\
Building in x64 is **not supported** since TMF is 32-bit.\
If you're having compilation issues, make sure you're **ONLY** compiling to x86/Win32.\
Building for United and Nations is not currently supported. Only Modloader/Premium builds are.

## Support me
There currently aren't any monetary ways of supporting me, so if you like Twinkie and you have a github account, please star it so that more people see it, or be kind enough to share it with your friends.

## Fonts
Twinkie uses Cascadia Mono (see its license [here](https://fonts.google.com/specimen/Cascadia+Mono/license) (Google Fonts), and [here](https://openfontlicense.org/open-font-license-official-text/) (openfontlicense.org)), and [ManiaIcons](https://github.com/openplanet-nl/maniaicons) by the Openplanet Team
