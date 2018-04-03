package com.askey.firefly.zwave.control.service;

import android.content.BroadcastReceiver;
import android.content.Context;
import android.content.Intent;

import com.askey.firefly.zwave.control.jni.ZwaveControlHelper;

public class ZwaveReceiver extends BroadcastReceiver {
    public ZwaveReceiver() {
    }

    @Override
    public void onReceive(Context context, Intent intent) {
        // TODO: This method is called when the BroadcastReceiver is receiving
        //aidl   暂时不用
        if (intent.getAction().equals("action.com.askey.zwave.add")) {

            byte[] dsk = new byte[200];
            ZwaveControlHelper.ZwController_AddDevice();

        } else if (intent.getAction().equals("action.com.askey.zwave.remove")) {

           String noteId = intent.getStringExtra("RemoveDevice");
            ZwaveControlHelper.ZwController_RemoveDevice();

        } else if (intent.getAction().equals("action.com.askey.zwave")) {

           String getStatus = intent.getStringExtra("get");
           String setStatus = intent.getStringExtra("set");
            if (getStatus != null && "getStatus".equals(getStatus)) {
                //Get device status
            }
            if (setStatus != null) {
                //control zwave device

            }

        }
    }
}
