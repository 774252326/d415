#include "rssetting.hpp"
#include <opencv2/opencv.hpp>
#include <iostream>
#include <sstream>
#include <fstream>
#include <thread>
#include "fioqt.hpp"

void Record(std::string serialNumber, cv::Size frameSize=cv::Size(1920,1080), unsigned int count=300)
{
    rs2::pipeline pipe;

    rs2::config cfg;

    cfg.enable_device(serialNumber);

    cfg.enable_stream(RS2_STREAM_COLOR, frameSize.width, frameSize.height, RS2_FORMAT_BGR8, 30);
    cfg.enable_stream(RS2_STREAM_DEPTH, frameSize.width, frameSize.height, RS2_FORMAT_Z16, 30);

    rs2::pipeline_profile profile=pipe.start(cfg);
    rs2::frameset frames;

    std::vector<cv::Mat> cq(count);

    int64 t0=cv::getTickCount();

    std::ostringstream oss;

    for(unsigned int i=0;i<count;i++)
    {

        //Wait for all configured streams to produce a frame
#if 1
        frames = pipe.wait_for_frames();
#else
        if(!pipe.poll_for_frames(&frames))   continue;
        else
            i++;

#endif

        double timep=frames.get_timestamp();
        unsigned long fn=frames.get_frame_number();
        oss<<fn<<"\t"<<timep<<"\t";

        rs2::frame color_frame = frames.get_color_frame();
        rs2::frame depth_frame = frames.get_depth_frame();

        timep=color_frame.get_timestamp();
        fn=color_frame.get_frame_number();
        oss<<fn<<"\t"<<timep<<"\t";
        timep=depth_frame.get_timestamp();
        fn=color_frame.get_frame_number();
        oss<<fn<<"\t"<<timep<<"\n";

        // Creating OpenCV Matrix from a color image
        cv::Mat color(frameSize, CV_8UC3, (void*)color_frame.get_data(), cv::Mat::AUTO_STEP);

        color.copyTo(cq[i]);
    }

    int64 t1=cv::getTickCount();

    double time=(double)(t1-t0)/cv::getTickFrequency();

    std::cout<<"\nt="<<time<<"\n"<<count/time<<"\n"<<std::flush;

    pipe.stop();

    std::string fol=serialNumber+"/";


    SetFolder(fol);


    for(unsigned int i=0;i<count;i++)
    {

        char buf[16];
        sprintf(buf, "%03u", i);

        std::string ext(buf);

        cv::imwrite(fol+"color"+ext+".png", cq[i]);

        std::cout<<i<<"\n"<<std::flush;

    }

    std::ofstream ofs(fol+"time.stamp");
    ofs<<oss.str();
    ofs.close();


}

int main()
{
    //    RSSetting::Set(RS2_OPTION_ENABLE_AUTO_EXPOSURE, 0, "Intel RealSense D415 #802212060171");
    //    RSSetting::Set(RS2_OPTION_EXPOSURE, 200, "Intel RealSense D415 #802212060171");


    RSSetting::Set(RS2_OPTION_INTER_CAM_SYNC_MODE, 1, "Intel RealSense D415 #802212060443", "Stereo Module");
    RSSetting::Set(RS2_OPTION_FRAMES_QUEUE_SIZE, 0, "Intel RealSense D415 #802212060443", "Stereo Module");
    RSSetting::Set(RS2_OPTION_FRAMES_QUEUE_SIZE, 0, "Intel RealSense D415 #802212060443", "RGB Camera");
    RSSetting::Set(RS2_OPTION_INTER_CAM_SYNC_MODE, 2, "Intel RealSense D415 #746112061911", "Stereo Module");
    RSSetting::Set(RS2_OPTION_FRAMES_QUEUE_SIZE, 0, "Intel RealSense D415 #746112061911", "Stereo Module");
    RSSetting::Set(RS2_OPTION_FRAMES_QUEUE_SIZE, 0, "Intel RealSense D415 #746112061911", "RGB Camera");

    RSSetting::Status();

    std::thread th2(Record, "802212060443", cv::Size(1280,720), 100);
    std::thread th1(Record, "746112061911", cv::Size(1280, 720), 100);

    th1.join();
    th2.join();

    return 0;
}
