package com.askey.firefly.zwave.control.service;

import android.Manifest;
import android.annotation.TargetApi;
import android.app.Activity;
import android.app.AlertDialog;
import android.app.Service;
import android.content.DialogInterface;
import android.content.Intent;
import android.content.pm.PackageManager;
import android.os.Build;
import android.os.IBinder;
import android.os.RemoteCallbackList;
import android.os.RemoteException;
import android.provider.Settings;
import android.support.annotation.Nullable;
import android.support.v4.app.ActivityCompat;
import android.support.v4.content.ContextCompat;
import android.text.LoginFilter;
import android.util.Log;
import android.os.Process;

import com.askey.firefly.zwave.control.bean.DeviceList;
import com.askey.firefly.zwave.control.dao.ImportData;
import com.askey.firefly.zwave.control.dao.ZwaveDevice;
import com.askey.firefly.zwave.control.dao.ZwaveDeviceManager;
import com.askey.firefly.zwave.control.jni.ZwaveControlHelper;
import com.askey.firefly.zwave.control.thirdparty.usbserial.UsbSerial;
import com.askey.firefly.zwave.control.utils.Logg;
import com.askey.zwave.control.IZwaveContrlCallBack;
import com.askey.zwave.control.IZwaveControlInterface;
import com.google.gson.Gson;

import org.json.JSONArray;
import org.json.JSONException;
import org.json.JSONObject;

import java.util.List;
import java.util.Timer;
import java.util.TimerTask;
import android.os.Trace;

import static android.R.id.message;
import static com.askey.firefly.zwave.control.utils.Const.DEBUG;
import static com.askey.firefly.zwave.control.utils.Const.ZWCONTROL_CFG_PATH;
import static com.askey.firefly.zwave.control.utils.Const.FILE_PATH;
import static com.askey.firefly.zwave.control.utils.Const.SAVE_NODEINFO_FILE;
import com.askey.firefly.zwave.control.application.ZwaveProvisionList;
/**
 * 项目名称：ZwaveControl
 * 类描述：
 * 创建人：skysoft  charles.bai
 * 创建时间：2017/7/10 11:36
 * 修改人：skysoft
 * 修改时间：2017/7/10 11:36
 * 修改备注：
 */
public class ZwaveControlService extends Service {


    private final String TAG = "ZwaveControlService";
    private RemoteCallbackList<IZwaveContrlCallBack> mCallbacks = new RemoteCallbackList<>();
    private UsbSerial mUsbSerial = new UsbSerial(this);
    private IZwaveContrlCallBack mCallBack;
    private static ZwaveControlService mService;
    private int flag;
    private int nodeId;
    private ZwaveDeviceManager zwaveDeviceManager;

    @Override
    public void onCreate() {
        super.onCreate();
        mService = this;
        ImportData.importFile(this,"app.cfg");
        ImportData.importFile(this,"zipgateway.cfg");
        ImportData.importFile(this,"Portal.ca_x509.pem");
        ImportData.importFile(this,"ZIPR.key_1024.pem");
        ImportData.importFile(this,"ZIPR.x509_1024.pem");
        ImportData.importFile(this,"zwave_device_rec.txt");
        ImportData.importFile(this,"zipgateway_provisioning_list.cfg");
        ImportData.importFile(this,"cmd_class.cfg");
        ImportData.importDatabase(this);
        zwaveDeviceManager = ZwaveDeviceManager.getInstance(this);
//        //test
//        ZwaveDevice zwaveDevice = new ZwaveDevice();
//        zwaveDevice.setHomeId("23456");
//        zwaveDevice.setAddress("12");
//        zwaveDevice.setName("sdfsdf");
//        zwaveDevice.setType("4234");
//        zwaveDevice.setNodeId(1);
//        ZwaveDeviceManager.getInstance(ZwaveControlService.this).insertZwaveDevice(zwaveDevice);


    }
public static ZwaveControlService getInstance() {
    if (mService != null) {
        return mService;
    }
    return null;
}

    @Override
    public IBinder onBind(Intent intent) {
        Logg.i(TAG, "=====onBind=========");

        int creatResult = ZwaveControlHelper.CreateZwController(); //测试返回0
        if(creatResult==0){
            Logg.i(TAG,"==CreateZwController=creatResult="+creatResult);
        }else{
            Logg.e(TAG,"==CreateZwController=creatResult="+creatResult);
        }
        return remoteBinder;
    }

    @Override
    public int onStartCommand(Intent intent, int flags, int startId) {
        intent = new Intent("android.intent.action.bootreceiver");
        sendBroadcast(intent);
        return super.onStartCommand(intent, flags, startId);
    }

