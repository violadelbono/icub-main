// -*- mode:C++; tab-width:4; c-basic-offset:4; indent-tabs-mode:nil -*-

//
// $Id: DragonflyDeviceDriver2.h,v 1.1 2009/03/18 17:03:16 ale-scalzo Exp $
//
//

#ifndef __DragonflyDeviceDriver2h__
#define __DragonflyDeviceDriver2h__

//=============================================================================
// YARP Includes
//=============================================================================

// May 06, readapted for YARP2 by nat

#include <yarp/os/Semaphore.h>
#include <yarp/os/Bottle.h>
#include <yarp/dev/FrameGrabberInterfaces.h>
#include <yarp/os/Stamp.h>
#include <yarp/dev/PreciselyTimed.h>

namespace yarp {
    namespace dev {
        class Dragonfly2OpenParameters;
        class DragonflyDeviceDriver2;
    }
}

/**
* \file FirewireCamera.h device driver for managing the 
* IEEE-1394-DR2 Camera
*/

/**
* Structure for defining the open() parameters of the camera.
*/
class yarp::dev::Dragonfly2OpenParameters
{
public:
    // Parameters
    unsigned int _unit_number;
    unsigned int _port_number;
    unsigned int _size_x;
    unsigned int _size_y;
    unsigned int _video_type;

    double _brightness;
    double _exposure;
    double _sharpness;
    double _whiteB,_whiteR;
    double _hue;
    double _saturation;
    double _gamma;
    double _shutter;
    double _gain;
    double _iris;

    bool _DR2; // Dragonfly2 model

    bool _fleacr;  //FLEA color reconstruction flag

    /**
    * Constructor. Add here the parameters for the open().
    */
    Dragonfly2OpenParameters()
    {
        // parameters initialization
        _port_number = 0;
        _unit_number = 0;
        _size_x = 0;
        _size_y = 0;
        _video_type = 0;

        //uninitialized - inherit registry stored values
        _brightness=-1.0;
        _exposure=-1.0;
        _sharpness=-1.0;
        _whiteB=_whiteR=-1.0;
        _hue=-1.0;
        _saturation=-1.0;	    
        _gamma=-1.0;	    
        _shutter=-1.0;
        _gain=-1.0;
        _iris=-1.0;

        _DR2 = false;

        // FLEA cameras are compatible with DRAGONFLY's but ...
        // the color reconstruction method is different 
        // (GBRG instead of RGGB)
        // The default is to use Dragonsfly's method
        _fleacr = false;

    }

};

