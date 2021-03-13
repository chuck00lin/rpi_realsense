# rpi_realsense

## frame rate test


*compile parameters:*
`g++ dist_rs.cpp -o dist_rs $(pkg-config --cflags --libs realsense2 libusb-1.0) -lrealsense-file -lrealsense2-gl -lfw -lpthread`
