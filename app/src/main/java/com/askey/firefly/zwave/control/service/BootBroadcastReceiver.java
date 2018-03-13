package com.askey.firefly.zwave.control.service;

import android.content.BroadcastReceiver;
import android.content.ComponentName;
import android.content.Context;
import android.content.Intent;
import android.content.ServiceConnection;
import android.os.IBinder;

import com.askey.firefly.zwave.control.MainActivity;
import com.askey.firefly.zwave.control.utils.Logg;
import com.askey.zwave.control.IZwaveControlInterface;

/**
 * 项目名称：ZwaveControl
 * 类描述：
 * 创建人：skysoft  charles.bai
 * 创建时间：2017/7/10 11:39
 * 修改人：skysoft
 * 修改时间：2017/7/10 11:39
 * 修改备注：
 */
public class BootBroadcastReceiver extends BroadcastReceiver{
    private final String TAG = "BootBroadcastReceiver";
    @Override
    public void onReceive(Context context, Intent intent) {

        if (intent.getAction().equals("android.intent.action.BOOT_COMPLETED")) {
            Logg.i(TAG, "=========BOOT_COMPLETED===========");
//启动程序
            Intent activity = new Intent(context, MainActivity.class);
            activity.addFlags(Intent.FLAG_ACTIVITY_NEW_TASK);
            context.startActivity(activity);
        }
        if (intent.getAction().equals("android.intent.action.USER_PRESENT")) {
            Logg.i(TAG, "=========USER_PRESENT===========");
            ServiceConnection serviceConnection = new ServiceConnection() {
                @Override
                public void onServiceConnected(ComponentName componentName, IBinder iBinder) {
                }

                @Override
                public void onServiceDisconnected(ComponentName componentName) {

                }
            };
            Intent service = new Intent(context, ZwaveControlService.class);
            context. bindService(service, serviceConnection, Context.BIND_AUTO_CREATE);
        }
        if (intent.getAction().equals("android.intent.action.destroy")) {
            Logg.i(TAG, "=========destroy===========");
            //启动程序
            Intent activity = new Intent(context, MainActivity.class);
            activity.addFlags(Intent.FLAG_ACTIVITY_NEW_TASK);
            context.startActivity(activity);
        }
    }
}
