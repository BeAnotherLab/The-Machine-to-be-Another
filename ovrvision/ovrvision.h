// ovrvision.h
// Version 2.12 : 2/Dec/2014
//
//MIT License
//THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
//IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
//FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
//AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
//LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
//OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
//THE SOFTWARE.
//
// Oculus Rift : TM & Copyright Oculus VR, Inc. All Rights Reserved
// Unity : TM & Copyright Unity Technologies. All Rights Reserved

#ifndef __OVRVISION__
#define __OVRVISION__

/////////// INCLUDE ///////////

//Pratform header
#ifdef WIN32
#ifndef _WIN32_WINNT
#define _WIN32_WINNT 0x400
#endif
#include <windows.h>
#endif /*WIN32*/

#ifdef MACOSX

#endif	/*MACOSX*/
	
#ifdef LINUX

#endif	/*LINUX*/

//Common header
#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <string.h>

#ifdef _OVRVISION_EXPORTS	//in ovrvision
#include "ovrvision_ds.h"   //DirectShow
#include "ovrvision_avf.h"   //AVFoundation
#include "ovrvision_undistort.h"
#else
//USB cameras driver
#ifdef WIN32
class OvrvisionDirectShow;
#elif MACOSX
#define OvrvisionAVFoundation   void
#elif LINUX

#endif
//Undistrot class
class OvrvisionUndistort;
#endif /*_OVRVISION_EXPORTS*/

//OVR Group
namespace OVR {

/////////// VARS AND DEFS ///////////

#ifdef WIN32
    #ifdef _OVRVISION_EXPORTS
    #define OVRPORT __declspec(dllexport)
    #else
    #define OVRPORT __declspec(dllimport)
    #endif
#endif /*WIN32*/
    
#ifdef MACOSX
    #define OVRPORT 
#endif	/*MACOSX*/
	
#ifdef LINUX
    
#endif	/*LINUX*/


//unsigned char to byte
typedef unsigned char byte;
typedef unsigned char* pbyte;

//Open Flags
typedef enum ov_cameraprop {
	OV_CAMVGA_FULL	= 0, 	//640x480 @60fps x2	: Faster
	OV_CAMVGA_SVGA,			//800x600 @30fps x2	: Slowly(Windows only)
} Camprop;

//Left or Right camera.
#ifndef _OV_CAMEYE_ENUM_
#define _OV_CAMEYE_ENUM_
typedef enum ov_cameraeye {
	OV_CAMEYE_LEFT = 0,		//Left camera
	OV_CAMEYE_RIGHT,		//Right camera
	OV_CAMNUM,
} Cameye;
#endif

//HMD Flags
typedef enum ov_hmdprop {
	OV_HMD_OCULUS_OTHER = 0,
	OV_HMD_OCULUS_DK1,
	OV_HMD_OCULUS_DK2,
} HmdProp;

//Device Status
typedef enum ov_camstatus {
	OV_CAMCLOSED= 0,			//No open device
	OV_CAMOPENED,				//Open device
} CamStatus;

//SetProperty AutoMode
#define OV_SET_AUTOMODE		(-1)

//Result define
#define OV_RESULT_OK		(0)
#define OV_RESULT_FAILED	(1)

//Processing Quality
typedef enum ov_psqt {
	OV_PSQT_NONE = 0,		//No Processing quality
	OV_PSQT_LOW = 1,		//Low Processing quality
	OV_PSQT_HIGH = 2,		//High Processing quality
	OV_PSQT_REFSET = 3,		//RefOnly Processing quality
} OvPSQuality;

//File path
#define OV_DEFAULT_SETTING_FILEPATH		"ovrvision_conf2.xml"

/////////// CLASS ///////////

//Ovrvision
class OVRPORT Ovrvision
{
public:
	//Constructor/Destructor
	Ovrvision();
	~Ovrvision();

	//Initialize
	//Open the Ovrvision
	int Open(int locationID, OVR::Camprop flag, OVR::HmdProp hmd = OV_HMD_OCULUS_DK2);
	//Close the Ovrvision
	void Close();