    @Override
    public void onDestroy() {
        Logg.i(TAG, "=====onDestroy=========");
        Intent intent = new Intent("android.intent.action.destroy");
        sendBroadcast(intent);
        mCallbacks.kill();
        super.onDestroy();
    }
    private IZwaveControlInterface.Stub remoteBinder = new IZwaveControlInterface.Stub() {

        @Override
        public void registerListener(IZwaveContrlCallBack callBack) throws RemoteException {
            mCallbacks.register(callBack);
            mCallBack = callBack;
        }

        @Override
        public void unRegisterListener(IZwaveContrlCallBack callBack) throws RemoteException {
            mCallbacks.unregister(callBack);
            mCallBack = null;
        }

        @Override
        public void openController(IZwaveContrlCallBack callBack) throws RemoteException {
           doOpenController(callBack);
        }


        @Override
        public int addDevice(IZwaveContrlCallBack callBack) throws RemoteException {
            /*String str = "36494-17891-09522-29717-11896-35740-65300-09870";
            byte[] bstr = str.getBytes();
            byte[] dsk = new byte[str.length()+1];

            for(int i = 0; i < bstr.length; ++i)
                dsk[i] = bstr[i];
            dsk[str.length()] = '\0';*/
            byte[] dsk = new byte[6];
            dsk[0] = '4';
            dsk[1] = '4';
            dsk[2] = '7';
            dsk[3] = '0';
            dsk[4] = '2';
            dsk[5] = '\0';
            int result =  ZwaveControlHelper.ZwController_AddDevice();
            //int result = 0;//ZwaveControlHelper.ZwController_StartLearnMode();
            return result;
        }

        @Override
        public int removeDevice(IZwaveContrlCallBack callBack) throws RemoteException {
            //Thread.currentThread().dumpStack();
            //android.util.Log.d(TAG,"LinHui", new Throwable());
            //new RuntimeException("LinHui").printStackTrace();
            //Process.sendSignal(android.os.Process.myPid(), Process.SIGNAL_QUIT);
            //Trace.beginSection("app:ZwController_RemoveDevice");
            long startTime = System.currentTimeMillis();
            int result = ZwaveControlHelper.ZwController_RemoveDevice();
            //Trace.endSection();
            long spendTime = System.currentTimeMillis() - startTime;
            Log.i(TAG,"took " + spendTime + " ms");
            return result;
        }

        @Override
        public int getDevices(IZwaveContrlCallBack callBack) throws RemoteException {
            int result = ZwaveControlHelper.ZwController_GetDeviceInfo();
            return result;
        }

        @Override
        public int getDeviceInfo(IZwaveContrlCallBack callBack, int deviceId) throws RemoteException {
            flag = 1;
            nodeId = deviceId;
            int result = ZwaveControlHelper.ZwController_GetDeviceList();
            return result;
        }

        @Override
        public int removeFailedDevice(IZwaveContrlCallBack callBack, int deviceId) throws RemoteException {
            int result = ZwaveControlHelper.ZwController_RemoveFailedDevice(deviceId);
            return result;
        }

        @Override
        public int replaceFailedDevice(IZwaveContrlCallBack callBack, int deviceId) throws RemoteException {
            byte[] dsk = new byte[200];
            int result = ZwaveControlHelper.ZwController_ReplaceFailedDevice(deviceId, dsk, 0);
            return result;
        }

        @Override
        public int stopAddDevice(IZwaveContrlCallBack callBack) throws RemoteException {
            int result = ZwaveControlHelper.ZwController_StopAddDevice();
            return result;
        }

        @Override
        public int stopRemoveDevice(IZwaveContrlCallBack callBack) throws RemoteException {
            int result = ZwaveControlHelper.ZwController_StopRemoveDevice();
            return result;
        }

        @Override
        public int getDeviceBattery(IZwaveContrlCallBack callBack, int deviceId) throws RemoteException {
            Logg.i(TAG,"=====getDeviceBattery==deviceId==="+deviceId);
            int result = ZwaveControlHelper.ZwController_GetSwitchMultiLevel(deviceId);

            return result;
        }

        @Override
        public int getSensorMultiLevel(IZwaveContrlCallBack callBack,int deviceId) throws RemoteException {
            /*Logg.i(TAG,"=====getSensorMultiLevel==deviceId==="+deviceId);
            String str = "55106-03713-41807-09806-27111-48391-14810-50406";
            byte[] bstr = str.getBytes();
            byte[] dsk = new byte[str.length()+1];

            for(int i = 0; i < bstr.length; ++i)
                dsk[i] = bstr[i];
            dsk[str.length()] = '\0';
            //int result = ZwaveControlHelper.ZwController_addProvisionListEntry(dsk, dsk.length);

            try{
                Thread.sleep(2000);
            }catch (InterruptedException e)
            {
                e.printStackTrace();
            }

            String str1 = "45106-03713-41807-09806-27111-48391-14810-50406";
            byte[] bstr1 = str1.getBytes();
            byte[] dsk1 = new byte[str1.length()+1];

            for(int i = 0; i < bstr.length; ++i)
                dsk1[i] = bstr1[i];
            dsk1[str1.length()] = '\0';
            result = ZwaveControlHelper.ZwController_addProvisionListEntry(dsk1, dsk1.length);*/
            //int result = ZwaveControlHelper.ZwController_getSpecifyDeviceInfo(deviceId);
            int result = ZwaveControlHelper.ZwController_getDoorLockConfiguration(deviceId);
            return result;
        }

        @Override
        public int updateNode(IZwaveContrlCallBack callBack,int deviceId) throws RemoteException {
            Logg.i(TAG,"=====updateNode==deviceId==="+deviceId);
            //int result = ZwaveControlHelper.ZwController_UpdateNode(deviceId);
            String str = "55106-03713-41807-09806-27111-48391-14810-50406";
            byte[] bstr = str.getBytes();
            byte[] dsk = new byte[str.length()+1];
            int i = 0;
            for(i = 0; i < bstr.length; ++i)
                dsk[i] = bstr[i];
            dsk[str.length()] = '\0';

            //int result = ZwaveControlHelper.ZwController_rmProvisionListEntry(dsk, dsk.length);
            //int result = ZwaveControlHelper.ZwController_getAllProvisionListEntry();

            ZwaveProvisionList[] plList = new ZwaveProvisionList[6];
            if(plList == null){
                Log.i(TAG,"======================");
            }

            for(i = 0; i < 6; i++)
            {
                plList[i] = new ZwaveProvisionList();
            }

            plList[0].setType(ZwaveProvisionList.PL_INFO_TYPE_INCL_STS);

            plList[0].setInclusionState(ZwaveProvisionList.PL_INCL_STS_PENDING);

            plList[1].setType(ZwaveProvisionList.PL_INFO_TYPE_BOOT_MODE);
            plList[1].setBootMode(ZwaveProvisionList.PL_BOOT_MODE_SMART_STRT);

            plList[2].setType(ZwaveProvisionList.PL_INFO_TYPE_NAME);
            plList[2].stProvisionList.name = "skysoft";

            plList[3].setType(ZwaveProvisionList.PL_INFO_TYPE_LOC);
            plList[3].stProvisionList.loc = "complany";

            plList[4].setType(ZwaveProvisionList.PL_INFO_TYPE_PROD_TYPE);
            plList[4].stProvisionList.pti.generic_cls = 0x10;
            plList[4].stProvisionList.pti.specific_cls = 0x01;
            plList[4].stProvisionList.pti.icon_type = 0x0700;

            plList[5].setType(ZwaveProvisionList.PL_INFO_TYPE_PROD_ID);
            plList[5].stProvisionList.pii.manf_id = 0;
            plList[5].stProvisionList.pii.prod_type = 0x0003;
            plList[5].stProvisionList.pii.prod_id = 0x0002;
            plList[5].stProvisionList.pii.app_ver = 0x04;
            plList[5].stProvisionList.pii.app_sub_ver = 0x01;

            int result = ZwaveControlHelper.ZwController_addProvisionListEntry(dsk, dsk.length, plList, 6);
            return result;
        }

        @Override
        public int reNameDevice(IZwaveContrlCallBack callBack, String homeId, int deviceId,String newName) throws RemoteException {
            //数据库
            Logg.i(TAG,"=====reNameDevice==homeId==="+homeId +"=deviceId=="+deviceId +"=newName=="+newName);
            updateName(homeId,deviceId,newName);
            return 0;
        }

        @Override
        public int setDefault(IZwaveContrlCallBack callBack) throws RemoteException {
            setDefaultCallBack("");
            return 0;
        }

        @Override
        public int getConfiguration(IZwaveContrlCallBack callBack, String homeId, int deviceId, int paramMode, int paramNumber, int rangeStart, int rangeEnd) throws RemoteException {
            int result = ZwaveControlHelper.ZwController_GetConfiguration(deviceId, paramMode, paramNumber, rangeStart, rangeEnd);
            return result;
        }

        @Override
        public int setConfiguration(IZwaveContrlCallBack callBack, String homeId, int deviceId, int paramNumber, int paramSize, int useDefault, int paramValue) throws RemoteException {
            int result = ZwaveControlHelper.ZwController_SetConfiguration(deviceId, paramNumber, paramSize, useDefault, paramValue);
            setConfigurationCallBack(String.valueOf(result));
            return result;
        }

        @Override
        public int getSupportedSwitchType(IZwaveContrlCallBack callBack, int deviceId) throws RemoteException {
            int result = ZwaveControlHelper.ZwController_GetSupportedSwitchType(deviceId);
            return result;
        }

        @Override
        public int startStopSwitchLevelChange(IZwaveContrlCallBack callBack,String homeId, int deviceId, int startLvlVal, int duration, int pmyChangeDir, int secChangeDir, int secStep) throws RemoteException {
            int result = ZwaveControlHelper.ZwController_startStopSwitchLevelChange(deviceId,startLvlVal,duration,pmyChangeDir,secChangeDir,secStep);
            startStopSwitchLevelChangeCallBack(String.valueOf(result));
            return result;
        }

        @Override
        public int getPowerLevel(IZwaveContrlCallBack callBack, int deviceId) throws RemoteException {
            int result = ZwaveControlHelper.ZwController_rmAllProvisionListEntry();
            return result;
        }

        public int time = 0x10; 

        @Override
        public int setSwitchAllOn(IZwaveContrlCallBack callBack, int deviceId) throws RemoteException {
            int result = ZwaveControlHelper.ZwController_SetSwitchAllOn(deviceId);
            setSwitchAllOnCallBack(String.valueOf(result));
            return result;
        }

        @Override
        public int setSwitchAllOff(IZwaveContrlCallBack callBack, int deviceId) throws RemoteException {
            int result = ZwaveControlHelper.ZwController_SetSwitchAllOff(deviceId);
            setSwitchAllOffCallBack(String.valueOf(result));
            return result;
        }

        @Override
        public int getBasic(IZwaveContrlCallBack callBack,int deviceId) throws RemoteException {
            int result = ZwaveControlHelper.ZwController_GetBasic(deviceId);
            return result;
        }

        @Override
        public int setBasic(IZwaveContrlCallBack callBack,int deviceId, int value) throws RemoteException {
            int result = ZwaveControlHelper.ZwController_SetBasic(deviceId,value);
            setBasicCallBack(String.valueOf(result));
            return result;
        }

        @Override
        public int getSwitchMultiLevel(IZwaveContrlCallBack callBack, int deviceId) throws RemoteException {
            int result = ZwaveControlHelper.ZwController_GetSwitchMultiLevel(deviceId);
            return result;
        }

        @Override
        public int setSwitchMultiLevel(IZwaveContrlCallBack callBack, int deviceId, int value, int duration) throws RemoteException {
            int result = ZwaveControlHelper.ZwController_SetSwitchMultiLevel(deviceId, value, duration);
            setSwitchMultiLevelCallBack(String.valueOf(result));
            return result;
        }

        @Override
        public int closeController() throws RemoteException {
            int result = ZwaveControlHelper.CloseZwController();
            return result;
        }

    };