/**
*
@ingroup icub_hardware_modules
\defgroup icub_dragonfly2 dragonfly2
The dragonfly2 framegrabber device driver can acquire RGB color images in 320x240 or 640x480 resolutions.

\section intro_sec Description
The dragonfly2 framegrabber device driver is based on libdc1394-2. 
It can acquire RGB color images in 320x240 or 640x480 resolutions. 
In 640x480 there are two options: Bayer decoding performed on board by the camera or Bayer pattern decoding performed by the driver. In the second mode the bandwidth required to the Firewire bus is lower, and thus the framerate can be up to 60 fps. In the first mode the framerate is limited to 15 fps with two cameras on the same channel. Moreover, once the resolution is chosen, 
the image can be cropped on board by the camera before the transfer on the Firewire bus. 

\section lib_sec Libraries
libdc1394-2 (Linux)
PGRFlyCapture (Windows)

\section parameters_sec Parameters
--name             // image output port

--DR2              // if specified, dragonfly2 hardware model is assumed

--video_type       // 1: RGB 320x240, 2: RGB 640x480 (default), 3: RGB 640x480 software Bayer decoding (higher fps) 

--width|size_x     // width image cropping (limited to image size)

--height|size_y    // height image cropping (limited to image size)

--port_number      // Firewire channel (need to specify only if you have more than one firewire card)

--d|unit_number    // camera unit number 

--white_balance    // red and blue balance, values are normalized betwen 0.0 and 1.0 

--feature          // camera feature setting, normalized between 0.0 and 1.0 (features listed below)

Features:

- brightness
- exposure
- sharpness
- hue
- saturation
- gamma
- shutter
- gain
- iris

\section portsa_sec Ports Accessed
None.

\section portsc_sec Ports Created
Output ports:
- <as specified by --name> streams out a yarp::sig::ImageOf<yarp::sig::PixelRgb> which contains the image grabbed by the Dragonfly camera.

Input ports:
- <as specified by --name> input port to control the camera features and acquisition modes, accept a yarp::os::Bottle. The FrameGrabberGui module connects to this port.

\section in_files_sec Input Data Files
None.

\section out_data_sec Output Data Files
None.

\section conf_file_sec Configuration Files
None.

\section tested_os_sec Tested OS
Linux and Windows.

\section example_sec Example Instantiation of the Module
Usage syntax:

yarpdev --device grabber --subdevice dragonfly2 --name <yarp port name> [--DR2] 
[--video_type <type>] [--width|size_x <W> --height|size_y <H>] [--port_number <pn>] 
[--unit_number|d <un>] [--white_balance <red_value> <blue_value>] [--feature <value>] [...]

Example:

yarpdev --device grabber --subdevice dragonfly2 --name /icub/cam/left  --DR2 --d 0|1 [...]

yarpdev --device grabber --subdevice dragonfly2 --name /icub/cam/right --DR2 --d 1|0 [...]

\author Paul Fitzpatrick, Giorgio Metta, Alessandro Scalzo

Copyright (C) 2008 RobotCub Consortium

CopyPolicy: Released under the terms of the GNU GPL v2.0.

This file can be edited at src/modules/dragonfly2/DragonflyDeviceDriver2.h
**/

/**
* dragonfly2 device driver implementation.
*/

