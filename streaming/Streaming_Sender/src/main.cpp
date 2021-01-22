
// Standard includes
#include <stdio.h>
#include <string.h>

// ZED includes
#include <sl/Camera.hpp>

// Sample includes
#include "utils.hpp"

// Using namespace
using namespace sl;
using namespace std;

void print(string msg_prefix, ERROR_CODE err_code = ERROR_CODE::SUCCESS, string msg_suffix = "");
int parseArgs(int argc, char **argv, sl::InitParameters& param);

int main(int argc, char **argv) {
    // Create a ZED camera
    Camera zed;

    // Set configuration parameters for the ZED
    InitParameters init_parameters;
    init_parameters.camera_resolution = sl::RESOLUTION::HD720;
    init_parameters.camera_fps = 15;
    init_parameters.depth_mode = DEPTH_MODE::PERFORMANCE;
    init_parameters.coordinate_units = UNIT::METER;
    init_parameters.depth_minimum_distance = 0.5;
    init_parameters.sdk_verbose = true;
    int res_arg = parseArgs(argc, argv, init_parameters);

    // Open the camera
    auto returned_state = zed.open(init_parameters);
    if (returned_state != ERROR_CODE::SUCCESS) {
        print("Camera Open", returned_state, "Exit program.");
        return EXIT_FAILURE;
    }
    
    // Set parameters for Positional Tracking
    PositionalTrackingParameters positional_tracking_param;
    positional_tracking_param.enable_area_memory = true;
    // enable Positional Tracking
    returned_state = zed.enablePositionalTracking(positional_tracking_param);
    if (returned_state != ERROR_CODE::SUCCESS) {
        print("Enabling positionnal tracking failed: ", returned_state);
        zed.close();
        return EXIT_FAILURE;
    }

    StreamingParameters stream_params;
    stream_params.codec = STREAMING_CODEC::H264;
    stream_params.bitrate = 8000;
    stream_params.chunk_size = 4096;
    //stream_params.adaptative_bitrate = true;
    if (argc == 2 && res_arg == 1) stream_params.port = atoi(argv[1]);
    if (argc > 2) stream_params.port = atoi(argv[2]);

    returned_state = zed.enableStreaming(stream_params);
    if (returned_state != ERROR_CODE::SUCCESS) {
        print("Streaming initialization error: ", returned_state);
        return EXIT_FAILURE;
    }

    print("Streaming on port " + to_string(stream_params.port));

    SetCtrlHandler();

    while (!exit_app) {
        if (zed.grab() != ERROR_CODE::SUCCESS)
            sleep_ms(1);
    }

    // disable Streaming
    zed.disableStreaming();

    // close the Camera
    zed.close();
    return EXIT_SUCCESS;
}

void print(string msg_prefix, ERROR_CODE err_code, string msg_suffix) {
    cout << "[Sample]";
    if (err_code != ERROR_CODE::SUCCESS)
        cout << "[Error] ";
    else
        cout << " ";
    cout << msg_prefix << " ";
    if (err_code != ERROR_CODE::SUCCESS) {
        cout << " | " << toString(err_code) << " : ";
        cout << toVerbose(err_code);
    }
    if (!msg_suffix.empty())
        cout << " " << msg_suffix;
    cout << endl;
}

int parseArgs(int argc, char **argv, sl::InitParameters& param) {
    if (argc > 1 && string(argv[1]).find(".svo") != string::npos) {
        // SVO input mode
        param.input.setFromSVOFile(argv[1]);
        cout << "[Sample] Using SVO File input: " << argv[1] << endl;
    } else if (argc > 1 && string(argv[1]).find(".svo") == string::npos) {
        string arg = string(argv[1]);
        unsigned int a, b, c, d, port;
        if (sscanf(arg.c_str(), "%u.%u.%u.%u:%d", &a, &b, &c, &d, &port) == 5) {
            // Stream input mode - IP + port
            string ip_adress = to_string(a) + "." + to_string(b) + "." + to_string(c) + "." + to_string(d);
            param.input.setFromStream(sl::String(ip_adress.c_str()), port);
            cout << "[Sample] Using Stream input, IP : " << ip_adress << ", port : " << port << endl;
        } else if (sscanf(arg.c_str(), "%u.%u.%u.%u", &a, &b, &c, &d) == 4) {
            // Stream input mode - IP only
            param.input.setFromStream(sl::String(argv[1]));
            cout << "[Sample] Using Stream input, IP : " << argv[1] << endl;
        } else if (arg.find("HD2K") != string::npos) {
            param.camera_resolution = sl::RESOLUTION::HD2K;
            cout << "[Sample] Using Camera in resolution HD2K" << endl;
        } else if (arg.find("HD1080") != string::npos) {
            param.camera_resolution = sl::RESOLUTION::HD1080;
            cout << "[Sample] Using Camera in resolution HD1080" << endl;
        } else if (arg.find("HD720") != string::npos) {
            param.camera_resolution = sl::RESOLUTION::HD720;
            cout << "[Sample] Using Camera in resolution HD720" << endl;
        } else if (arg.find("VGA") != string::npos) {
            param.camera_resolution = sl::RESOLUTION::VGA;
            cout << "[Sample] Using Camera in resolution VGA" << endl;
        } else
            return 1;
    } else {
        // Default
        return 1;
    }
    return 0;
}

