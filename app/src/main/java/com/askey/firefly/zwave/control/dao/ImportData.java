package com.askey.firefly.zwave.control.dao;

import android.content.Context;
import android.util.Log;

import com.askey.firefly.zwave.control.R;
import com.askey.firefly.zwave.control.service.ZwaveControlService;
import com.askey.firefly.zwave.control.utils.Const;

import java.io.File;
import java.io.FileNotFoundException;
import java.io.FileOutputStream;
import java.io.IOException;
import java.io.InputStream;

/**
 * Created by skysoft on 2017/1/11.
 */

public class ImportData {
    //导入单个外部数据库
    public static boolean importDatabase(Context context) {
        //若存在sd卡
        String dbPath =  Const.DATA_PATH;
        File dir = new File(dbPath);
        if (!dir.exists()) {
            dir.mkdirs();
        }

        String savedbfile = Const.DBPATH ;//  /storage/emulated/0/duanzhao
        try {
            if (!(new File(savedbfile).exists())) {//判断数据库文件是否存在，若不存在则执行导入，否则直接打开数据库
                InputStream is = context.getResources().openRawResource(R.raw.zwave); //欲导入的数据库
                FileOutputStream fos = new FileOutputStream(savedbfile);
                byte[] buffer = new byte[1024];
                int count = 0;
                while ((count = is.read(buffer)) > 0) {
                    fos.write(buffer, 0, count);
                }
                fos.flush();
                fos.close();
                is.close();
            }
            return true;
        } catch (FileNotFoundException e) {
            e.printStackTrace();
        } catch (IOException e) {
            e.printStackTrace();
        }
        return false;

    }

    public static boolean importFile(Context context,String name) {
        //
        String dbPath =  Const.DATA_PATH;///data/data/PACKAGE_NAME
        File dir = new File(dbPath);
        if (!dir.exists()) {
            dir.mkdirs();
        }


        String savedbfile = Const.FILE_PATH + name;
        try {
            if (!(new File(savedbfile).exists())) {//判断数据库文件是否存在，若不存在则执行导入，否则直接打开数据库
                InputStream is = context.getAssets().open(name);
                FileOutputStream fos = new FileOutputStream(savedbfile);
                byte[] buffer = new byte[1024];
                int count = 0;
                while ((count = is.read(buffer)) > 0) {
                    fos.write(buffer, 0, count);
                }
                fos.flush();
                fos.close();
                is.close();
            }
            return true;
        } catch (FileNotFoundException e) {
            e.printStackTrace();
        } catch (IOException e) {
            e.printStackTrace();
        }
        return false;
    }
}