    private void setBasicCallBack(String result) {

        String setBasicResult = "setBasic:" + result;
        final int N = mCallbacks.beginBroadcast();
        try {
            //add finish
            Logg.i(TAG, "=====setBasicCallBack=========");

            Log.d("ZwaveControlService", "ZwaveControlService " + result);
            for (int i = 0;i < N;i++) {
                mCallbacks.getBroadcastItem(i).setBasic(setBasicResult);
            }

        } catch (Exception e) {
            Logg.i(TAG, "=====Exception=========" + e.toString());
            e.printStackTrace();
        }
        mCallbacks.finishBroadcast();
    }



    private void updateName(String homeId, int deviceId, String newName) {
        int result = -1;
       ZwaveDevice zwaveDevice = zwaveDeviceManager.queryZwaveDevices(homeId, deviceId);
        if (zwaveDevice != null) {
            zwaveDevice.setName(newName);
            zwaveDeviceManager.updateZwaveDevice(zwaveDevice);
            result = 0;
        } else {
            result = -1;
        }
        String reNameResult = "reNameDevice:"+result;
        reNameDeviceCallBack(reNameResult);
    }

    private void reNameDeviceCallBack(String result) {

        final int N = mCallbacks.beginBroadcast();
        try {
            //add finish
            Logg.i(TAG, "=====updateNameCallBack=========");

            Log.d("ZwaveControlService", "ZwaveControlService " + result);
            for (int i = 0;i < N;i++) {
                mCallbacks.getBroadcastItem(i).reNameDeviceCallBack(result);
            }

        } catch (Exception e) {
            Logg.i(TAG, "=====Exception=========" + e.toString());
            e.printStackTrace();
        }
        mCallbacks.finishBroadcast();
    }


