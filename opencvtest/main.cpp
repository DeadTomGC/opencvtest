#include <OpenNI.h>
#include <opencv2/opencv.hpp>
#include <iostream>

using namespace cv;
using namespace openni;

int main()
{
    OpenNI::initialize();
    puts( "Kinect initialization..." );
    Device device;
    if ( device.open( openni::ANY_DEVICE ) != 0 )
    {
        puts( "Kinect not found !" ); 
        return -1;
    }
    puts( "Kinect opened" );
    VideoStream color;
    color.create( device, SENSOR_COLOR );
    color.start();
    puts( "Camera ok" );
    VideoMode paramvideo;
    paramvideo.setResolution( 640, 480 );
    paramvideo.setFps( 30 );
    paramvideo.setPixelFormat( PIXEL_FORMAT_DEPTH_100_UM );
    paramvideo.setPixelFormat( PIXEL_FORMAT_RGB888 );
    color.setVideoMode( paramvideo );
    puts( "Réglages des flux vidéos ok" );

    // If the depth/color synchronisation is not necessary, start is faster :
    device.setDepthColorSyncEnabled( false );

    // Otherwise, the streams can be synchronized with a reception in the order of our choice :
    //device.setDepthColorSyncEnabled( true );
    //device.setImageRegistrationMode( openni::IMAGE_REGISTRATION_DEPTH_TO_COLOR );

    VideoStream* stream = new VideoStream();
    stream = &color;
    puts( "Kinect initialization completed" );
	CascadeClassifier haar_cascade;
	haar_cascade.load("haarcascade_frontalface_alt.xml");
    if ( device.getSensorInfo( SENSOR_DEPTH ) != NULL )
    {
        VideoFrameRef colorFrame;
        cv::Mat colorcv( cv::Size( 640, 480 ), CV_8UC3, NULL );
        cv::namedWindow( "RGB", CV_WINDOW_AUTOSIZE );

        int changedIndex;
        while( device.isValid() )
        {
			Sleep(1000);
            OpenNI::waitForAnyStream( &stream, 1, &changedIndex );
            switch ( changedIndex )
            {
                case 0:
                    color.readFrame( &colorFrame );

                    if ( colorFrame.isValid() )
                    {
                        colorcv.data = (uchar*) colorFrame.getData();
						Mat gray;
						cvtColor(colorcv, gray, CV_BGR2GRAY);
                        cv::cvtColor( colorcv, colorcv, CV_BGR2RGB );
						vector< Rect_<int> > faces;
						haar_cascade.detectMultiScale(colorcv, faces);
						std::cout<<"People be at these places:\n";
						for(int i=0;i<faces.size();i++){
							std::cout<<faces[i].x<<" "<<faces[i].y<<std::endl;


						}
                        cv::imshow( "RGB", colorcv );
                    }
                    break;

                default:
                    puts( "Error retrieving a stream" );
            }
            cv::waitKey( 1 );
        }

        cv::destroyWindow( "RGB" );
    }
    color.stop();
    color.destroy();
    device.close();
    OpenNI::shutdown();
}