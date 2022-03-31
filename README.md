# camera capture sdk for windows with simple api


## SDCameraCapSdk简介
本库完成Windows平台下屏幕或窗口采集功能，其支持特性如下：

* 1、当系统连接多屏幕时，可以选择指定屏幕进行采集。
* 2、当屏幕或窗口分辨率不等于用户指定输出分辨率时，自动进行保宽高比，左右或上下添加黑边，保证输出画面不变形。
* 3、支持对外输出恒定帧率，因系统性能不足时自动补帧处理，系统性能过高时自动丢帧处理。
* 4、被采集窗口关闭而无法继续采集时，持续输出最后一帧有效画面。
* 5、支持对外输出用户指定的分辨率和色度空间，可直通编码器。
* 6、内部采用高性能libYUV缩放和色度空间转换，支持设置缩放算法。
* 7、仅六个接口，调用简洁，性能强劲。
* 8、C++开发动态库，支持C、C++、C#跨语言调用

## SDCameraCapSdk  C API

### 
* 环境初始化，系统只需调用一次<br>
@param: outputPath：日志文件输出的目录，若目录不存在将自动创建<br>
@param: outputLevel：日志输出的级别，只有等于或者高于该级别的日志输出到文件<br>
@return: <br>
void  `SDDesktopCap_Enviroment_Init`(const char * outputPath,  int outputLevel);

### 
* 环境反初始化，系统只需调用一次<br>
@return:<br>
void  `SDDesktopCap_Enviroment_Free`();

### 
* 创建SDDesktopCap对象<br>
@return: 返回模块指针，为NULL则失败<br>
void*  `SDDesktopCap_New`();

### 
* 销毁SDDesktopCap,注意：【涉及到资源销毁，使用者应该做好本接口与其他接口的互斥保护】<br>
@param  ppDesktopCap: 模块指针的指针，内部销毁完后将设置为NULL<br>
@return: <br>
void  `SDDesktopCap_Delete`(void** ppDesktopCap);

### 
* 开始启动采集<br>
@param pDesktopCap: 模块指针<br>
@param eCapSource: 采集源，屏幕或窗口<br>
@param unSourceId: 采集源ID，通过SDGetScreenDeviceList 或 SDGetWindowList获得。屏幕采集时，默认主屏幕ID为0<br>
@param bCapCursor: 是否采集鼠标<br>
@param unCapFrameRate: 采集帧率<br>
@param unCapWidth: 采集输出宽度，当源宽高与之不一致时，内部将缩放到输出宽高<br>
@param unCapHeight: 采集输出高度<br>
@param eCapFormat: 采集输出格式<br>
@param pfCapCallback: 采集输出回调接口，可以在回调中进行编码耗时操作<br>
@param pObject: 采集输出回调接口透传参数<br>
@return: TRUE成功，FALSE失败<br>
BOOL  `SDDesktopCap_Start`(void* pDesktopCap, DESKTOP_CAP_SOURCE eCapSource, unsigned int unSourceId, BOOL bCapCursor, unsigned int unCapFrameRate,
									unsigned int unCapWidth, unsigned int unCapHeight, DESKTOP_CAP_OUT_FORMAT eCapFormat, CaptureDeskTopVideoCallback pfCapCallback, void* pObject);

### 
* 停止采集<br>
@param pDesktopCap: 模块指针<br>
@return: <br>
void  `SDDesktopCap_Stop`(void* pDesktopCap);

### 
* 枚举当前系统可用的屏幕设备(获取设备ID和设备名称)。主屏幕ID固定为0，名称为空。若只采集主屏幕或系统为单屏幕时可不调用本API，直接指定ID为0<br>
@param: punScreenDeviceIds 设备ID存放列表，默认设备其ID为0<br>
@param: pScreenDeviceNames 设备名称存放列表，默认设备其名称为空<br>
@param: unMaxDeviceNum 最大枚举的设备数量<br>
@param: unMaxDeviceNameLen 最大设备名长度<br>
@return: 返回设备数量<br>
unsigned int  `SDDesktopCap_GetScreenDeviceList`(unsigned int* punScreenDeviceIds, char (*pScreenDeviceNames)[SD_MAX_SUPPORT_NAME_LEN], unsigned int unMaxDeviceNum, unsigned int unMaxDeviceNameLen);


### 
* 枚举当前系统窗口信息，仅枚举当前时刻未最小化的窗口<br>
@param: punWindowIds 窗口ID<br>
@param: pWindowNames 窗口名称存放列表<br>
@param: unMaxWindowNum 最大枚举的窗口数量<br>
@param: unMaxWindowNameLen 最大窗口名长度<br>
@return: 返回窗口数量<br>
unsigned int  `SDDesktopCap_GetWindowList`(unsigned int* punWindowIds, char(*pWindowNames)[SD_MAX_SUPPORT_NAME_LEN], unsigned int unMaxWindowNum, unsigned int unMaxWindowNameLen);


### 
* 设置采集过程中需要缩放时，采用的缩放质量\速度档次，未调用本API时默认采用DESKTOP_SACLE_MEDIUM。Start之前或之后调用均可
@param pDesktopCap: 模块指针
@param: eScalePreset 质量\速度档次
*/
void `SDDesktopCap_SetScalePreset`(void* pDesktopCap, DESKTOP_SCALE_PRESET eScalePreset);


### 本库可免费使用，若需要商用或定制服务请联系 www.mediapro.cc