    //添加设备
       private void doAddDevice(String result) {
           Logg.i(TAG, "=====doAddDevice=========");

               final int N = mCallbacks.beginBroadcast();
        try {
            //add finish
            for (int i = 0;i < N;i++) {
                mCallbacks.getBroadcastItem(i).addDeviceCallBack(result);
            }
        } catch (Exception e) {
            e.printStackTrace();
            Logg.i(TAG, "=====Exception=========" + e.toString());
        }
        mCallbacks.finishBroadcast();
    }

    //删除设备
       private void doRemoveDevice(String result) {

        final int N = mCallbacks.beginBroadcast();
        try {
            Logg.i(TAG, "=====doRemoveDevice=========");
            //remove finish
            for (int i = 0;i < N;i++) {
                mCallbacks.getBroadcastItem(i).removeDeviceCallBack(result);
            }
        } catch (Exception e) {
            Logg.i(TAG, "=====Exception=========" + e.toString());
            e.printStackTrace();
        }
        mCallbacks.finishBroadcast();
    }

    //

       private void doGetDeviceList(String result) {
           Logg.i(TAG, "=====doGetDeviceList=========");
        final int N = mCallbacks.beginBroadcast();
        try {

            //get Device finish
            for (int i = 0;i < N;i++) {
                mCallbacks.getBroadcastItem(i).getDevicesCallBack(result);
            }
        } catch (Exception e) {
            Logg.i(TAG, "=====Exception=========" + e.toString());
            e.printStackTrace();
        }
        mCallbacks.finishBroadcast();
    }

