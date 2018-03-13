package com.askey.firefly.zwave.control.thirdparty.usbserial.usbserial;

import android.app.PendingIntent;
import android.content.Context;
import android.content.Intent;
import android.hardware.usb.UsbConstants;
import android.hardware.usb.UsbDevice;
import android.hardware.usb.UsbDeviceConnection;
import android.hardware.usb.UsbEndpoint;
import android.hardware.usb.UsbInterface;
import android.hardware.usb.UsbManager;
import android.hardware.usb.UsbRequest;
import android.util.Log;

import com.askey.firefly.zwave.control.thirdparty.usbserial.ByteRingBuffer;
import com.askey.firefly.zwave.control.thirdparty.usbserial.utils.HexData;

import java.nio.ByteBuffer;
import java.util.HashMap;
import java.util.Map;
import java.util.concurrent.atomic.AtomicBoolean;

public class APMSerialDevice
{
    private static final String TAG = "UsbSerial";
    private static final String ACTION_USB_PERMISSION = "com.android.example.USB_PERMISSION";

    public static UsbDeviceConnection conn = null;
    public static UsbInterface iface = null;
    private static UsbEndpoint inEndpoint = null;
    private static UsbEndpoint outEndpoint = null;
    public static WorkerThread task = null;
    public static UsbManager manager = null;
    public static UsbDevice dev = null;
    public static int vid = 0;
    public static int pid = 0;
    public static ByteRingBuffer buffer = new ByteRingBuffer(1024);

    public static int Open(Context host)
    {
        if(task != null){
            Log.e(TAG,"APM Task Is Already Running...");
            return -1;
        }
        manager = (UsbManager) host.getSystemService(Context.USB_SERVICE);
        HashMap<String, UsbDevice> usbDevices = manager.getDeviceList();
        if (usbDevices.isEmpty()){
            return -2;
        }
        for (Map.Entry<String, UsbDevice> entry : usbDevices.entrySet()) {
            dev = entry.getValue();
            vid = dev.getVendorId();
            pid = dev.getProductId();
            Log.d(TAG, "Usb Device Vid = "+ Integer.toHexString(vid) +",Pid = "+ Integer.toHexString(pid));
            if ((vid == 0x0658) && (pid == 0x0280)) {
                Log.d(TAG, "Usb Device Is APM Device...");
                if (manager.hasPermission(dev)) {
                    Log.d(TAG, "Usb Permission Ok....");
                    break;
                } else {
                    Log.e(TAG, "Usb Permission Na,Try To Request Permission....");
                    PendingIntent mPendingIntent = PendingIntent.getBroadcast(host, 0, new Intent(ACTION_USB_PERMISSION), 0);
                    manager.requestPermission(dev, mPendingIntent);
                    return -2;
                }
            }else{
                dev = null;
                Log.d(TAG, "Usb Device Is Not APM Device...");
            }
        }
        if(dev == null){
            Log.d(TAG, "APM Device Not Found...");
            return -2;
        }
        int interfaceCount = dev.getInterfaceCount();
        Log.d(TAG,"APM Device Interface Count = " + interfaceCount);
        iface = dev.getInterface(0);
        if(iface == null){
            Log.e(TAG,"APM Data Interface Not Found...");
            return -2;
        }

        conn = manager.openDevice(dev);

        if(conn.claimInterface(iface, true))
        {
            Log.d(TAG, "Interface succesfully claimed");
        }else
        {
            Log.e(TAG, "Interface could not be claimed");
            return -2;
        }

        // Assign endpoints
        int numberEndpoints = iface.getEndpointCount();
        Log.d(TAG, "APM Device Data Interface Has " + numberEndpoints + " Endpoints...");
        for(int i=0;i<=numberEndpoints-1;i++)
        {
            UsbEndpoint endpoint = iface.getEndpoint(i);
            if(endpoint.getType() == UsbConstants.USB_ENDPOINT_XFER_BULK
                    && endpoint.getDirection() == UsbConstants.USB_DIR_IN)
            {
                Log.d(TAG, "APM Device Data Interface Endpoint " + i + " Is IN Endpoint, Address = " + endpoint.getAddress());
                inEndpoint = endpoint;
            }else if(endpoint.getType() == UsbConstants.USB_ENDPOINT_XFER_BULK
                    && endpoint.getDirection() == UsbConstants.USB_DIR_OUT)
            {
                Log.d(TAG, "APM Device Data Interface Endpoint " + i + " Is Out Endpoint, Address = " + endpoint.getAddress());
                outEndpoint = endpoint;
            }else{
                Log.e(TAG, "APM Device Data Interface Endpoint " + i + " Is Unknown Endpoint...");
            }
        }

        if(outEndpoint == null || inEndpoint == null)
        {
            Log.e(TAG, "APM Device Data Interface Does Not Have An IN or OUT Endpoint");
            return -2;
        }
        Log.d(TAG,"APM Device Open Success...");
        task = new WorkerThread();
        task.callback = mCallback;
        task.requestIN = new UsbRequest();
        task.requestIN.initialize(conn, inEndpoint);
        task.requestIN.queue(task.buffer, 1024);
        task.start();
        return 0;
    }

