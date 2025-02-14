# particles

Implements particle demos

<img src="demo/rain.gif" width="400">

## How to build

*Windows*

Open git bash to the directory containing this repository.

```
particles $ mkdir build
particles $ cd build
particles/build $ cmake ..
particles/build $ start particles.sln
```

Your solution file should contain four projects.
To run from the git bash command shell, 

```
particles/build $ ../bin/Debug/billboard-axis.exe
particles/build $ ../bin/Debug/explosion.exe
particles/build $ ../bin/Debug/sparkle-trail.exe
particles/build $ ../bin/Debug/sparkles.exe
```

*macOS*

Open terminal to the directory containing this repository.

```
particles $ mkdir build
particles $ cd build
particles/build $ cmake ..
particles/build $ make
```

To run each program from build, you would type

```
particles/build $ ../bin/billboard-axis
particles/build $ ../bin/sparkles
particles/build $ ../bin/sparkle-trail
particles/build $ ../bin/explosion
```

## Demo of basic features

1. Trailing sparkles
<img src="demo/sparkle-trail.gif" width="400">
2. Explosion animation
<img src="demo/explosion.gif" width="400">
3. Axis Billboard
<img src="demo/billboard-axis.gif" width="400">

## Unique features 

1. Animated sprites - Explosion, flame, fireball
<div style="display:flex;">
    <img src="demo/explosion2.gif" width="200">
    <img src="demo/flame.gif" width="200">
</div>
<div style="display:flex;">
    <img src="demo/fireball.gif" width="200">
    <img src="demo/fireball2.gif" width="200">
</div>
2. Snow
<img src="demo/snow.gif" width="400">
3. Stars
<img src="demo/stars.gif" width="400">
4. Rain
<img src="demo/rain.gif" width="400">

