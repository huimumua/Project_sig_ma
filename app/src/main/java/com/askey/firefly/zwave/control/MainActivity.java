package com.askey.firefly.zwave.control;

import android.Manifest;
import android.annotation.TargetApi;
import android.app.Activity;
import android.app.AlertDialog;
import android.content.ComponentName;
import android.content.Context;
import android.content.DialogInterface;
import android.content.Intent;
import android.content.ServiceConnection;
import android.content.pm.PackageManager;
import android.graphics.drawable.ColorDrawable;
import android.os.Build;
import android.os.IBinder;
import android.os.Looper;
import android.os.RemoteException;
import android.support.v4.app.ActivityCompat;
import android.support.v4.content.ContextCompat;
import android.support.v7.app.AppCompatActivity;
import android.os.Bundle;
import android.util.Log;
import android.view.Gravity;
import android.view.LayoutInflater;
import android.view.View;
import android.view.ViewGroup;
import android.view.WindowManager;
import android.widget.Button;
import android.widget.EditText;
import android.widget.LinearLayout;
import android.widget.PopupWindow;
import android.widget.TextView;
import android.widget.Toast;
import android.graphics.Color;

import com.askey.firefly.zwave.control.dao.ImportData;
import com.askey.firefly.zwave.control.dao.ZwaveDevice;
import com.askey.firefly.zwave.control.dao.ZwaveDeviceManager;
import com.askey.firefly.zwave.control.jni.ZwaveControlHelper;
import com.askey.firefly.zwave.control.service.ZwaveControlService;
import com.askey.firefly.zwave.control.thirdparty.usbserial.UsbSerial;
import com.askey.firefly.zwave.control.utils.Const;
import com.askey.firefly.zwave.control.utils.Logg;
import com.askey.zwave.control.IZwaveContrlCallBack;
import com.askey.zwave.control.IZwaveControlInterface;

import org.greenrobot.greendao.annotation.apihint.Internal;

import java.io.File;
import java.io.FileNotFoundException;
import java.io.FileOutputStream;
import java.io.IOException;
import java.io.InputStream;

import com.google.gson.Gson;

import org.json.JSONArray;
import org.json.JSONException;
import org.json.JSONObject;

public class MainActivity extends AppCompatActivity implements View.OnClickListener{

