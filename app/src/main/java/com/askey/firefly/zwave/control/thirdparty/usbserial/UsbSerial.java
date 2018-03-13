package com.askey.firefly.zwave.control.thirdparty.usbserial;

import android.content.IntentFilter;
import android.hardware.usb.UsbDevice;
import android.util.Log;

import java.util.concurrent.Semaphore;


import android.content.Context;
import android.hardware.usb.UsbDeviceConnection;
import android.hardware.usb.UsbManager;

import com.askey.firefly.zwave.control.thirdparty.usbserial.usbserial.APMSerialDevice;
import com.askey.firefly.zwave.control.thirdparty.usbserial.usbserial.CDCSerialDevice;

import static java.lang.Thread.sleep;


public class UsbSerial {

    static {
        System.loadLibrary("usbserial-jni");
    }

    private final String TAG = "UsbSerial";
    private Context mContext = null;

    public UsbSerial(Context context) {
        mContext = context;
        UsbSerial_Set_Object();
    }

    public int Open()
    {
        return CDCSerialDevice.Open(mContext);
    }

    public void Close()
    {
        CDCSerialDevice.Close();
    }

    public void Write(byte[] data)
    {
        CDCSerialDevice.Write(data);
    }

    public int Read(byte[] data, int len)
    {
        return CDCSerialDevice.Read(data,len);
    }

    public int Check()
    {
        return CDCSerialDevice.BytesToRead();
    }

    public void SwitchToApmMode(){

        final byte[] cmd = {0x01,0x03,0x00,0x27,(byte)0xDB};
        byte[] ret = new byte[16];
        Write(cmd);
        try{
            sleep(1000);
        }catch(InterruptedException e){
            e.printStackTrace();
        }
        Read(ret,1);
        if(ret[0] == 0x06){
            Log.d(TAG,"Device Enter APM...");
        }else{
            Log.d(TAG,"Device Enter APM Fail...");
        }
        Close();
    }

    private native void UsbSerial_Set_Object();

    public int OpenApm()
    {
        int ret;
        int de_cnt = 10;
        while(de_cnt > 0){
            try{
                ret = APMSerialDevice.Open(mContext);
                if(ret == 0){
                    return 0;
                }else if(ret == -1){
                    return -1;
                }
                sleep(3000);
            }catch (InterruptedException e){
                Log.e(TAG, "Open Unknown Error....");
                return -1;
            }
            de_cnt--;
        }
        Log.e(TAG, "Open APM Device Timeout....");
        return -1;
    }

    public int GetApmBcdDevice(){
        return APMSerialDevice.GetBcdDevice();
    }

    public void XFR(byte[] send,byte[] rec){
        WriteApm(send);
        ReadApm(rec,rec.length);
    }

    public void WriteApm(byte[] data){
        APMSerialDevice.Write(data);
    }

    public int ReadApm(byte[] buf,int len){
        return APMSerialDevice.Read(buf,len);
    }

    public void CloseApm(){
        APMSerialDevice.Close();
    }

}
