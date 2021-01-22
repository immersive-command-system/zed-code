# zed-code
ZED Stereo Camera Code

This code is based on the ZED C++ SDK and does not use ROS, which is why its in a different repo than ImmersiveInterface2.
To integrate it into ISAACS the ZED ROS wrapper will need to be used. 

3D Mapping Planning Doc: https://docs.google.com/document/d/1UF9w5l6qaanxyj5wSWAWRYyflIOXF0gGdvBvm6EOkEA/edit?usp=sharing


## zed


1. `zed/Spatial Mapping/src/main.cpp` initiates ZED's mapping module to process an incoming camera stream over a local network and perform 3D reconstruction, with live visualization. Run by compiling, and then in the build folder run `./SpatialMapping <host_ip> <port>`

	Compile on Linux by:

    - mkdir build
    - cd build
    - cmake ..
    - make

    Compile on Windows by:
    - Create a "build" folder in the source folder
	- Open cmake-gui and select the source and build folders
	- Generate the Visual Studio `Win64` solution
	- Open the resulting solution and change configuration to `Release`
	- Build solution


## local network streaming

1. `streaming/Streaming_Sender/src/main.cpp` sends ZED 2 camera stream over a local network. Run by compiling it, and then in the build folder run `./ZED_Streaming_Sender <port>`

Compile on Linux by:

    - mkdir build
    - cd build
    - cmake ..
    - make

