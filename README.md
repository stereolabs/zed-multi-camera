# zed-multi-input

**This sample is designed to work with the ZED stereo camera only and requires the ZED SDK. For more information: https://www.stereolabs.com**

It demonstrates how to use multiple ZEDs into a single program.
Each grab is done in a separate thread.

- Tested with 2 ZEDs in HD1080 @ 30fps (ASUS Sabertooth Z87 motherboard and GTX770)
- Tested with 3 ZEDs in HD720 @ 30fps
- Tested with 4 ZEDs in VGA @ 30fps

**Warning :**
 - **This sample only works on Linux**
 - A middle-range NVIDIA GPU card is needed with enough memory (around 2GB).
 - If the frames appeared corrupted (green/purple, tearing) it means that the USB 3.0 controller is not fast enough to handle multiples ZEDs or the framerate chosen is too high

This sample needs OpenCV for image display and basic processing.

##Build the program

Open a terminal and execute the following command:

    $ mkdir build
    $ cd build
    $ cmake ..
    $ make

##Run the program

Open a terminal in build directory and execute the following command:

    $ ./ZED\ Multi\ Input
