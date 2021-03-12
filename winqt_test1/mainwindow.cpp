#include "mainwindow.h"
#include "ui_mainwindow.h"
using namespace std;
using namespace cv;

QImage cvMat_to_QImage(const cv::Mat &mat );
//change realsense frame to mat
cv::Mat draw_color(rs2::frame color_frame);

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    //test caculating
    double D=20.4;
    int I=2;
    D/= I;
    qDebug()<<"CCCCC : "<<D;
    //
    string filename;
    filename ="ww";
    QString name = QFileDialog::getOpenFileName(this, tr("Open .bag file"), "F://Fruit_harvest//Raw_data");
    qDebug()<<name;
    filename = name.toStdString();
    QString qstr = QString::fromStdString(filename);
    qDebug()<<qstr;

    //initialize realsense
       rs2::config cfg;
       rs2::context context;
       const rs2::playback playback = context.load_device(filename);
       const std::vector<rs2::sensor> sensors = playback.query_sensors();
       for(const rs2::sensor& sensor : sensors){

           const std::vector<rs2::stream_profile> stream_profiles = sensor.get_stream_profiles();
           for(const rs2::stream_profile& stream_profile : stream_profiles){
   //            std::cout << stream_profile.stream_name();
   //            cfg.enable_stream(stream_profile.stream_type(), stream_profile.stream_index());
           }
       }

     cfg.enable_device_from_file(filename);
         rs2::pipeline pipeline;
         auto pipeline_profile = pipeline.start(cfg); //pipeline_profile 沒宣告 先試試auto
         pipeline_profile.get_device().as<rs2::playback>().set_real_time(false);

         // Show enable streams
         qDebug() << "Enable streams: ";
         const std::vector<rs2::stream_profile> stream_profiles = pipeline_profile.get_streams();
         for(const rs2::stream_profile stream_profile : stream_profiles){
             qDebug() << QString::fromUtf8(stream_profile.stream_name().c_str());
         }
         // Preprocessing setting
         rs2::decimation_filter dec;
         dec.set_option(RS2_OPTION_FILTER_MAGNITUDE, 2);
         rs2::disparity_transform depth2disparity;
         rs2::disparity_transform disparity2depth(false);
         rs2::spatial_filter spat;
         spat.set_option(RS2_OPTION_FILTER_MAGNITUDE, 2);
         spat.set_option(RS2_OPTION_FILTER_SMOOTH_ALPHA, 0.5);
         spat.set_option(RS2_OPTION_FILTER_SMOOTH_DELTA, 20);
         rs2::temporal_filter temp;
         qDebug() << "G1";
         // frame preset
         int frame_int = 0;
         uint64_t last_position = pipeline_profile.get_device().as<rs2::playback>().get_position();
         rs2::align align_to(RS2_STREAM_COLOR);

         auto frameset=pipeline.wait_for_frames();//not sure if this is right
         //change rs frame to cv mat preset
         auto color_frame = frameset.get_color_frame();
         rs2::depth_frame d_frame=frameset.get_depth_frame();

             // Open a video file or an image file or a camera stream.
             string str, outputFile;
             Mat frame, blob;
             Mat curr_mat;
             while(true){
         //        if(frame == 300)    break;

                   if(pipeline.poll_for_frames(&frameset)){
                     QElapsedTimer timer;
                     timer.start();
                     frameset = align_to.process(frameset);
                     frameset = frameset.apply_filter(depth2disparity);
                     frameset = frameset.apply_filter(spat);
                     frameset = frameset.apply_filter(temp);
                     frameset = frameset.apply_filter(disparity2depth);
                     //renew frames
                     color_frame = frameset.get_color_frame();
                     frame=draw_color(color_frame);
                     d_frame=frameset.get_depth_frame();
                     if (frame.empty()){
                          qDebug() <<"FK NO";
                          break;
                     }
                     if((frame_int > 36) && (frame_int%1 == 0)){
                         curr_mat = frame.clone();

                         qDebug() << frame.rows << frame.cols<< "ID:"<<frame_int;

                         //curr_vec = bbox_t2bbox_t_history(temp);

                         //show
                         ui->label->clear();
                         QImage image=cvMat_to_QImage(frame);
                         QPixmap image_pixmap=QPixmap::fromImage(image);
                         ui->label->setPixmap(image_pixmap.scaled(ui->label->width(),ui->label->height(),Qt::KeepAspectRatio));
                         //cv::imshow("dd", frame);
                         }
                     frame_int++;

                     const uint64_t current_position = pipeline_profile.get_device().as<rs2::playback>().get_position();
                     if( static_cast<int64_t>( current_position - last_position ) < 0 ){
                         qDebug() << "END";
                       break;
                       }
                     last_position = current_position;

                 }
             }
}

