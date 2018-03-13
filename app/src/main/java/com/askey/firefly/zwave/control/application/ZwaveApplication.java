package com.askey.firefly.zwave.control.application;

import android.app.Application;

/**
 * Created by skysoft on 2017/7/27.
 */

public class ZwaveApplication extends Application {
    private static ZwaveApplication instance;

    @Override
    public void onCreate() {
        super.onCreate();
        instance = this;
    }

    public static ZwaveApplication getInstance(){
        return instance;
    }

}