    private void removeFailCallBack(String result) {

        Logg.i(TAG, "=====removeFailCallBack=========");
        final int N = mCallbacks.beginBroadcast();
        try {

            //get Device finish
            for (int i = 0;i < N;i++) {
                mCallbacks.getBroadcastItem(i).removeFailedDeviceCallBack(result);
            }
        } catch (Exception e) {
            Logg.i(TAG, "=====Exception=========" + e.toString());
            e.printStackTrace();
        }
        mCallbacks.finishBroadcast();
    }
    private void stopAddDeviceCallBack(String result) {

        Logg.i(TAG, "=====stopAddDeviceCallBack=========");
        final int N = mCallbacks.beginBroadcast();
        try {

            //get Device finish
            for (int i = 0;i < N;i++) {
                mCallbacks.getBroadcastItem(i).stopAddDeviceCallBack(result);
            }
        } catch (Exception e) {
            Logg.i(TAG, "=====Exception=========" + e.toString());
            e.printStackTrace();
        }
        mCallbacks.finishBroadcast();
    }
    private void stopRemoveDeviceCallBack(String result) {

        Logg.i(TAG, "=====stopRemoveDeviceCallBack=========");
        final int N = mCallbacks.beginBroadcast();
        try {

            //get Device finish
            for (int i = 0;i < N;i++) {
                mCallbacks.getBroadcastItem(i).stopRemoveDeviceCallBack(result);
            }
        } catch (Exception e) {
            Logg.i(TAG, "=====Exception=========" + e.toString());
            e.printStackTrace();
        }
        mCallbacks.finishBroadcast();
    }
    private void getDeviceBatteryCallBack(String result) {

        Logg.i(TAG, "=====getDeviceBatteryCallBack=========");
        final int N = mCallbacks.beginBroadcast();
        try {


            //get Device finish
            for (int i = 0;i < N;i++) {
                mCallbacks.getBroadcastItem(i).getDeviceBatteryCallBack(result);
            }
        } catch (Exception e) {
            Logg.i(TAG, "=====Exception=========" + e.toString());
            e.printStackTrace();
        }
        mCallbacks.finishBroadcast();
    }
    private void getSensorMultiLevelCallBack(String result) {

        Logg.i(TAG, "=====getSensorMultiLevelCallBack=========");
        final int N = mCallbacks.beginBroadcast();
        try {

            //get Device finish
            for (int i = 0;i < N;i++) {
                mCallbacks.getBroadcastItem(i).getSensorMultiLevelCallBack(result);
            }
        } catch (Exception e) {
            Logg.i(TAG, "=====Exception=========" + e.toString());
            e.printStackTrace();
        }
        mCallbacks.finishBroadcast();
    }
    private void updateNodeCallBack(String result) {

        Logg.i(TAG, "=====updateNodeCallBack=========");
        final int N = mCallbacks.beginBroadcast();
        try {

            //get Device finish
            for (int i = 0;i < N;i++) {
                mCallbacks.getBroadcastItem(i).updateNodeCallBack(result);
            }
        } catch (Exception e) {
            Logg.i(TAG, "=====Exception=========" + e.toString());
            e.printStackTrace();
        }
        mCallbacks.finishBroadcast();
    }
    private void setDefaultCallBack(String str) {

        int result = ZwaveControlHelper.ZwController_SetDefault();
        final int N = mCallbacks.beginBroadcast();
        try {

            String resultDefault = "setDefault:"+result;
            Logg.i(TAG, "=====resultDefault========="+resultDefault);
            //get Device finish
            for (int i = 0;i < N;i++) {
                mCallbacks.getBroadcastItem(i).setDefaultCallBack(resultDefault);
            }
        } catch (Exception e) {
            Logg.i(TAG, "=====Exception=========" + e.toString());
            e.printStackTrace();
        }
        mCallbacks.finishBroadcast();
    }
    private void replaceFailCallBack(String result) {

        Logg.i(TAG, "=====replaceFailCallBack=========");
        final int N = mCallbacks.beginBroadcast();
        try {

            //get Device finish
            for (int i = 0;i < N;i++) {
                mCallbacks.getBroadcastItem(i).replaceFailedDeviceCallBack(result);
            }
        } catch (Exception e) {
            Logg.i(TAG, "=====Exception=========" + e.toString());
            e.printStackTrace();
        }
        mCallbacks.finishBroadcast();
    }
    private void doGetDeviceInfo(String result) {

        final int N = mCallbacks.beginBroadcast();
        try {
            Logg.i(TAG, "=====doGetDeviceInfo=========");
            //get Device finish
            for (int i = 0;i < N;i++) {
                mCallbacks.getBroadcastItem(i).getDevicesInfoCallBack(result);
            }
        } catch (Exception e) {
            Logg.i(TAG, "=====Exception=========" + e.toString());
            e.printStackTrace();
        }
        mCallbacks.finishBroadcast();
    }

    private void setSwitchAllOffCallBack(String result) {
        Logg.i(TAG, "=====setSwitchAllOff=========");
        String onResult = "setSwitchAllOff:" + result;
        final int N = mCallbacks.beginBroadcast();
        try {

            //get Device finish
            for (int i = 0;i < N;i++) {
                mCallbacks.getBroadcastItem(i).setSwitchAllOff(onResult);
            }
        } catch (Exception e) {
            Logg.i(TAG, "=====Exception=========" + e.toString());
            e.printStackTrace();
        }
        mCallbacks.finishBroadcast();
    }

    private void setSwitchAllOnCallBack(String result) {
        Logg.i(TAG, "=====setSwitchAllOn=========");
        String onResult = "setSwitchAllOn:" + result;
        final int N = mCallbacks.beginBroadcast();
        try {

            //get Device finish
            for (int i = 0;i < N;i++) {
                mCallbacks.getBroadcastItem(i).setSwitchAllOn(onResult);
            }
        } catch (Exception e) {
            Logg.i(TAG, "=====Exception=========" + e.toString());
            e.printStackTrace();
        }
        mCallbacks.finishBroadcast();

    }