    private Button btnAdd,btnRemove,btnGet,btnOpen,btnBarr,btnSensor,btnUpdate,btnReset;
    private TextView txAddResult,txRemoveResult,txDeviceInfo,txOpenResult;
    IZwaveControlInterface zwaveService;
    private EditText etNodeId;
    private Button btnGetPower,btnOn, btnOff;
    private String nodeId;
    private UsbSerial mUsbSerial = new UsbSerial(this);

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);

        btnAdd = (Button) findViewById(R.id.btn_add);
        btnRemove = (Button) findViewById(R.id.btn_remove);
        btnGet = (Button) findViewById(R.id.btn_get);
        btnOpen = (Button) findViewById(R.id.btn_open);
        btnBarr = (Button) findViewById(R.id.btn_getBarr);
        btnSensor = (Button) findViewById(R.id.btn_getSenser);
        btnUpdate = (Button) findViewById(R.id.btn_update);
        etNodeId = (EditText) findViewById(R.id.et_node_id);
        btnGetPower = (Button) findViewById(R.id.btn_get_power);
        btnOn = (Button) findViewById(R.id.btn_on);
        btnOff = (Button) findViewById(R.id.btn_off);
        btnReset = (Button) findViewById(R.id.btn_reset);

        btnAdd.setOnClickListener(this);
        btnRemove.setOnClickListener(this);
        btnGet.setOnClickListener(this);
        btnOpen.setOnClickListener(this);
        btnBarr.setOnClickListener(this);
        btnSensor.setOnClickListener(this);
        btnUpdate.setOnClickListener(this);
        btnGetPower.setOnClickListener(this);
        btnOn.setOnClickListener(this);
        btnOff.setOnClickListener(this);
        btnReset.setOnClickListener(this);

        /*txAddResult = (TextView) findViewById(R.id.tx_add_result);
        txRemoveResult = (TextView) findViewById(R.id.tx_remove_result);
        txDeviceInfo = (TextView) findViewById(R.id.tx_get_result);*/
        txOpenResult = (TextView) findViewById(R.id.tx_open_result);

        Intent intent = new Intent();
        intent.setPackage("com.askey.firefly.zwave.control");
        bindService(intent, serviceConnection, Context.BIND_AUTO_CREATE);
    }

    private IZwaveContrlCallBack.Stub mCallback = new IZwaveContrlCallBack.Stub() {


        @Override
        public void addDeviceCallBack(String result) throws RemoteException {
            Log.i("MainActivity", "======addDeviceCallBack=====" + result);
            String messageType = null;
            String status = null;
            String nodeId = null;
            try {
                JSONObject jsonObject = new JSONObject(result);
                messageType = jsonObject.optString("MessageType");
                status = jsonObject.optString("Status");
                nodeId = jsonObject.optString("Nodeid");


            } catch (JSONException e) {
                e.printStackTrace();
            }
            if ("Success".equals(status)) {
                txOpenResult.setText(messageType+" :"+status+"  nodeId:"+nodeId);
            }
        }

        @Override
        public void openControlCallBack(String result,int length) throws RemoteException {
            Log.i("MainActivity", "======openControlCallBack=====" + result);
            if(result.equals("openController:0"))
            {
                //txOpenResult.setTextColor(Color.GREEN);
                txOpenResult.setText("Open the controller sucessfully.");
            }
            else
            {
                txOpenResult.setText("Open the controller with error:"+result);
            }
        }

        @Override
        public void removeDeviceCallBack(String result) throws RemoteException {
            Log.i("MainActivity", "======removeDeviceCallBack=====" + result);

            String messageType = null;
            String status = null;
            String nodeId = null;
            try {
                JSONObject jsonObject = new JSONObject(result);
                messageType = jsonObject.optString("MessageType");
                status = jsonObject.optString("Status");
                nodeId = jsonObject.optString("Nodeid");


            } catch (JSONException e) {
                e.printStackTrace();
            }
            if ("Success".equals(status)) {
                txOpenResult.setText(messageType+" :"+status+"  nodeId:"+nodeId);
            }

        }

        @Override
        public void getDevicesCallBack(String result) throws RemoteException {
            //Log.i("MainActivity", "======getDevicesCallBack=====" + result);
            Context context = getBaseContext();
            View contentView = LayoutInflater.from(context).inflate(R.layout.window_layout, null);
            WindowManager manger = (WindowManager) getSystemService(Context.WINDOW_SERVICE);

            final PopupWindow popupWindow = new PopupWindow(contentView,
                            manger.getDefaultDisplay().getWidth()-100, WindowManager.LayoutParams.WRAP_CONTENT, true);

            popupWindow.setTouchable(true);
            TextView showText = (TextView) contentView.findViewById(R.id.node_info);
            showText.setText(result);
            showText.setBackgroundColor(Color.GRAY);

            popupWindow.setBackgroundDrawable(new ColorDrawable());
            popupWindow.showAtLocation(contentView, Gravity.CENTER_VERTICAL, 0, 0);
            popupWindow.showAsDropDown(contentView);
        }

        @Override
        public void getDevicesInfoCallBack(String result) throws RemoteException {
            Log.i("MainActivity", "======getDevicesInfoCallBack=====" + result);
        }

        @Override
        public void removeFailedDeviceCallBack(String result) throws RemoteException {

        }

        @Override
        public void replaceFailedDeviceCallBack(String result) throws RemoteException {

        }

        @Override
        public void stopAddDeviceCallBack(String result) throws RemoteException {

        }

        @Override
        public void stopRemoveDeviceCallBack(String result) throws RemoteException {

        }

        @Override
        public void getDeviceBatteryCallBack(String result) throws RemoteException {

        }

        @Override
        public void getSensorMultiLevelCallBack(String result) throws RemoteException {

        }

        @Override
        public void updateNodeCallBack(String result) throws RemoteException {

        }

        @Override
        public void reNameDeviceCallBack(String result) throws RemoteException {

        }

        @Override
        public void setDefaultCallBack(String result) throws RemoteException {

        }

        @Override
        public void getConfiguration(String result) throws RemoteException {

        }

        @Override
        public void setConfiguration(String result) throws RemoteException {

        }

        @Override
        public void getSupportedSwitchType(String result) throws RemoteException {

        }

        @Override
        public void startStopSwitchLevelChange(String result) throws RemoteException {

        }

        @Override
        public void getPowerLevel(String result) throws RemoteException {

        }

        @Override
        public void setSwitchAllOn(String result) throws RemoteException {

        }

        @Override
        public void setSwitchAllOff(String result) throws RemoteException {

        }

        @Override
        public void getBasic(String result) throws RemoteException {
            Looper.prepare();
            // 创建构建器
            /*AlertDialog.Builder builder = new AlertDialog.Builder(MainActivity.this);
            // 设置参数
            builder.setTitle("Message from device").setIcon(R.drawable.ic_launcher_background)
                    .setMessage(result)
                    .setNeutralButton("Got it", new DialogInterface.OnClickListener() {// 中间级

                                @Override
                                public void onClick(DialogInterface dialog,
                                        int which) {
                                    //this.finish();
                                }
                            });
            builder.create().show();*/
            
            Toast.makeText(MainActivity.this, result, Toast.LENGTH_LONG).show();
            Looper.loop();
        }

        @Override
        public void setBasic(String result) throws RemoteException {

        }

        @Override
        public void getSwitchMultiLevel(String result) throws RemoteException {

        }

        @Override
        public void setSwitchMultiLevel(String result) throws RemoteException {

        }

    };

    private ServiceConnection serviceConnection = new ServiceConnection() {
        @Override
        public void onServiceConnected(ComponentName componentName, IBinder iBinder) {
            zwaveService = IZwaveControlInterface.Stub.asInterface(iBinder);

            if (zwaveService != null) {
                try {
                    zwaveService.registerListener(mCallback);
                } catch (RemoteException e) {
                    e.printStackTrace();
                }
            }
        }

        @Override
        public void onServiceDisconnected(ComponentName componentName) {

        }
    };
    @Override
    protected void onDestroy() {
        super.onDestroy();
        unbindService(serviceConnection);
    }


    @Override
    public void onClick(View view) {
        nodeId = etNodeId.getText().toString();
        if ("".equals(nodeId)) {
            nodeId = "1";
        }

        switch (view.getId()) {
            case R.id.btn_open:
                if (zwaveService != null) {
                    try {
                        //remove service
                        zwaveService.openController(mCallback);
                    } catch (RemoteException e) {
                        e.printStackTrace();
                    }
                }
                break;
            case R.id.btn_add:
                if (zwaveService != null) {
                    try {
                        //remove service
                        zwaveService.addDevice(mCallback);
                        //txAddResult.setText("add device result");
                    } catch (RemoteException e) {
                        e.printStackTrace();
                    }

                }
                break;
            case R.id.btn_remove:
                if (zwaveService != null) {
                    try {
                        //remove service
                        zwaveService.removeDevice(mCallback);
                        //txRemoveResult.setText("remove device result");

                    } catch (RemoteException e) {
                        e.printStackTrace();
                    }

                }
                break;
            case R.id.btn_get:
                if (zwaveService != null) {
                    try {
                        //remove service
                        zwaveService.getDevices(mCallback);
                        //txDeviceInfo.setText("device information result");
                    } catch (RemoteException e) {
                        e.printStackTrace();
                    }

                }
                break;
            case R.id.btn_getBarr:
                if (zwaveService != null) {
                    try {
                        //remove service
                        zwaveService.getDeviceBattery(mCallback,Integer.parseInt(nodeId));
                    } catch (RemoteException e) {
                        e.printStackTrace();
                    }

                }
                break;
            case R.id.btn_getSenser:
                if (zwaveService != null) {
                    try {
                        //remove service
                        zwaveService.getSensorMultiLevel(mCallback,Integer.parseInt(nodeId));
                    } catch (RemoteException e) {
                        e.printStackTrace();
                    }

                }
                break;
            case R.id.btn_update:
                if (zwaveService != null) {
                    try {
                        //remove service
                        zwaveService.updateNode(mCallback,Integer.parseInt(nodeId));
                        //ZwaveControlHelper.ZwController_SetSwitchMultiLevel(Integer.parseInt(nodeId),20,5);
                    } catch (RemoteException e) {
                        e.printStackTrace();
                    }

                }
                break;
            case R.id.btn_get_power:
                if (!"".equals(nodeId)) {
                    try {
                        zwaveService.getPowerLevel(mCallback, Integer.parseInt(nodeId));
                    } catch (RemoteException e) {
                        e.printStackTrace();
                    }
                }
                break;
            case R.id.btn_on:
                if (!"".equals(nodeId)) {
                    try {
                        zwaveService.setSwitchAllOn(mCallback, Integer.parseInt(nodeId));
                    } catch (RemoteException e) {
                        e.printStackTrace();
                    }
                }
                break;
            case R.id.btn_off:
                if (!"".equals(nodeId)) {
                    try {
                        zwaveService.setSwitchAllOff(mCallback, Integer.parseInt(nodeId));
                    } catch (RemoteException e) {
                        e.printStackTrace();
                    }
                }
                break;
            case R.id.btn_reset:
                try {
                    zwaveService.setDefault(mCallback);
                } catch (RemoteException e) {
                    e.printStackTrace();
                }
                break;
        }
    }

}