    public static void Close()
    {
        if(task == null){
            Log.e(TAG,"Read Task Not Running...");
            return;
        }
        task.working.set(false);
        conn.releaseInterface(iface);
        conn.close();
        task = null;
    }

    public static void Write(byte[] data) {
        int n;
        if(task == null){
            Log.e(TAG,"Read Task Not Running...");
            return;
        }
        n = conn.bulkTransfer(outEndpoint, data, data.length, 2000);
        //Log.d(TAG,"Write Data::" + hexToString(data));
        //Log.d(TAG,"Actually  Wrote Data Length = " + n);
    }

    public static int Read(byte[] data,int len){
        int n;
        if(task == null){
            Log.e(TAG,"Read Task Not Running...");
            return -1;
        }

        n = buffer.read(data, 0, len);
        while(0 == n)
        {
            Log.w(TAG,"APM read data length is 0, retry....");
            try{
                Thread.sleep(2);
                n = buffer.read(data, 0, len);
            }
            catch (InterruptedException e)
            {
                e.printStackTrace();
            }
        }

        Log.d(TAG,"Actually  Read Data Length = " + n);
        if(n > 0){
            byte[] buf = new byte[n];
            System.arraycopy(data,0,buf,0,n);
            //Log.d(TAG,"Read Data::" + hexToString(buf));
        }
        return n;
    }

    public static int GetBcdDevice(){
        byte[] des;
        int bcdDevice;
        if(task == null){
            Log.e(TAG,"Read Task Not Running...");
            return -1;
        }
        des = new byte[0x12];
        conn.controlTransfer(0x80, 0x06, 0x0100, 0, des, 0x12, 2000);
        bcdDevice = 0xFF & des[12];
        bcdDevice += (0xFF & des[13]) * 0x100;
        Log.d(TAG,"BcdDevice = " + Integer.toHexString(bcdDevice)+ " Raw Device Descriptor::" + HexData.hexToString(des));
        return bcdDevice;
    }

    private static UsbReadCallback mCallback = new UsbReadCallback() {
        @Override
        public void onReceivedData(byte[] arg0) {
            Log.d(TAG,"Received Data::" + hexToString(arg0));
            buffer.write(arg0);
        }
    };

    private static class WorkerThread extends Thread {

        public UsbReadCallback callback;
        private UsbRequest requestIN;
        public AtomicBoolean working;
        public ByteBuffer buffer = ByteBuffer.allocate(1024);

        WorkerThread() {
            working = new AtomicBoolean(true);
        }

        @Override
        public void run() {
            Log.d(TAG, "Usb Serial Port Reading Task Start...");
            while (working.get()) {
                UsbRequest request = conn.requestWait();
                if (request != null && request.getEndpoint().getType() == UsbConstants.USB_ENDPOINT_XFER_BULK
                        && request.getEndpoint().getDirection() == UsbConstants.USB_DIR_IN) {
                    if (buffer.position() > 0) {
                        byte[] dst = new byte[buffer.position()];
                        buffer.position(0);
                        buffer.get(dst, 0, dst.length);
                        buffer.clear();
                        callback.onReceivedData(dst);
                    } else {
                        //Log.d(TAG, "Usb Serial Port Read Nothing...");
                    }
                    requestIN.queue(buffer, 1024);
                } else {
                    Log.e(TAG, "Usb Serial Port Request In Wait Expression...");
                    return;
                }
            }
        }
    }

    interface UsbReadCallback
    {
        void onReceivedData(byte[] data);
    }


    private static String hexToString(byte[] data)
    {
        final String HEXES = "0123456789ABCDEF";
        final String HEX_INDICATOR = "0x";
        final String SPACE = " ";
        if(data != null)
        {
            StringBuilder hex = new StringBuilder(2*data.length);
            for(int i=0;i<=data.length-1;i++)
            {
                byte dataAtIndex = data[i];
                hex.append(HEX_INDICATOR);
                hex.append(HEXES.charAt((dataAtIndex & 0xF0) >> 4))
                        .append(HEXES.charAt((dataAtIndex & 0x0F)));
                hex.append(SPACE);
            }
            return hex.toString();
        }else
        {
            return null;
        }
    }
}