    private void getPowerLevel(String result) {
        Logg.i(TAG, "=====getPowerLevel=========");
        final int N = mCallbacks.beginBroadcast();
        try {

            //get Device finish
            for (int i = 0;i < N;i++) {
                mCallbacks.getBroadcastItem(i).setSwitchAllOn(result);
            }
        } catch (Exception e) {
            Logg.i(TAG, "=====Exception=========" + e.toString());
            e.printStackTrace();
        }
        mCallbacks.finishBroadcast();
    }

    private void startStopSwitchLevelChangeCallBack(String result) {
        Logg.i(TAG, "=====startStopSwitchLevelChange=========");
        String levelChangeResult = "startStopSwitchLevelChange:" + result;
        final int N = mCallbacks.beginBroadcast();
        try {

            //get Device finish
            for (int i = 0;i < N;i++) {
                mCallbacks.getBroadcastItem(i).startStopSwitchLevelChange(levelChangeResult);
            }
        } catch (Exception e) {
            Logg.i(TAG, "=====Exception=========" + e.toString());
            e.printStackTrace();
        }
        mCallbacks.finishBroadcast();
    }

    private void getSupportedSwitchType(String result) {
        Logg.i(TAG, "=====getSupportedSwitchType=========");
        final int N = mCallbacks.beginBroadcast();
        try {

            //get Device finish
            for (int i = 0;i < N;i++) {
                mCallbacks.getBroadcastItem(i).getSupportedSwitchType(result);
            }
        } catch (Exception e) {
            Logg.i(TAG, "=====Exception=========" + e.toString());
            e.printStackTrace();
        }
        mCallbacks.finishBroadcast();
    }
    private void getBasic(String result) {
        Logg.i(TAG, "=====getBasic=========");
        final int N = mCallbacks.beginBroadcast();
        try {

            //get Device finish
            for (int i = 0;i < N;i++) {
                mCallbacks.getBroadcastItem(i).getBasic(result);
            }
        } catch (Exception e) {
            Logg.i(TAG, "=====Exception=========" + e.toString());
            e.printStackTrace();
        }
        mCallbacks.finishBroadcast();
    }

    private void setSwitchMultiLevelCallBack(String result) {
        Logg.i(TAG, "=====setSwitchMultiLevel=========");
        String onResult = "setSwitchMultiLevel:" + result;
        final int N = mCallbacks.beginBroadcast();
        try {

            //get Device finish
            for (int i = 0;i < N;i++) {
                mCallbacks.getBroadcastItem(i).setSwitchMultiLevel(onResult);
            }
        } catch (Exception e) {
            Logg.i(TAG, "=====Exception=========" + e.toString());
            e.printStackTrace();
        }
        mCallbacks.finishBroadcast();

    }

    private void getSwitchMultiLevel(String result) {
        Logg.i(TAG, "=====getSwitchMultiLevel=========");
        final int N = mCallbacks.beginBroadcast();
        try {

            //get Device finish
            for (int i = 0;i < N;i++) {
                mCallbacks.getBroadcastItem(i).getSwitchMultiLevel(result);
            }
        } catch (Exception e) {
            Logg.i(TAG, "=====Exception=========" + e.toString());
            e.printStackTrace();
        }
        mCallbacks.finishBroadcast();
    }

    public void doOpenController(IZwaveContrlCallBack callBack) {
        Logg.i(TAG, "=====doOpenController=========");
        if (mCallBack == null) {
            return;
        }
        byte[] result = new byte[500];
        int isOK = ZwaveControlHelper.OpenZwController(FILE_PATH, SAVE_NODEINFO_FILE, result);
//        if (isOK == 0) {
//            insertHomeDevice(new String(result));
//        }
        Logg.i(TAG,"===isOK=="+isOK);
        String openResult = "openController:"+isOK;
        Log.d("ZwaveControlService", "ZwaveControlService " + new String(result) + isOK);
        final int N = mCallbacks.beginBroadcast();
        try {
            for (int i = 0;i < N;i++) {
                mCallbacks.getBroadcastItem(i).openControlCallBack(openResult,result.length);
            }
        } catch (Exception e) {
            Logg.i(TAG, "=====Exception=========" + e.toString());
            e.printStackTrace();
        }
        mCallbacks.finishBroadcast();
    }

    private void insertHomeDevice(String result) {
        try {
            JSONObject json = new JSONObject(result);
           String homeId = json.optString("Home Id");
           int nodeId = json.optInt("Node Id");
            ZwaveDevice homeDevice = zwaveDeviceManager.queryZwaveDevices(homeId, nodeId);
            if (homeDevice == null) {
                if (!homeId.toString().trim().equals("") && nodeId == 1) {
                    ZwaveDevice device = new ZwaveDevice();
                    device.setHomeId(homeId);
                    device.setNodeId(nodeId);
                    device.setName(String.valueOf(nodeId));
                    zwaveDeviceManager.insertZwaveDevice(device);
                }
            }
        } catch (JSONException e) {
            e.printStackTrace();
        }
    }

