#include <cstdlib>
#include <exception>
#include <librealsense2/hpp/rs_frame.hpp>
#include <librealsense2/hpp/rs_pipeline.hpp>
#include <librealsense2/hpp/rs_processing.hpp>
#include <librealsense2/hpp/rs_types.hpp>
#include <librealsense2/rs.hpp>
#include <iostream>
#include <memory>
#include <chrono>

int main( int argc, char *argv[]) try{
    
    //rs2::pipeline p;
    //p.start();
    //rs2::frameset frames = p.wait_for_frames();
    //rs2::depth_frame depth = frames.get_depth_frame();
    //get the depth frame's dimensions
    //float width=depth.get_width();
    //float height= depth.get_height();
    //float dist_to_center= depth.get_distance(width/2, height/2);
    bool recording= false;

    //start record
    recording= true;
    
    rs2::frameset frames;
    rs2::frame depth;
    rs2::colorizer color_map;

    //create a shared pointer to a pipeline
    auto pipe= std::make_shared<rs2::pipeline>();
    //start streaming
    pipe->start();
    //initialize a shared pointer to a device with the current deviece on the pipeline
    rs2::device device= pipe->get_active_profile().get_device();

    pipe->stop();// stop the pipeline with default configuration
    pipe = std::make_shared<rs2::pipeline>();
    rs2::config cfg;

    //cfg.enable_record_to_file("a.bag")
    cfg.enable_record_to_file("./a.bag");
    pipe->start(cfg);//file will open at this point
    device= pipe->get_active_profile().get_device();

    auto start= std::chrono::steady_clock::now(); 
    std::chrono::seconds t_dur(5);
    while(true){
        auto end= std::chrono::steady_clock::now();
        std::chrono::duration<double> Elapsed =end-start;
        std::cout<<Elapsed.count()<<"sec\r";
        if(Elapsed.count()>=5)
            break;

    }
    recording= false;
    pipe->stop(); //stop the pipeline that holds the file and the recorder
    pipe= std::make_shared<rs2::pipeline>();//reset the shared pointer with a new pipeline

    std::cout<<"end of record";



}

catch(const rs2::error & e){
    std:: cerr << "Realsense error calling"<< e.get_failed_function()<<"("<<e.get_failed_args()<<")\n"<<e.what()<< std::endl;
    return EXIT_FAILURE;
}

catch(const std::exception& e){
    std::cerr << e.what()<< std::endl;
    return EXIT_FAILURE;
}