MainWindow::~MainWindow()
{
    delete ui;
}


cv::Mat draw_color(rs2::frame color_frame){

    int color_width = color_frame.as<rs2::video_frame>().get_width();
    int color_height = color_frame.as<rs2::video_frame>().get_height();

    cv::Mat color_mat;
    const rs2_format color_format = color_frame.get_profile().format();
    switch( color_format ){
    case rs2_format::RS2_FORMAT_RGB8:{
        color_mat = cv::Mat( color_height, color_width, CV_8UC3, const_cast<void*>( color_frame.get_data() ) ).clone();
        cv::cvtColor( color_mat, color_mat, cv::COLOR_RGB2BGR );
        return color_mat;
        break;
    }
    case rs2_format::RS2_FORMAT_RGBA8:{
        color_mat = cv::Mat( color_height, color_width, CV_8UC4, const_cast<void*>( color_frame.get_data() ) ).clone();
        cv::cvtColor( color_mat, color_mat, cv::COLOR_RGBA2BGRA );
        return color_mat;
        break;
    }
    case rs2_format::RS2_FORMAT_BGR8:{
        color_mat = cv::Mat( color_height, color_width, CV_8UC3, const_cast<void*>( color_frame.get_data() ) ).clone();
        return color_mat;
        break;
    }
    case rs2_format::RS2_FORMAT_BGRA8:{
        color_mat = cv::Mat( color_height, color_width, CV_8UC4, const_cast<void*>( color_frame.get_data() ) ).clone();
        return color_mat;
        break;
    }
    case rs2_format::RS2_FORMAT_Y16:{
        color_mat = cv::Mat( color_height, color_width, CV_16UC1, const_cast<void*>( color_frame.get_data() ) ).clone();
        constexpr double scaling = static_cast<double>( std::numeric_limits<uint8_t>::max() ) / static_cast<double>( std::numeric_limits<uint16_t>::max() );
        color_mat.convertTo( color_mat, CV_8U, scaling );
        return color_mat;
        break;
    }
    case rs2_format::RS2_FORMAT_YUYV:{
        color_mat = cv::Mat( color_height, color_width, CV_8UC2, const_cast<void*>( color_frame.get_data() ) ).clone();
        cv::cvtColor( color_mat, color_mat, cv::COLOR_YUV2BGR_YUYV );
        return color_mat;
        break;
    }
    default:
        throw std::runtime_error( "unknown color format" );
        break;
    }
}
QImage cvMat_to_QImage(const cv::Mat &mat ) {
  switch ( mat.type() )
  {
     // 8-bit, 4 channel
     case CV_8UC4:
     {
        QImage image( mat.data, mat.cols, mat.rows, mat.step, QImage::Format_RGB32 );
        return image;
     }

     // 8-bit, 3 channel
     case CV_8UC3:
     {
        QImage image( mat.data, mat.cols, mat.rows, mat.step, QImage::Format_RGB888 );
        return image.rgbSwapped();
     }

     // 8-bit, 1 channel
     case CV_8UC1:
     {
        static QVector<QRgb>  sColorTable;
        // only create our color table once
        if ( sColorTable.isEmpty() )
        {
           for ( int i = 0; i < 256; ++i )
              sColorTable.push_back( qRgb( i, i, i ) );
        }
        QImage image( mat.data, mat.cols, mat.rows, mat.step, QImage::Format_Indexed8 );
        image.setColorTable( sColorTable );
        return image;
     }

     default:
        qDebug("Image format is not supported: depth=%d and %d channels\n", mat.depth(), mat.channels());
        break;
  }
  return QImage();
}
