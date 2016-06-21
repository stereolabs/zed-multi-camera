///////////////////////////////////////////////////////////////////////////
//
// Copyright (c) 2015, STEREOLABS.
//
// All rights reserved.
//
// THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
// "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
// LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
// A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
// OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
// SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
// LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
// DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
// THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
// (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
// OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
//
///////////////////////////////////////////////////////////////////////////


/******************************************************************************************************************
 ** This sample demonstrates how to use two ZEDs with the ZED SDK, each grab are in a separated thread            **
 ** This sample has been tested with 3 ZEDs in HD720@30fps resolution                                             **
 ** This only works for Linux                                                                                     **
 *******************************************************************************************************************/

//standard includes
#include <stdio.h>
#include <string.h>
#include <ctime>
#include <chrono>
#include <thread>

//opencv includes
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>

//ZED Includes
#include <zed/Camera.hpp>

using namespace sl::zed;
using namespace std;

const int NUM_CAMERAS = 2;
const int FPS = 15;
const ZEDResolution_mode ZED_RES = HD720;


Camera* zed[NUM_CAMERAS];
cv::Mat SbSResult[NUM_CAMERAS];
cv::Mat ZED_LRes[NUM_CAMERAS];
int width, height;
long long ZED_Timestamp[NUM_CAMERAS];

bool stop_signal;

void grab_run(int x) {
    while (!stop_signal) {
        bool res = zed[x]->grab(SENSING_MODE::STANDARD, 1, 1);

        if (!res) {
            ZED_Timestamp[x] = zed[x]->getCameraTimestamp();
            //sl::zed::Mat depthMM = zed[x]->retrieveMeasure(MEASURE::DEPTH);
            slMat2cvMat(zed[x]->retrieveImage(SIDE::LEFT)).copyTo(SbSResult[x](cv::Rect(0, 0, width, height)));
            slMat2cvMat(zed[x]->normalizeMeasure(MEASURE::DISPARITY)).copyTo(SbSResult[x](cv::Rect(width, 0, width, height)));
        }
        std::this_thread::sleep_for(std::chrono::milliseconds(1));
    }
    delete zed[x];
}
//main  function

int main(int argc, char **argv) {

#ifdef WIN32
    std::cout << "Multiple ZEDs are not available under Windows" << std::endl;
    return -1;
#endif

    sl::zed::InitParams params;
    params.mode = MODE::PERFORMANCE;

    // Create
    for (int i = 0; i < NUM_CAMERAS; i++) {
        zed[i] = new Camera(ZED_RES, FPS, i);

        ERRCODE err = zed[i]->init(params);

        cout << "ZED N°" << i << " -> Result : " << errcode2str(err) << endl;
        if (err != SUCCESS) {
            delete zed[i];
            return 1;
        }

        width = zed[i]->getImageSize().width;
        height = zed[i]->getImageSize().height;
        SbSResult[i] = cv::Mat(height, width * 2, CV_8UC4, 1);
    }

    char key = ' ';

    //Create both grabbing thread with the camera number as parameters
    std::vector<std::thread*> thread_vec;
    for (int i = 0; i < NUM_CAMERAS; i++)
        thread_vec.push_back(new std::thread(grab_run, i));

    //Create windows for viewing results with OpenCV
    cv::Size DisplaySize(720, 404);

    for (int i = 0; i < NUM_CAMERAS; i++)
        ZED_LRes[i] = cv::Mat(DisplaySize, CV_8UC4);

    //loop until 'q' is pressed
    while (key != 'q') {
        //Resize and imshow
        for (int i = 0; i < NUM_CAMERAS; i++) {
            char wnd_name[21];
            sprintf(wnd_name, "ZED N° %d", i);
            cv::resize(SbSResult[i], ZED_LRes[i], DisplaySize);
            cv::imshow(wnd_name, ZED_LRes[i]);
        }

        //compare Timestamp between both camera (uncomment following line)
        //for (int i = 0; i < NUM_CAMERAS; i++) std::cout << " Timestamp " << i << ": " << ZED_Timestamp[i] << std::endl;

        //get the Key
        key = cv::waitKey(20);
    }

    //out --> tells both thread to finish
    stop_signal = true;

    //end of thread --sync
    for (auto it : thread_vec) it->join();

    return 0;
}
