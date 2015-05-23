Granular OSC
============

An OSC control interface to control a granular synthesizer. 

Tested using [Control](http://charlie-roberts.com/Control/) app's MultiTouchXY interface. Each touch point controls one of the five parameters of the granular synth. 

1. Grain rate
2. Pan position (left-right)
3. Grain size
4. Grain init frequency
5. Maximum number of grains

Build
======
For an out of source build, open a terminal and navigate to the source directory and run the following commands:

    mkdir build
    cd build
    cmake ../
    make

To run, run the file `hw2` in the build directory. 

-------

Dependencies:
* `PortAudio`
* `liblo`
* `cmake`

------
Author: Akshay Cadambi (akshay@mat.ucsb.edu)
