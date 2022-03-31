//***************************************************************************//
//* 版权所有  www.mediapro.cc
//*
//* 内容摘要：windows屏幕、窗口采集类 DLL封装
//*	
//* 当前版本：V1.0		
//* 作    者：mediapro
//* 完成日期：2021-09-01
//**************************************************************************//

#ifndef _SD_DESKTOP_CAPTURE_SDK_H_
#define _SD_DESKTOP_CAPTURE_SDK_H_

#ifdef __cplusplus
extern "C" {
#endif

#if defined _WIN32 || defined __CYGWIN__
  #ifdef DLL_EXPORTS
    #ifdef __GNUC__
      #define DLLIMPORT_SCREENCAP __attribute__ ((dllexport))
    #else
      #define DLLIMPORT_SCREENCAP __declspec(dllexport) 
    #endif
  #else
    #ifdef __GNUC__
      #define DLLIMPORT_SCREENCAP 
    #else
      #define DLLIMPORT_SCREENCAP 
    #endif
  #endif
#else
  #if __GNUC__ >= 4
    #define DLLIMPORT_SCREENCAP __attribute__ ((visibility ("default")))
  #else
    #define DLLIMPORT_SCREENCAP
  #endif
#endif

#ifdef WIN32
typedef signed __int64       int64_t;
#endif

#ifdef __APPLE__
#ifndef OBJC_BOOL_DEFINED
typedef int BOOL;
#endif 
#else
#ifndef BOOL
typedef int BOOL;
#endif
#endif

#ifndef TRUE
#define TRUE 1
#endif
#ifndef FALSE
#define FALSE 0
#endif



//枚举屏幕设备的最大数量
#define SD_MAX_SUPPORT_SCREEN_DEVICES_NUM				2
//枚举窗口的最大数量
#define SD_MAX_SUPPORT_WINDOW_NUM				        32
//枚举屏幕设备或窗口名称的最大长度
#define SD_MAX_SUPPORT_NAME_LEN			            	512



//日志输出的级别
typedef enum DESKTOP_CAP_LOG_OUTPUT_LEVEL
{
	DESKTOP_LOG_LEVEL_DEBUG = 1,
	DESKTOP_LOG_LEVEL_INFO,
	DESKTOP_LOG_LEVEL_WARNING,
	DESKTOP_LOG_LEVEL_ERROR,
	DESKTOP_LOG_LEVEL_ALARM,
	DESKTOP_LOG_LEVEL_FATAL,
	DESKTOP_LOG_LEVEL_NONE
} DESKTOP_CAP_LOG_OUTPUT_LEVEL;

//采集源
typedef enum DESKTOP_CAP_SOURCE
{
    DESKTOP_SOURCE_SCREEN = 0,       
    DESKTOP_SOURCE_WINDOW,
} DESKTOP_CAP_SOURCE;


//输出视频格式
typedef enum DESKTOP_CAP_OUT_FORMAT
{
    DESKTOP_OUT_FORMAT_YUV420P = 0,       
    DESKTOP_OUT_FORMAT_NV12,             
    DESKTOP_OUT_FORMAT_NV21,        
    DESKTOP_OUT_FORMAT_RGB24,
    DESKTOP_OUT_FORMAT_RGBA32
} DESKTOP_CAP_OUT_FORMAT;


//采集缩放Proset
typedef enum DESKTOP_SCALE_PRESET
{
	//对应libyuv的kFilterNone
    DESKTOP_SCALE_VERYFAST = 0,
	//对应libyuv的kFilterLinear
    DESKTOP_SCALE_FAST,
	//对应libyuv的kFilterBilinear
    DESKTOP_SCALE_MEDIUM,
	//对应libyuv的kFilterBox
    DESKTOP_SCALE_SLOW
} DESKTOP_SCALE_PRESET;


//采集状态
typedef enum DESKTOP_CAP_STATUS
{
    //初次采集成功时触发
    DESKTOP_STATUS_SUCCESS = 0,
    //临时失败，可恢复
    DESKTOP_STATUS_ERROR_TEMPORARY,
    //永久失败，不可恢复，比如窗口采集模式下窗口关闭
    DESKTOP_STATUS_ERROR_PERMANENT
} DESKTOP_CAP_STATUS;



//采集数据输出回调函数
typedef void (*CaptureDeskTopVideoCallback)(DESKTOP_CAP_OUT_FORMAT eFormat, unsigned char* pucData, unsigned int unWidth, unsigned int unHeight, void* pObject);

//采集状况对外通知回调函数
typedef void (*CaptureDeskStatusCallback)(DESKTOP_CAP_STATUS eStatus, void* pObject);


//////////////////////////////////////////////////////////////////////////
// 屏幕采集封装接口

/***
* 环境初始化，系统只需调用一次，主要用于日志模块的初始化
* @param: outputPath表示日志存放路径，支持相对路径和绝对路径，若目录不存在将自动创建
* @param: outputLevel表示日志输出的级别，只有等于或者高于该级别的日志输出到文件，取值范围参考DESKTOP_CAP_LOG_OUTPUT_LEVEL
* @return:
*/
DLLIMPORT_SCREENCAP void  SDDesktopCap_Enviroment_Init(const char* outputPath, int outputLevel);

DLLIMPORT_SCREENCAP void  SDDesktopCap_Enviroment_Free();


/***
* 枚举当前系统可用的屏幕设备(获取设备ID和设备名称)。主屏幕ID固定为0，名称为空，若只采集主屏幕或系统为单屏幕时可不调用本API
* @param: punScreenDeviceIds 设备ID存放列表，默认设备其ID为0
* @param: pScreenDeviceNames 设备名称存放列表，默认设备其名称为空
* @param: unMaxDeviceNum 最大枚举的设备数量
* @param: unMaxDeviceNameLen 最大设备名长度
* @return: 返回设备数量
*/
DLLIMPORT_SCREENCAP unsigned int SDDesktopCap_GetScreenDeviceList(unsigned int* punScreenDeviceIds, char (*pScreenDeviceNames)[SD_MAX_SUPPORT_NAME_LEN], unsigned int unMaxDeviceNum, unsigned int unMaxDeviceNameLen);


/***
* 枚举当前系统窗口信息，仅枚举当前时刻未最小化的窗口
* @param: punWindowIds 窗口ID
* @param: pWindowNames 窗口名称存放列表
* @param: unMaxWindowNum 最大枚举的窗口数量
* @param: unMaxWindowNameLen 最大窗口名长度
* @return: 返回窗口数量
*/
DLLIMPORT_SCREENCAP unsigned int SDDesktopCap_GetWindowList(unsigned int* punWindowIds, char(*pWindowNames)[SD_MAX_SUPPORT_NAME_LEN], unsigned int unMaxWindowNum, unsigned int unMaxWindowNameLen);


/***
* 创建SDDesktopCap对象
* @return: 返回模块指针，为NULL则失败
*/
DLLIMPORT_SCREENCAP void*  SDDesktopCap_New();


/***
* 销毁SDDesktopCap对象，使用者应该做好与其他API之间的互斥保护
* @param ppDesktopCap: 模块指针指针
* @return:
*/
DLLIMPORT_SCREENCAP void  SDDesktopCap_Delete(void** ppDesktopCap);



/***
* 开始启动SDDesktopCap
* @param pDesktopCap: 模块指针
* @param eCapSource: 采集源，屏幕或窗口
* @param unSourceId: 采集源ID，通过SDGetScreenDeviceList 或 SDGetWindowList获得。屏幕采集时，默认主屏幕ID为0
* @param bCapCursor: 是否采集鼠标
* @param unCapFrameRate: 采集帧率
* @param unCapWidth: 采集输出宽度，当源宽高与之不一致时，内部将缩放到输出宽高
* @param unCapHeight: 采集输出高度
* @param eCapFormat: 采集输出格式
* @param pfCapCallback: 采集输出回调接口，可以在回调中进行编码耗时操作
* @param pObject: 采集输出回调接口透传参数
* @return: TRUE成功，FALSE失败
*/
DLLIMPORT_SCREENCAP BOOL  SDDesktopCap_Start(void* pDesktopCap, DESKTOP_CAP_SOURCE eCapSource, unsigned int unSourceId, BOOL bCapCursor, unsigned int unCapFrameRate,
									unsigned int unCapWidth, unsigned int unCapHeight, DESKTOP_CAP_OUT_FORMAT eCapFormat, CaptureDeskTopVideoCallback pfCapCallback, void* pObject);



/***
* 停止SDDesktopCap
* @param pDesktopCap: 模块指针
* @return:
*/
DLLIMPORT_SCREENCAP void  SDDesktopCap_Stop(void* pDesktopCap);



/***
* 设置采集过程中需要缩放时，采用的缩放质量\速度档次，未调用本API时默认采用DESKTOP_SACLE_MEDIUM。Start之前或之后调用均可
* @param pDesktopCap: 模块指针
* @param: eScalePreset 质量\速度档次
*/
DLLIMPORT_SCREENCAP void SDDesktopCap_SetScalePreset(void* pDesktopCap, DESKTOP_SCALE_PRESET eScalePreset);
	

/***
* 设置采集状态回调函数[可选]，注意不能在回调中执行耗时操作，不能在回调中调用本模块API。Start之前或之后调用均可
* @param: pfCapStatusCallback 状态回调函数
* @param: pObject 回调接口透传参数
*/
DLLIMPORT_SCREENCAP void SDDesktopCap_SetStatusCallback(void* pDesktopCap, CaptureDeskStatusCallback pfCapStatusCallback, void* pObject);

#ifdef __cplusplus
}
#endif

#endif // _SD_DESKTOP_CAPTURE_SDK_H_
