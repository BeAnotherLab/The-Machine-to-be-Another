// ovrvision_ar.h
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

#ifndef __OVRVISION_AR__
#define __OVRVISION_AR__

/////////// INCLUDE ///////////

#pragma warning (disable : 4251)
#pragma warning (disable : 4290)

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
#include <vector>

//Marker 
#ifdef _OVRVISION_EXPORTS
#include <aruco.h>
#include <opencv2/opencv.hpp>
typedef aruco::MarkerDetector MarkerDetector;
typedef aruco::Marker Marker;
typedef aruco::CameraParameters CameraParameters;
typedef cv::Mat ovMat;
#else
#define MarkerDetector void
#define Marker void
#define CameraParameters void
#define ovMat void
#endif

//Left or Right camera.
#ifndef _OV_CAMEYE_ENUM_
#define _OV_CAMEYE_ENUM_
typedef enum ov_cameraeye {
	OV_CAMEYE_LEFT = 0,		//Left camera
	OV_CAMEYE_RIGHT,		//Right camera
	OV_CAMNUM,
} Cameye;
#endif

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


//Vector2D structure
typedef struct OVRPORT ov_st_vector2d {
	union {
		float v[2];
		struct {
			float x;
			float y;
		};
	};
} OvVector2D;

//Vector3D structure
typedef struct OVRPORT ov_st_vector3d {
	union {
		float v[3];
		struct {
			float x;
			float y;
			float z;
		};
	};
} OvVector3D;

//Vector4D structure
typedef struct OVRPORT ov_st_vector4d {
	union {
		float v[4];
		struct {
			float x;
			float y;
			float z;
			float w;
		};
	};
} OvVector4D;

//Marker Data
typedef struct OVRPORT ov_st_marker_data {
	int				id;					//MarkerID
	OvVector3D		translate;			//Position data
	OvVector4D		quaternion;			//Rotation data
	OvVector2D		centerPtOfImage;	//Center Position of image
} OvMarkerData;

//unsigned char to byte
typedef unsigned char byte;

//Result define
#define OV_RESULT_OK		(0)
#define OV_RESULT_FAILED	(1)

/////////// CLASS ///////////

//Ovrvision
class OVRPORT OvrvisionAR
{
public:
	//Constructor/Destructor
	//markersize_meter : 15cm = 0.15f, 1m = 1.0f
	OvrvisionAR(float markersize_meter, int w, int h, float focalPoint);
	~OvrvisionAR();

	//Methods

	//image set
	void SetImageRGB(unsigned char* pImage);
	void SetImageOpenCVImage(ovMat* pImageMat);
	//Detect marker
	void Render();				//RGB 24bit
	//Get marker data
	int				   GetMarkerDataSize();
	OVR::OvMarkerData* GetMarkerData();
	OVR::OvMarkerData* GetMarkerData(int idx);

	//Set marker size
	void SetMarkerSizeMeter(float value) { m_markerSize_Meter = value; };
	float GetMarkerSizeMeter(){ return m_markerSize_Meter; };

	void SetInstantTraking();

private:
	//Marker detector
	MarkerDetector*			m_detector;
	CameraParameters*		m_cameraParam;
	//Marker size
	float					m_markerSize_Meter;

	//Marker data
	OVR::OvMarkerData*		m_pMarkerData;
	int						m_markerDataSize;

	//Set image infomation
	byte*	m_pImageSrc;		//image pointer
	ovMat*	m_pImageOpenCVMat;	//opencv type image
	int		m_width;			//width
	int		m_height;			//height
	bool	m_isReady;

	//Private Methods
	//Rotation Matrix to Quaternion
	void RotMatToQuaternion(OvVector4D* outQuat, const float* inMat);
};

};

#endif /*__OVRVISION_AR__*/
