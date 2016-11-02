# Stereolabs ZED - Using multiple ZED

This sample shows how to use multiple ZED cameras in a single application. Currently available on Linux only.

## Getting started

- First, download the latest version of the ZED SDK on [stereolabs.com](https://www.stereolabs.com).
- For more information, read the ZED [API documentation](https://www.stereolabs.com/developers/documentation/API/).

### Prerequisites

- Ubuntu 16.04
- [ZED SDK](https://www.stereolabs.com/developers/) and its dependencies ([CUDA](https://developer.nvidia.com/cuda-downloads),[OpenCV 3.1](http://opencv.org/downloads.html))


## Build the program

Download the sample. In the sample directory, open a terminal and execute the following command:

    mkdir build
    cd build
    cmake ..
    make

## Run the program

Open a terminal in the 'build' directory and execute the following command:

    ./ZED\ Multi\ Input

## How it works

- Video capture for each camera is done in a separate thread for optimal performance. You can specify the number of ZED used by changing the [`NUM_CAMERAS`](https://github.com/stereolabs/zed-multi-input/blob/master/src/main.cpp#L43) parameter.
- Each camera has its own timestamp (uncomment [this line](https://github.com/stereolabs/zed-multi-input/blob/master/src/main.cpp#L122) to display it). These timestamps can be used for device synchronization.
- OpenCV is used to display the images and depth maps. To stop the application, simply press 'q'.


### Limitations

- This sample works on Linux only.
- You may need to adjust your ZED cameras framerate, resolution and depth quality depending on your setup. As a reference, here are some configurations we tested internally:
  - Tested with 2 ZEDs in HD1080 @ 30fps (Asus Sabertooth Z87 motherboard and GTX770)
  - Tested with 3 ZEDs in HD720 @ 30fps
  - Tested with 4 ZEDs in VGA @ 30fps
- If corrupted frames appear (green or purple images, tearing), the USB3.0 controller maximum bandwidth has been reached. Either use an additional USB3.0 PCIe expansion card or reduce framerate.
- You can also use multiple GPUs to load-balance computations (use `param.device` to change the GPU used for the current ZED) and improve performance.
