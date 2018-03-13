// IZwaveContrlCallBack.aidl
package com.askey.zwave.control;

// Declare any non-default types here with import statements

interface IZwaveContrlCallBack {

    void addDeviceCallBack(String result);

    void openControlCallBack(String result,int length);

    void removeDeviceCallBack(String result);

    void getDevicesCallBack(String result);

    void getDevicesInfoCallBack(String result);

    void removeFailedDeviceCallBack(String result);

    void replaceFailedDeviceCallBack(String result);

    void stopAddDeviceCallBack(String result);

    void stopRemoveDeviceCallBack(String result);

    void getDeviceBatteryCallBack(String result);

    void getSensorMultiLevelCallBack(String result);

    void updateNodeCallBack(String result);

    void reNameDeviceCallBack(String result);

    void setDefaultCallBack(String result);

    void getConfiguration(String result);

    void setConfiguration(String result);

    void getSupportedSwitchType(String result);

    void startStopSwitchLevelChange(String result);

    void getPowerLevel(String result);

    void setSwitchAllOn(String result);

    void setSwitchAllOff(String result);

    void getBasic(String result);

    void setBasic(String result);

    void getSwitchMultiLevel(String result);

    void setSwitchMultiLevel(String result);

}