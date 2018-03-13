// IZwaveControlInterface.aidl
package com.askey.zwave.control;
import com.askey.zwave.control.IZwaveContrlCallBack;

// Declare any non-default types here with import statements

interface IZwaveControlInterface {

    void registerListener(IZwaveContrlCallBack callBack);
    void unRegisterListener(IZwaveContrlCallBack callBack);

    /**
     *Initialize zwave server
     */
     void openController(IZwaveContrlCallBack callBack);

    /**
     * add zwave devices
     */
    int addDevice(IZwaveContrlCallBack callBack);

    /**
    * remove zwave devices
    */
    int removeDevice(IZwaveContrlCallBack callBack);

    /**
    * get All Zwave Devices
    */
    int getDevices(IZwaveContrlCallBack callBack);

    /**
       * get Zwave Devices info
       */
    int getDeviceInfo(IZwaveContrlCallBack callBack,int deviceId);

    int removeFailedDevice(IZwaveContrlCallBack callBack,int deviceId);

    int replaceFailedDevice(IZwaveContrlCallBack callBack,int deviceId);

    int stopAddDevice(IZwaveContrlCallBack callBack);

    int stopRemoveDevice(IZwaveContrlCallBack callBack);

    int getDeviceBattery(IZwaveContrlCallBack callBack,int deviceId);

    int getSensorMultiLevel(IZwaveContrlCallBack callBack,int deviceId);

    int updateNode(IZwaveContrlCallBack callBack,int deviceId);

    int reNameDevice(IZwaveContrlCallBack callBack,String homeId,int deviceId,String newName);

    int setDefault(IZwaveContrlCallBack callBack);

    /**
    * paramMode  0 或 1
    * paramNumber 当paramMode为0时  paramNumber 值为 1,2,3,4,7,8的任意值  rangeStart rangeEnd 可以不用填写
    *             当paramMode为1时 paramNumber可以不用传值  rangeStart 1-8 的值  rangeEnd  1-8的值
    * 不用传的值目前可以默认传0
    * */
    int getConfiguration(IZwaveContrlCallBack callBack,String homeId,int deviceId, int paramMode, int paramNumber,
                                       int rangeStart, int rangeEnd);

         /**
         * paramNumber  1,2,3,4,7,8的任意值
         * paramNumber 值为 1 时是用来控制灯渐变亮的时间的， 值有0-253， 0表示瞬间变亮，1-127中的值是指根据多少秒从暗变到亮， 128-253是以分钟来计算，即，128对应1分钟，129对应2分钟....以此类推，
               * paramNumber 值为 2 控制灯变暗。。。取值和上面一样。
               *  paramNumber 值为 3 灯光最小亮度， 取值0-100
               *  paramNumber 值为 4 灯光最大亮度，取值0-100，
               *  paramNumber 值为 7 值0-255  0表示 LED off ， 254表示 status mode 255表示locator mode
               *  paramNumber 值为 8 0-2的值，0 = Incandescent  , 1 = LED , 2 = CFL ;
         * paramSize    数据长度 目前可以固定传 1
         * useDefault   0  或 1
         *  paramValue   当useDefault为0 时可以传 0-255的任意数来调节光强  0 表示关灯  255表示开灯
         *               当useDefault为1 时 可以不用传值
         * */
    int setConfiguration(IZwaveContrlCallBack callBack,String homeId,int deviceId, int paramNumber, int paramSize,
                                       int useDefault, int paramValue);

    int getSupportedSwitchType(IZwaveContrlCallBack callBack, int deviceId);

        /**
         * 控制灯泡亮暗
         * deviceId  ： nodeId
         * startLvlVal  1-99 或 255 亮度
         * duration  变化的时间 1-99 单位秒
         * pmyChangeDir  变化方向 0 变亮 1 变暗  3 不变
         * secChangeDir  0  1  3
         * secStep  1-99 或 255
         * */
    int startStopSwitchLevelChange(IZwaveContrlCallBack callBack,String homeId, int deviceId, int startLvlVal, int duration,int pmyChangeDir, int secChangeDir, int secStep);

    int getPowerLevel(IZwaveContrlCallBack callBack, int deviceId);

    int setSwitchAllOn(IZwaveContrlCallBack callBack, int deviceId);

    int setSwitchAllOff(IZwaveContrlCallBack callBack, int deviceId);

    int getBasic(IZwaveContrlCallBack callBack,int deviceId);

    int setBasic(IZwaveContrlCallBack callBack,int deviceId, int value);

    int getSwitchMultiLevel(IZwaveContrlCallBack callBack,int deviceId);

    int setSwitchMultiLevel(IZwaveContrlCallBack callBack,int deviceId, int value, int duration);

    /**
    * destroy Zawve
    */
     int closeController();


}