class yarp::dev::DragonflyDeviceDriver2 : 
    public IFrameGrabber, 
    public IPreciselyTimed,
    public IFrameGrabberRgb, public IFrameGrabberImage, public IFrameGrabberControlsDC1394, public DeviceDriver
{
private:
    DragonflyDeviceDriver2(const DragonflyDeviceDriver2&);
    void operator=(const DragonflyDeviceDriver2&);

public:
    /**
    * Constructor.
    */
    DragonflyDeviceDriver2();

    /**
    * Destructor.
    */
    virtual ~DragonflyDeviceDriver2();

    /**
    * Initialize the device driver implementation.
    * @param par parameters for the device driver implementation
    * @return returns true on success, false on failure.
    */
    bool open(const Dragonfly2OpenParameters& par);

    /**
    * Open the device driver.
    * @param config configuration for the device driver
    * @return returns true on success, false on failure.
    */
    virtual bool open(yarp::os::Searchable& config)
    {
        Dragonfly2OpenParameters params;
        yarp::os::Value *value;
        if (config.check("port_number",value)) {
            params._port_number = value->asInt();
        }		
        if (config.check("unit_number",value)||config.check("d",value)) {
            params._unit_number = value->asInt();
        }
        if (config.check("size_x",value)||config.check("width",value)){
            params._size_x  = value->asInt();
        }
        if (config.check("size_y",value)||config.check("height",value)){
            params._size_y  = value->asInt();
        }

        params._video_type=0;
        if (config.check("video_type",value))
        {
            params._video_type = value->asInt();
        }

        //params._offset_y = config.find("offset_y").asInt();
        //params._offset_x = config.find("offset_x").asInt();
        //params._alfa = (float)config.find("alfa").asInt();

        if (config.check("brightness", value)){
            params._brightness=value->asDouble();
        }
        if (config.check("exposure", value)){
            params._exposure=value->asDouble();
        }
        if (config.check("sharpness", value)){
            params._sharpness=value->asDouble();
        }
        yarp::os::Bottle& white_balance = config.findGroup("white_balance");
        if (!white_balance.isNull()) {
            params._whiteR = white_balance.get(1).asDouble();
            params._whiteB = white_balance.get(2).asDouble();
        }
        if (config.check("hue", value)){
            params._hue=value->asDouble();
        }
        if (config.check("saturation", value)){
            params._saturation=value->asDouble();
        }
        if (config.check("gamma", value)){
            params._gamma=value->asDouble();
        }
        if (config.check("shutter", value)){
            params._shutter=value->asDouble();
        }
        if (config.check("gain", value)){
            params._gain=value->asDouble();
        }
        if (config.check("iris", value)){
            params._iris=value->asDouble();
        }       

        params._DR2 = config.check("DR2","If present indicates Dragonfly2 camera model");

        params._fleacr = config.check("flea", "If present indicates to use Flea color reconstruction ");


        return open(params);
    }

    /**
    * Closes the device driver.
    * @return returns true/false on success/failure.
    */
    virtual bool close(void);

    /**
    * Implements FrameGrabber basic interface.
    * @param buffer the pointer to the array to store the last frame.
    * @return returns true/false on success/failure.
    */
    virtual bool getRawBuffer(unsigned char *buffer);

    /**
    * Implements the Frame grabber basic interface.
    * @return the size of the raw buffer (for the Dragonfly
    * camera this is 1x640x480).
    */
    virtual int getRawBufferSize();

    /**
    * Implements FrameGrabber basic interface.
    */
    virtual int height() const;

    /**
    * Implements FrameGrabber basic interface.
    */
    virtual int width() const;

    /** 
    * FrameGrabber bgr interface, returns the last acquired frame as
    * a buffer of bgr triplets. A demosaicking method is applied to 
    * reconstuct the color from the Bayer pattern of the sensor.
    * @param buffer pointer to the array that will contain the last frame.
    * @return true/false upon success/failure
    */
    virtual bool getRgbBuffer(unsigned char *buffer);

    /** 
    * FrameGrabber image interface, returns the last acquired frame as
    * an rgb image. A demosaicking method is applied to 
    * reconstuct the color from the Bayer pattern of the sensor.
    * @param image that will store the last frame.
    * @return true/false upon success/failure
    */
    virtual bool getImage(yarp::sig::ImageOf<yarp::sig::PixelRgb>& image);

    /** 
    * Implements the IPreciselyTimed interface.
    * @return the yarp::os::Stamp of the last image acquired
    */
    virtual yarp::os::Stamp getLastInputStamp();

    /**
    * Set Brightness.
    * @param v normalized image brightness [0.0 : 1.0]. 
    * @return true/false upon success/failure
    */
    virtual bool setBrightness(double v);
    /**
    * Set Exposure.
    * @param v normalized image exposure [0.0 : 1.0].
    * @return true/false upon success/failure
    */
    virtual bool setExposure(double v);
    /**
    * Set Sharpness. 
    * @param v normalized image sharpness [0.0 : 1.0].
    * @return true/false upon success/failure
    */
    virtual bool setSharpness(double v);
    /**
    * Set White Balance.
    * @param blue normalized image blue balance [0.0 : 1.0].
    * @param red normalized image red balance [0.0 : 1.0].
    * @return true/false upon success/failure
    */
    virtual bool setWhiteBalance(double blue, double red);
    /**
    * Set Hue.
    * @param v normalized image hue [0.0 : 1.0].
    * @return true/false upon success/failure
    */
    virtual bool setHue(double v);
    /**
    * Set Saturation.
    * @param v normalized image saturation [0.0 : 1.0]. 
    * @return true/false upon success/failure
    */
    virtual bool setSaturation(double v);
    /**
    * Set Gamma.
    * @param v normalized image gamma [0.0 : 1.0].
    * @return true/false upon success/failure
    */
    virtual bool setGamma(double v);		
    /**
    * Set Shutter.
    * @param v normalized camera shutter time [0.0 : 1.0].
    * @return true/false upon success/failure
    */
    virtual bool setShutter(double v);
    /**
    * Set Gain. 
    * @param v normalized camera gain [0.0 : 1.0].
    * @return true/false upon success/failure
    */
    virtual bool setGain(double v);
    /**
    * Set Iris. 
    * @param v normalized camera iris [0.0 : 1.0].
    * @return true/false upon success/failure
    */
    virtual bool setIris(double v);
    //virtual bool setTemperature(double v);
    //virtual bool setWhiteShading(double r,double g,double b);
    //virtual bool setOpticalFilter(double v);
    //virtual bool setCaptureQuality(double v);

    /**
    * Get Brightness.
    * @return normalized image brightness [0.0 : 1.0].
    */
    virtual double getBrightness();
    /**
    * Get Exposure.
    * @return normalized image exposure [0.0 : 1.0].
    */
    virtual double getExposure();    
    /**
    * Get Sharpness.
    * @return normalized image sharpness [0.0 : 1.0].
    */
    virtual double getSharpness();    
    /**
    * Get White Balance.
    * @param blue normalized blue balance [0.0 : 1.0].
    * @param red normalized red balance [0.0 : 1.0].
    * @return true/false upon success/failure
    */
    virtual bool getWhiteBalance(double &blue, double &red);
    /**
    * Get Hue.
    * @return normalized hue [0.0 : 1.0].
    */
    virtual double getHue();
    /**
    * Get Saturation.
    * @return normalized saturation [0.0 : 1.0].
    */
    virtual double getSaturation();
    /**
    * Get Gamma.
    * @return normalized gamma [0.0 : 1.0].
    */
    virtual double getGamma();
    /**
    * Get Shutter.
    * @return normalized shutter time [0.0 : 1.0].
    */
    virtual double getShutter();
    /**
    * Get Gain.
    * @return normalized gain [0.0 : 1.0].
    */
    virtual double getGain();
    /**
    * Get Iris.
    * @return normalized iris [0.0 : 1.0].
    */
    virtual double getIris();


    //virtual double getTemperature() const;
    //virtual double getWhiteShading() const;
    //virtual double getOpticalFilter() const;
    //virtual double getCaptureQuality() const;

    //virtual bool setAutoBrightness(bool bAuto=true);

    //virtual bool setAutoGain(bool bAuto=true);

    //virtual bool setAutoShutter(bool bAuto=true);

    //virtual bool setAutoWhiteBalance(bool bAuto=true);

    //virtual bool setAuto(bool bAuto=true);

    //virtual void PrintSettings();

    /////////
    // DC1394
    /////////

    /**
    * Is feature present?
    * @param feature feature ID.
    * @return true=present, false=not present.
    */
    virtual bool hasFeatureDC1394(int feature);
    /**
    * Set feature value.
    * @param feature feature ID
    * @param value normalized feature value [0.0 : 1.0].
    * @return true/false upon success/failure.
    */
    virtual bool setFeatureDC1394(int feature,double value);
    /**
    * Get feature value.
    * @param feature feature ID
    * @return normalized feature value [0.0 : 1.0].
    */	
    virtual double getFeatureDC1394(int feature);

    /**
    * Can be feature turned on/off?
    * @param feature feature ID.
    * @return true=YES, false=NO.
    */
    virtual bool hasOnOffDC1394(int feature);
    /**
    * Switch feature on/off.
    * @param feature feature ID.
    * @param onoff true=ON false=OFF.
    * @return true/false upon success/failure.
    */
    virtual bool setActiveDC1394(int feature, bool onoff);
    /**
    * Is feature ON or OFF?
    * @param feature feature ID.
    * @return true=ON false=OFF.
    */
    virtual bool getActiveDC1394(int feature);

    /**
    * Has feature Auto mode?
    * @param feature feature ID.
    * @return true=YES, false=NO.
    */
    virtual bool hasAutoDC1394(int feature);
    /**
    * Has feature Manual mode?
    * @param feature feature ID.
    * @return true=YES, false=NO.
    */	
    virtual bool hasManualDC1394(int feature);
    /**
    * Has feature Manual mode?
    * @param feature feature ID.
    * @return true=YES, false=NO.
    */
    virtual bool hasOnePushDC1394(int feature);
    /**
    * Switch feature Auto/Manual.
    * @param feature feature ID.
    * @param onoff true=Auto false=Manual.
    * @return true/false upon success/failure.
    */
    virtual bool setModeDC1394(int feature, bool auto_onoff);
    /**
    * Is feature mode Auto or Manual?
    * @param feature feature ID.
    * @return true=Auto false=Manual.
    */
    virtual bool getModeDC1394(int feature);
    /**
    * Trigger feature One Push adjust.
    * @param feature feature ID.
    * @return true/false upon success/failure.
    */
    virtual bool setOnePushDC1394(int feature);

    /**
    * Get supported video mode mask.
    * The video mode bitmask is obtained as follows:
    *
    * 	unsigned int mask=0;
    *
    *	for (unsigned int m=0; m<modes.num; ++m)
    *
    *			mask|=1<<(modes.modes[m]-DC1394_VIDEO_MODE_MIN);
    *
    * 0=160x120 YUV444, 1=320x240 YUV422, 2=640x480 YUV411, 3=640x480 YUV422, 4=640x480 RGB8, 5=640x480 MONO8,
    * 6=640x480 MONO16,7=800x600 YUV422, 8=800x600 RGB8, 9=800x600_MONO8, 10=1024x768 YUV422, 11=1024x768 RGB8, 12=1024x768 MONO8, 
    * 13=800x600 MONO16, 14=1024x768 MONO16, 15=1280x960 YUV422, 16=1280x960 RGB8, 17=1280x960_MONO8, 18=1600x1200 YUV422, 19=1600x1200 RGB8,
    * 20=1600x1200 MONO8, 21=1280x960 MONO16, 22=1600x1200_MONO16, 23=EXIF, 24=FORMAT7 0, 25=FORMAT7 1, 26=FORMAT7 2, 27=FORMAT7 3,
    * 28=FORMAT7 4, 29=FORMAT7 5, 30=FORMAT7 6, 31=FORMAT7 7
    * @return video mode bitmask.
    */
    virtual unsigned int getVideoModeMaskDC1394();
    /**
    * Get camera acquisition mode.
    * @return video mode ID: 0=160x120 YUV444, 1=320x240 YUV422, 2=640x480 YUV411, 3=640x480 YUV422, 4=640x480 RGB8, 5=640x480 MONO8,
    * 6=640x480 MONO16,7=800x600 YUV422, 8=800x600 RGB8, 9=800x600_MONO8, 10=1024x768 YUV422, 11=1024x768 RGB8, 12=1024x768 MONO8, 
    * 13=800x600 MONO16, 14=1024x768 MONO16, 15=1280x960 YUV422, 16=1280x960 RGB8, 17=1280x960_MONO8, 18=1600x1200 YUV422, 19=1600x1200 RGB8,
    * 20=1600x1200 MONO8, 21=1280x960 MONO16, 22=1600x1200_MONO16, 23=EXIF, 24=FORMAT7 0, 25=FORMAT7 1, 26=FORMAT7 2, 27=FORMAT7 3,
    * 28=FORMAT7 4, 29=FORMAT7 5, 30=FORMAT7 6, 31=FORMAT7 7
    */
    virtual unsigned int getVideoModeDC1394();
    /**
    * Set camera acquisition mode.
    * @param video_mode ID: 0=160x120 YUV444, 1=320x240 YUV422, 2=640x480 YUV411, 3=640x480 YUV422, 4=640x480 RGB8, 5=640x480 MONO8,
    * 6=640x480 MONO16,7=800x600 YUV422, 8=800x600 RGB8, 9=800x600_MONO8, 10=1024x768 YUV422, 11=1024x768 RGB8, 12=1024x768 MONO8, 
    * 13=800x600 MONO16, 14=1024x768 MONO16, 15=1280x960 YUV422, 16=1280x960 RGB8, 17=1280x960_MONO8, 18=1600x1200 YUV422, 19=1600x1200 RGB8,
    * 20=1600x1200 MONO8, 21=1280x960 MONO16, 22=1600x1200_MONO16, 23=EXIF, 24=FORMAT7 0, 25=FORMAT7 1, 26=FORMAT7 2, 27=FORMAT7 3,
    * 28=FORMAT7 4, 29=FORMAT7 5, 30=FORMAT7 6, 31=FORMAT7 7
    * @return true/false upon success/failure.
    */
    virtual bool setVideoModeDC1394(int video_mode);

    /**
    * Get supported framerates mask.
    * The framerates bitmask is obtained as follows:
    *
    *	unsigned int mask=0;
    *
    *	for (unsigned int f=0; f<fps.num; ++f)
    *
    *		mask|=1<<(fps.framerates[f]-DC1394_FRAMERATE_MIN);
    *
    * 0=1.875 fps, 1=3.75 fps, 2=7.5 fps, 3=15 fps, 4=30 fps, 5=60 fps, 6=120 fps, 7=240 fps.
    * @return framerates bitmask.
    */
    virtual unsigned int getFPSMaskDC1394();

    /**
    * Get camera framerate.
    * @return framerate mode ID: 0=1.875 fps, 1=3.75 fps, 2=7.5 fps, 3=15 fps, 4=30 fps, 5=60 fps, 6=120 fps, 7=240 fps.
    */
    virtual unsigned int getFPSDC1394();
    /**
    * Set camera framerate.
    * @param fps framerate ID: 0=1.875 fps, 1=3.75 fps, 2=7.5 fps, 3=15 fps, 4=30 fps, 5=60 fps, 6=120 fps, 7=240 fps.
    * @return true/false upon success/failure.
    */
    virtual bool setFPSDC1394(int fps);

    /**
    * Get camera Firewire ISO speed.
    * @return ISO speed ID: 0=100 Mbps, 1=200 Mbps, 2=400 Mbps, 3=800 Mbps, 4=1600 Mbps, 5=3200 Mbps.
    */
    virtual unsigned int getISOSpeedDC1394();
    /**
    * Set camera Firewire ISO speed.
    * @param speed ISO speed ID: 0=100 Mbps, 1=200 Mbps, 2=400 Mbps, 3=800 Mbps, 4=1600 Mbps, 5=3200 Mbps.
    * @return true/false upon success/failure.
    */
    virtual bool setISOSpeedDC1394(int speed);

    /**
    * Get supported color coding mask.
    * The framerates bitmask is obtained as follows:
    *
    *	unsigned int mask=0;
    *
    *	for (unsigned int m=0; m<codings.num; ++m)
    *
    *			mask|=1<<(codings.codings[m]-DC1394_COLOR_CODING_MIN);
    *
    * 0=MONO8, 1=YUV411, 2=YUV422, 3=YUV444, 4=RGB8, 5=MONO16, 6=RGB16, 7=MONO16S, 8=RGB16S, 9=RAW8, 10=RAW16.
    * @return framerates bitmask.
    */
    virtual unsigned int getColorCodingMaskDC1394(unsigned int video_mode);
    /**
    * Get image color coding.
    * @return image color coding ID: 0=MONO8, 1=YUV411, 2=YUV422, 3=YUV444, 4=RGB8, 5=MONO16, 6=RGB16, 7=MONO16S, 8=RGB16S, 9=RAW8, 10=RAW16.
    */
    virtual unsigned int getColorCodingDC1394();
    /**
    * Set image color coding.
    * @param coding image color coding ID: 0=MONO8, 1=YUV411, 2=YUV422, 3=YUV444, 4=RGB8, 5=MONO16, 6=RGB16, 7=MONO16S, 8=RGB16S, 9=RAW8, 10=RAW16.
    * @return true/false upon success/failure.
    */
    virtual bool setColorCodingDC1394(int coding);

    /**
    * Set White Balance.
    * @param blue normalized image blue balance [0.0 : 1.0].
    * @param red normalized image red balance [0.0 : 1.0].
    * @return true/false upon success/failure
    */
    virtual bool setWhiteBalanceDC1394(double b, double r);
    /**
    * Get White Balance.
    * @param blue normalized blue balance [0.0 : 1.0].
    * @param red normalized red balance [0.0 : 1.0].
    * @return true/false upon success/failure
    */
    virtual bool getWhiteBalanceDC1394(double &b, double &r);

    /** 
    * Get maximum image dimensions in Format 7 mode.
    * @param xdim maximum width
    * @param ydim maximum height
    * @param xstep width granularity
    * @param ystep height granularity
    * @return true/false upon success/failure
    */
    virtual bool getFormat7MaxWindowDC1394(unsigned int &xdim,unsigned int &ydim,unsigned int &xstep,unsigned int &ystep);

    /** 
    * Get image dimensions in Format 7 mode.
    * @param xdim image width
    * @param ydim image height
    * @return true/false upon success/failure
    */
    virtual bool getFormat7WindowDC1394(unsigned int &xdim,unsigned int &ydim);
    /** 
    * Set image dimensions in Format 7 mode.
    * @param xdim image width
    * @param ydim image height
    * @return true/false upon success/failure
    */
    virtual bool setFormat7WindowDC1394(unsigned int xdim,unsigned int ydim);

    /**
    * Set Operation Mode.
    * @param b1394b true=1394b false=LEGACY
    * @return true/false upon success/failure
    */
    virtual bool setOperationModeDC1394(bool b1394b);
    /**
    * Get Operation Mode.
    * @return true=1394b false=LEGACY
    */
    virtual bool getOperationModeDC1394(); 

    /**
    * Set image transmission ON/OFF.
    * @param bTxON true=ON false=OFF
    * @return true/false upon success/failure
    */
    virtual bool setTransmissionDC1394(bool bTxON);
    /**
    * Is image transmission ON or OFF?
    * @return true=ON false=OFF
    */
    virtual bool getTransmissionDC1394();
    //virtual bool setBayerDC1394(bool bON);
    //virtual bool getBayerDC1394();

    /**
    * Set feature commands propagation ON/OFF.
    * All the cameras on the same Firewire bus can be adjusted at once setting broadcast ON. In this way, they will share the feature settings. 
    * @param onoff true=ON false=OFF
    * @return true/false upon success/failure
    */
    virtual bool setBroadcastDC1394(bool onoff);
    /**
    * Set camera features to default.
    * @return true/false upon success/failure
    */
    virtual bool setDefaultsDC1394();
    /*
    * Reset camera.
    * @return true/false upon success/failure
    */
    virtual bool setResetDC1394();
    /**
    * Switch camera power ON/OFF.
    * @param onoff true=ON false=OFF
    * @return true/false upon success/failure
    */
    virtual bool setPowerDC1394(bool onoff);
    /**
    * Switch image capture ON/OFF.
    * @param onoff true=ON false=OFF
    * @return true/false upon success/failure
    */
    virtual bool setCaptureDC1394(bool bON);

    /** 
    * Get Firewire communication packet size.
    * In Format7 mode the framerate depends from packet size.
    * @return bytes per packet
    */
    virtual unsigned int getBytesPerPacketDC1394();
    /** 
    * Set Firewire communication packet size.
    * In Format7 mode the framerate depends from packet size.
    * @param bpp bytes per packet
    * @return true/false upon success/failure
    */
    virtual bool setBytesPerPacketDC1394(unsigned int bpp);

protected:
    void* system_resources;
};

#endif