    //开关
    private void getConfiguration(String result) {

        Logg.i(TAG, "=====getConfiguration=========");
        final int N = mCallbacks.beginBroadcast();
        try {
            for (int i = 0;i < N;i++) {
                mCallbacks.getBroadcastItem(i).getConfiguration(result);
            }
        } catch (Exception e) {
            Logg.i(TAG, "=====Exception=========" + e.toString());
            e.printStackTrace();
        }
        mCallbacks.finishBroadcast();
    }

    //开关
    private void setConfigurationCallBack(String result) {

        Logg.i(TAG, "=====setConfiguration=========");
        String configResult = "setConfiguration:" + result;
        final int N = mCallbacks.beginBroadcast();
        try {
            for (int i = 0;i < N;i++) {
                mCallbacks.getBroadcastItem(i).setConfiguration(configResult);
            }
        } catch (Exception e) {
            Logg.i(TAG, "=====Exception=========" + e.toString());
            e.printStackTrace();
        }
        mCallbacks.finishBroadcast();
    }

    public void zwaveCallBack(byte[] result, int len)  {
        // jni callback

        String jniResult = new String(result);
        Logg.i(TAG,"jniResult==="+jniResult);
        String messageType = null;
        String status = null;
        try {
            JSONObject jsonObject = new JSONObject(jniResult);
            messageType = jsonObject.optString("MessageType");
            status = jsonObject.optString("Status");

        } catch (JSONException e) {
            e.printStackTrace();
    }
    if ("Node Add Status".equals(messageType)) {
        doAddDevice(jniResult);
        if ("Success".equals(status)) {
           flag = 2;
            Logg.i(TAG,"=======Node Add Status=Success=");
            ZwaveControlHelper.ZwController_saveNodeInfo(SAVE_NODEINFO_FILE);
            ZwaveControlHelper.ZwController_GetDeviceInfo();
        }
    } else if ("Node Remove Status".equals(messageType)) {
        doRemoveDevice(jniResult);
        if ("Success".equals(status)) {
            Logg.i(TAG,"=======Node Remove Status=Success=");
            flag = 3;
            ZwaveControlHelper.ZwController_saveNodeInfo(SAVE_NODEINFO_FILE);
            ZwaveControlHelper.ZwController_GetDeviceList();
        }
    } else if ("Node List Report".equals(messageType)) {
        if (flag == 1) {
            flag = 0;
            doGetDeviceInfo(getDeviceInfo(jniResult));
        } else if (flag == 0) {
           String jsonResult = getDeviceList(jniResult);//json 添加name
            doGetDeviceList(jsonResult);
        } else if (flag == 2) {
            flag = 0;
            insertDevice(jniResult);
        } else if (flag == 3) {
            flag = 0;
            deleteDevice(jniResult);
        }
    } else if ("Node List Report".equals(messageType)) {/////
        removeFailCallBack(jniResult);
        if(jniResult.contains("Success")){
            ZwaveControlHelper.ZwController_saveNodeInfo(SAVE_NODEINFO_FILE);
        }
    } else if ("Node List Report".equals(messageType)) {/////
        replaceFailCallBack(jniResult);
        if(jniResult.contains("Success")){
            ZwaveControlHelper.ZwController_saveNodeInfo(SAVE_NODEINFO_FILE);
        }
    } else if ("Node List Report".equals(messageType)) {/////
        stopAddDeviceCallBack(jniResult);
    } else if ("Node List Report".equals(messageType)) {/////
        stopRemoveDeviceCallBack(jniResult);
    } else if ("Node Battery Value".equals(messageType)) {
        getDeviceBatteryCallBack(jniResult);
    } else if ("Sensor Information".equals(messageType)) {
        getSensorMultiLevelCallBack(jniResult);
    } else if ("Node List Report".equals(messageType)) {/////
        updateNodeCallBack(jniResult);
    } else if ("Configuration Get Information".equals(messageType)) {
        getConfiguration(jniResult);
    } else if ("  ".equals(messageType)) {////
        getSupportedSwitchType(jniResult);
    }else if ("Power Level Get Information".equals(messageType)) {
        getPowerLevel(jniResult);
    } else if ("Basic Information".equals(messageType)) {
        getBasic(jniResult);
    } else if ("  ".equals(messageType)) {////
        getSwitchMultiLevel(jniResult);
    }
    }


    private String getDeviceList(String Result) {
        JSONObject deviceListResult = null;
        try {
            deviceListResult = new JSONObject(Result);
            JSONArray list = deviceListResult.getJSONArray("Node Info List");
            for (int i = 0; i < list.length();i++ ) {
                JSONObject temp = list.getJSONObject(i);
                String homeId = temp.getString("Home id");
                String nodeId = temp.getString("Node id");
               ZwaveDevice zwaveDevice = zwaveDeviceManager.queryZwaveDevices(homeId, Integer.parseInt(nodeId));
                if (zwaveDevice != null) {
                    temp.put("deviceName", zwaveDevice.getName());
                } else {
                    temp.put("deviceName", "");
                }

            }
        } catch (JSONException e) {
            e.printStackTrace();
            return null;
        }
        return deviceListResult.toString();
    }