	//Get Camera data pre-store.
	void PreStoreCamData();
	//Get RGB8bit image data.
	// * You must call the PreStoreCamData() method ahead.
	unsigned char* GetCamImage(OVR::Cameye eye, OvPSQuality processing = OV_PSQT_HIGH);
	void GetCamImage(unsigned char* pImageBuf, OVR::Cameye eye, OvPSQuality processing = OV_PSQT_HIGH);
	//Get MJPEG raw data.
	// * You must call the PreStoreCamData() method ahead.
	void GetCamImageMJPEG(unsigned char* pImageBuf, int* pSize, OVR::Cameye eye);

	//This method will be detected if a hand is put in front of a camera. 
	bool PutHandInFrontOfCamera(unsigned char thres_less, unsigned char* pImageBuf);

	//Property

	//Set config parameter
	bool SetParamXMLfromFile(char* filename);
	//Save parameter
	//todo : char* savefilename = FullPath
	bool SaveParamXMLtoFile(char* savefilename = NULL);
	//DirectSaveParamXMLtoTempFile : Don't use this method. For Unity.
	void DirectSaveParamXMLtoTempFile(int* config1, float* config2);
	void DefaultSetting();

	//Set exposure
	void SetExposure(int value = OV_SET_AUTOMODE);
	//Set white balance
	void SetWhiteBalance(int value = OV_SET_AUTOMODE);
	//Set contrast ( manual only )
	void SetContrast(int value);
	//Set Saturation ( manual only )
	void SetSaturation(int value);
	//Set brightness ( manual only )
	void SetBrightness(int value);
	//Set sharpness ( manual only )
	void SetSharpness(int value);
	//Set gamma ( manual only )
	void SetGamma(int value);

	//Get exposure
	int GetExposure();
	//Get white balance
	int GetWhiteBalance();
	//Get contrast
	int GetContrast();
	//Get saturation
	int GetSaturation();
	//Get brightness
	int GetBrightness();
	//Get sharpness
	int GetSharpness();
	//Get gamma
	int GetGamma();

	//Get focalPoint
	float GetFocalPoint() { return m_focalPoint; };

	//Get image property
	Camprop GetCameraProperty();
	//Get camera status
	CamStatus GetCameraStatus();

	//Get pixel size
	int GetPixelSize();
	//Get image width
	int GetImageWidth(){ return m_width; };
	//Get image height
	int GetImageHeight(){ return m_height; };
	//Get image framerate
	int GetImageRate(){ return m_framerate; };
	//Get buffer size
	int GetBufferSize();

	//Ovrvision cameras calibration method
	//Get Oculus Right-eye Gap
	float GetOculusRightGap(int at){ return m_oculusRightGap[at]; };

private:

#ifdef WIN32
	//DirectShow
	OvrvisionDirectShow*	m_pODS[OV_CAMNUM];
#elif MACOSX
    OvrvisionAVFoundation*  m_pOAV[OV_CAMNUM];
#elif LINUX
	//NONE
#endif
	OvrvisionUndistort*		m_pUndistortEngine[OV_CAMNUM];

	//Pixels
	byte*			m_pPixels[OV_CAMNUM];
	byte*			m_pMJPEGPixels[OV_CAMNUM];

	//Camera status var
	OVR::Camprop	m_prop;
	OVR::CamStatus	m_status;
	bool			m_isNewFrame[OV_CAMNUM];
	//Camera status data
	int				m_width;
	int				m_height;
	int				m_framerate;
	//Property
	int				m_propExposure;		//Exposure
	int				m_propWhiteBalance;	//Whitebalance
	int				m_propContrast;		//Contrast
	int				m_propSaturation;	//Saturation
	int				m_propBrightness;	//Brightness
	int				m_propSharpness;	//Sharpness
	int				m_propGamma;		//Gamma
	float			m_focalPoint;		//FocalPoint
	float			m_oculusRightGap[3];	//GapVec3

	//Private method
	void InitCameraSetting(OVR::HmdProp hmd);
};

};

#endif /*__OVRVISION__*/
