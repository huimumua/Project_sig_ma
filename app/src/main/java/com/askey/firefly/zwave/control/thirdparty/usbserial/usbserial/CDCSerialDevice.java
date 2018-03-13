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

public class CDCSerialDevice
{
    private static final String TAG = "UsbSerial-CDC";
    private static final int CDC_REQTYPE_HOST2DEVICE = 0x21;
    private static final int CDC_REQTYPE_DEVICE2HOST = 0xA1;
    private static final int CDC_SET_LINE_CODING = 0x20;
    private static final int CDC_GET_LINE_CODING = 0x21;
    private static final int CDC_SET_CONTROL_LINE_STATE = 0x22;
    private static final String ACTION_USB_PERMISSION = "com.android.example.USB_PERMISSION";

    // Common values
    private static final int DATA_BITS_5 = 5;
    private static final int DATA_BITS_6 = 6;
    private static final int DATA_BITS_7 = 7;
    private static final int DATA_BITS_8 = 8;

    private static final int STOP_BITS_1 = 1;
    private static final int STOP_BITS_15 = 3;
    private static final int STOP_BITS_2 = 2;

    private static final int PARITY_NONE = 0;
    private static final int PARITY_ODD = 1;
    private static final int PARITY_EVEN = 2;
    private static final int PARITY_MARK = 3;
    private static final int PARITY_SPACE = 4;

    private static final byte[] CDC_DEFAULT_LINE_CODING = new byte[] {
            (byte) 0x00, // Offset 0:4 dwDTERate
            (byte) 0xC2,
            (byte) 0x01,
            (byte) 0x00,
            (byte) 0x00, // Offset 5 bCharFormat (1 Stop bit)
            (byte) 0x00, // bParityType (None)
            (byte) 0x08  // bDataBits (8)
    };

