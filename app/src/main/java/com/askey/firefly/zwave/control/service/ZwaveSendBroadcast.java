package com.askey.firefly.zwave.control.service;

import android.content.Context;
import android.content.Intent;
import android.util.Log;

import com.askey.firefly.zwave.control.application.ZwaveApplication;

import org.json.JSONException;
import org.json.JSONObject;

/**
 * Created by skysoft on 2017/7/27.
 */

public class ZwaveSendBroadcast {

    public static ZwaveSendBroadcast getInstance () {
        return SingleInstanceHolder.zwaveSendBroadcast;
    }
    private static class SingleInstanceHolder {
        private static ZwaveSendBroadcast zwaveSendBroadcast = new ZwaveSendBroadcast();
    }
    public void zwaveCallBack(byte[] result, int len) {
        //jni callback
        String jniResult = new String(result);
        String messageType = null;
        try {
            JSONObject jsonObject = new JSONObject(jniResult);
            messageType = jsonObject.optString("MessageType");

        } catch (JSONException e) {
            e.printStackTrace();
        }
        if ("Node Add Status".equals(messageType)) {
            addResult(jniResult);
        } else if ("Node Remove Status".equals(messageType)) {
            removeResult(jniResult);
        } else if ("Node Sensor Report".equals(messageType)) {//还要改
            sensorResult(jniResult);
        }


    }

    private void sensorResult(String jniResult) {
            Intent intent = new Intent();
            intent.putExtra("message", jniResult);
            intent.setAction("action.com.askey.mqtt");
            ZwaveApplication.getInstance().sendBroadcast(intent);
    }

    private void removeResult(String jniResult) {
        try {
            Intent intent = new Intent();
            JSONObject jsonObject = new JSONObject(jniResult);
            String status = jsonObject.optString("Status");
            if ("Success".equals(status)) {

                String nodeId = jsonObject.optString("NodeID");
                intent.putExtra("remove", nodeId);

            } else if ("Fail".equals(status)) {

                intent.putExtra("remove", false);
            }
            intent.setAction("action.com.askey.mqtt");
            ZwaveApplication.getInstance().sendBroadcast(intent);
        } catch (JSONException e) {
            e.printStackTrace();
        }
    }

    private void addResult(String jniResult) {
        try {
            Intent intent = new Intent();
            JSONObject jsonObject = new JSONObject(jniResult);
            String status = jsonObject.optString("Status");
            if ("Success".equals(status)) {
                String nodeId = jsonObject.optString("NodeID");
                intent.putExtra("id", "AddDevice:" + nodeId);

            } else if ("Fail".equals(status)) {
                intent.putExtra("id", "AddDevice:fail");
            }
//            intent.setAction("action.com.askey.mqtt");
            intent.setAction("com.askey.mqtt");
            ZwaveApplication.getInstance().sendBroadcast(intent);
        } catch (JSONException e) {
            e.printStackTrace();
        }
    }

}