    private void deleteDevice(String result) {
        Logg.i(TAG,"=======deleteDevice==");
        Gson gson = new Gson();
        DeviceList deviceList =gson.fromJson(result, DeviceList.class);
        List<DeviceList.NodeInfoList> temp =  deviceList.getNodeList();
        List<ZwaveDevice> list = zwaveDeviceManager.queryZwaveDeviceList();
        Logg.i(TAG,"===deleteDevice====temp.size()=="+temp.size());
        Logg.i(TAG,"==deleteDevice=====list.size()=="+list.size());
        int i,j;
        for (ZwaveDevice zwaveDevice :list) {//db
            i = 0;
            for (DeviceList.NodeInfoList nodeInfoTemp : temp) {//jni
                Logg.i(TAG,"===deleteDevice====zwaveDevice.getHomeId()=="+zwaveDevice.getHomeId());
                Logg.i(TAG,"===deleteDevice====nodeInfoTemp.getHomeId()=="+nodeInfoTemp.getHomeId());
                Logg.i(TAG,"===deleteDevice====zwaveDevice.getNodeId()=="+zwaveDevice.getNodeId());
                Logg.i(TAG,"===deleteDevice====nodeInfoTemp.getNodeId()=="+nodeInfoTemp.getNodeId());
                if (zwaveDevice.getHomeId().toString().trim().equals(nodeInfoTemp.getHomeId().toString().trim())) {
                    if(!zwaveDevice.getNodeId().toString().trim().equals(nodeInfoTemp.getNodeId().toString().trim())){
                        i++;
                    }
                }/*else  {
                    zwaveDeviceManager.deleteZwaveDevice(zwaveDevice.getZwaveId());
                    String removeResult = "removeDevice:" + zwaveDevice.getHomeId() + ":" + zwaveDevice.getNodeId();
                    Logg.i(TAG,"====deleteDevice=else==removeResult=="+removeResult);
                    doRemoveDevice(removeResult);
                    break;
                }*/

                if (temp.size() == i) {
                    zwaveDeviceManager.deleteZwaveDevice(zwaveDevice.getZwaveId());
                    String removeResult = "removeDevice:" + zwaveDevice.getHomeId() + ":" + zwaveDevice.getNodeId();
                    Logg.i(TAG,"==deleteDevice=====removeResult=="+removeResult);
                    doRemoveDevice(removeResult);
                }
            }
        }
    }

    private void insertDevice(String result) {
        Logg.i(TAG,"=======insertDevice==");
        Gson gson = new Gson();
        DeviceList deviceList =gson.fromJson(result, DeviceList.class);
        List<DeviceList.NodeInfoList> temp =  deviceList.getNodeList();
        Logg.i(TAG,"==insertDevice=====temp.size()=="+temp.size());
        for (DeviceList.NodeInfoList nodeInfoTemp : temp) {
            Logg.i(TAG,"==insertDevice====nodeInfoTemp.getHomeId()=="+nodeInfoTemp.getHomeId());
            Logg.i(TAG,"==insertDevice====nodeInfoTemp.getNodeId()=="+nodeInfoTemp.getNodeId());
            ZwaveDevice device = zwaveDeviceManager.queryZwaveDevices(nodeInfoTemp.getHomeId(),
                    Integer.parseInt(nodeInfoTemp.getNodeId()));
            Logg.i(TAG,"==insertDevice====device==");
            if (device == null ) {
                ZwaveDevice zwaveDevice = new ZwaveDevice();
                zwaveDevice.setHomeId(nodeInfoTemp.getHomeId());
                zwaveDevice.setNodeId(Integer.valueOf(nodeInfoTemp.getNodeId()));
                zwaveDevice.setNodeInfo(gson.toJson(nodeInfoTemp));
                zwaveDevice.setName(nodeInfoTemp.getNodeId());
                zwaveDeviceManager.insertZwaveDevice(zwaveDevice);
                Logg.i(TAG,"===#########=="+nodeInfoTemp.getNodeId());
                Logg.i(TAG,"===####nodeInfoTemp.getNodeId().equals(1)#####=="+nodeInfoTemp.getNodeId().equals("1"));
                Logg.i(TAG,"===#########=="+nodeInfoTemp.getNodeId().toString().trim().equals("1"));
                if(!nodeInfoTemp.getNodeId().toString().trim().equals("1")){
                    String addResult = "addDevice:" + nodeInfoTemp.getHomeId() + ":" + nodeInfoTemp.getNodeId();
                    Logg.i(TAG,"===insertDevice====addResult=="+addResult);
                    doAddDevice(addResult);
                    break;
                }
            }else{
                Logg.i(TAG,"==insertDevice====device=="+device.getHomeId()+"===="+device.getNodeId());
            }
        }
    }

    private String getDeviceInfo(String jniResult) {
        Gson gson = new Gson();
        DeviceList deviceList =gson.fromJson(jniResult, DeviceList.class);
        List<DeviceList.NodeInfoList> temp =  deviceList.getNodeList();
        DeviceList.NodeInfoList nodeInfo = null;
        for (DeviceList.NodeInfoList nodeInfoTemp : temp) {
            if (nodeInfoTemp.getNodeId().toString().trim().equals(String.valueOf(nodeId).toString().trim())) {
                nodeInfo = nodeInfoTemp;
            }
        }
        return gson.toJson(nodeInfo);
    }


}