    private static final int CDC_CONTROL_LINE_ON = 0x0003;
    private static final int CDC_CONTROL_LINE_OFF = 0x0000;


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
            Log.e(TAG,"CDC Task Is Already Running...");
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
            if ((vid == 0x0658) && (pid == 0x0200)) {
                Log.d(TAG, "Usb Device Is CDC Device...");
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
                Log.d(TAG, "Usb Device Is Not CDC Device...");
            }
        }
        if(dev == null){
            Log.d(TAG, "CDC Device Not Found...");
            return -2;
        }
        int interfaceCount = dev.getInterfaceCount();
        Log.d(TAG,"CDC Device Interface Count = " + interfaceCount);
        for (int iIndex = 0; iIndex < interfaceCount; ++iIndex)
        {
            iface = dev.getInterface(iIndex);
            if (iface.getInterfaceClass() == UsbConstants.USB_CLASS_CDC_DATA)
            {
                Log.d(TAG,"CDC Data Interface Found...");
                break;
            }else{
                iface = null;
            }
        }
        if(iface == null){
            Log.e(TAG,"CDC Data Interface Not Found...");
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
        Log.d(TAG, "CDC Device Data Interface Has " + numberEndpoints + " Endpoints...");
        for(int i=0;i<=numberEndpoints-1;i++)
        {
            UsbEndpoint endpoint = iface.getEndpoint(i);
            if(endpoint.getType() == UsbConstants.USB_ENDPOINT_XFER_BULK
                    && endpoint.getDirection() == UsbConstants.USB_DIR_IN)
            {
                endpoint.getAddress();
                Log.d(TAG, "CDC Device Data Interface Endpoint " + i + " Is IN Endpoint, Address = " + endpoint.getAddress());
                inEndpoint = endpoint;
            }else if(endpoint.getType() == UsbConstants.USB_ENDPOINT_XFER_BULK
                    && endpoint.getDirection() == UsbConstants.USB_DIR_OUT)
            {
                Log.d(TAG, "CDC Device Data Interface Endpoint " + i + " Is OUT Endpoint, Address = " + endpoint.getAddress());
                outEndpoint = endpoint;
            }else{
                Log.e(TAG, "CDC Device Data Interface Endpoint " + i + " Is Unknown Endpoint...");
            }
        }

        if(outEndpoint == null || inEndpoint == null)
        {
            Log.e(TAG, "CDC Device Data Interface Does Not Have An IN or OUT Endpoint");
            return -2;
        }

        // Default Setup
        setControlCommand(CDC_SET_LINE_CODING, 0, CDC_DEFAULT_LINE_CODING);
        setControlCommand(CDC_SET_CONTROL_LINE_STATE, CDC_CONTROL_LINE_ON, null);
        setBaudRate(115200);
        setDataBits(DATA_BITS_8);
        setStopBits(STOP_BITS_1);
        setParity(PARITY_NONE);
        Log.d(TAG,"CDC Device Open Success...");
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
            //Log.e(TAG,"Read Task Not Running...");
            return;
        }

        setControlCommand(CDC_SET_CONTROL_LINE_STATE, CDC_CONTROL_LINE_OFF, null);
        conn.releaseInterface(iface);
        conn.close();
        task.working.set(false);

        try
        {
            task.join();
        }
        catch (InterruptedException e)
        {
            e.printStackTrace();
        }

        task = null;
    }

    public static void Write(byte[] data) {
        int n;

        if(task == null){
            //Log.e(TAG,"Read Task Not Running...");
            return;
        }

        n = conn.bulkTransfer(outEndpoint, data, data.length, 2000);
        //Log.d(TAG,"Write Data::" + hexToString(data));
        //Log.d(TAG,"Actually  Wrote Data Length = " + n);
    }

    public static int Read(byte[] data,int len){
        int n;

        n = buffer.read(data, 0, len);
        //Log.d(TAG,"Actually  Read Data Length = " + n);
        if(n > 0){
            byte[] buf = new byte[n];
            System.arraycopy(data,0,buf,0,n);
            //Log.d(TAG,"Read "+n+" Data::" + hexToString(buf));
        }
        return n;
    }
    public static int GetBcdDevice(){
        byte[] des;
        int bcdDevice;

        if(task == null){
            //Log.e(TAG,"Read Task Not Running...");
            return -1;
        }

        des = new byte[0x12];
        conn.controlTransfer(0x80, 0x06, 0x0100, 0, des, 0x12, 2000);
        bcdDevice = 0xFF & des[12];
        bcdDevice += (0xFF & des[13]) * 0x100;
        Log.d(TAG,"BcdDevice = " + Integer.toHexString(bcdDevice)+ " Raw Device Descriptor::" + HexData.hexToString(des));
        return bcdDevice;
    }

    public static int BytesToRead(){
        return buffer.getUsed();
    }

    private static void setBaudRate(int baudRate)
    {
        byte[] data = getLineCoding();

        data[0] = (byte) (baudRate & 0xff);
        data[1] = (byte) (baudRate >> 8 & 0xff);
        data[2] = (byte) (baudRate >> 16 & 0xff);
        data[3] = (byte) (baudRate >> 24 & 0xff);

        setControlCommand(CDC_SET_LINE_CODING, 0, data);
    }

    private static void setDataBits(int dataBits)
    {
        byte[] data = getLineCoding();
        switch(dataBits)
        {
            case DATA_BITS_5:
                data[6] = 0x05;
                break;
            case DATA_BITS_6:
                data[6] = 0x06;
                break;
            case DATA_BITS_7:
                data[6] = 0x07;
                break;
            case DATA_BITS_8:
                data[6] = 0x08;
                break;
            default:
                return;
        }
        setControlCommand(CDC_SET_LINE_CODING, 0, data);
    }

    private static void setStopBits(int stopBits)
    {
        byte[] data = getLineCoding();
        switch(stopBits)
        {
            case STOP_BITS_1:
                data[4] = 0x00;
                break;
            case STOP_BITS_15:
                data[4] = 0x01;
                break;
            case STOP_BITS_2:
                data[4] = 0x02;
                break;
            default:
                return;
        }
        setControlCommand(CDC_SET_LINE_CODING, 0, data);
    }

    public static void setParity(int parity)
    {
        byte[] data = getLineCoding();
        switch(parity)
        {
            case PARITY_NONE:
                data[5] = 0x00;
                break;
            case PARITY_ODD:
                data[5] = 0x01;
                break;
            case PARITY_EVEN:
                data[5] = 0x02;
                break;
            case PARITY_MARK:
                data[5] = 0x03;
                break;
            case PARITY_SPACE:
                data[5] = 0x04;
                break;
            default:
                return;
        }
        setControlCommand(CDC_SET_LINE_CODING, 0, data);
    }

    private static UsbReadCallback mCallback = new UsbReadCallback() {
        @Override
        public void onReceivedData(byte[] arg0) {
            buffer.write(arg0);
        }
    };

    private static int setControlCommand(int request, int value, byte[] data)
    {
        int dataLength = 0;
        if(data != null)
        {
            dataLength = data.length;
        }
        int response = conn.controlTransfer(CDC_REQTYPE_HOST2DEVICE, request, value, 0, data, dataLength, 5000);
        Log.d(TAG,"Control Transfer Response: " + String.valueOf(response));
        return response;
    }

    private static byte[] getLineCoding()
    {
        byte[] data = new byte[7];
        int response = conn.controlTransfer(CDC_REQTYPE_DEVICE2HOST, CDC_GET_LINE_CODING, 0, 0, data, data.length, 5000);
        Log.d(TAG,"Control Transfer Response: " + String.valueOf(response));
        return data;
    }

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
                        Log.d(TAG, "Usb Serial Port Read Nothing...");
                    }
                    requestIN.queue(buffer, 1024);
                } else {
                    Log.e(TAG, "Usb Serial Port Request In Wait Expression...");
                    break;
                }
            }

            Log.d(TAG, "Usb Serial Port Reading Task Exit...");
        }
    }

    interface UsbReadCallback
    {
        void onReceivedData(byte[] data);
    }


    private static String hexToString(byte[] data)
    {
        final String HEXES = "0123456789ABCDEF";
        final String HEX_INDICATOR = "";
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
