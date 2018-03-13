package com.askey.firefly.zwave.control.utils;

import android.app.Activity;
import android.content.Context;
import android.os.Environment;
import android.widget.TextView;
import android.widget.Toast;

import java.io.BufferedWriter;
import java.io.File;
import java.io.FileOutputStream;
import java.io.OutputStreamWriter;
import java.text.SimpleDateFormat;
import java.util.Date;

/***
 * 日志工具类
 * Company: Chengdu Skysoft Info&Tech Co., Ltd.
 * Copyright ©2014-2018 成都天软信息技术有限公司
 * @since:JDK1.7
 * @version:1.0
 * @see
 * @author  charles
 ***/
public class Logg {

	public static final boolean DEBUG = true;

	public static void cstdr(String TAG, String msg) {
		if (DEBUG) {
			android.util.Log.i("cstdr", TAG + "~~~" + msg);
		}
	}

	/**
	 * @return void </br>
	 * @throws </br>
	 * @about version 1.0
	 * @Description Send an INFO log message
	 */
	public static void i(String tag, String msg) {

		if (Const.DEBUG) {
			android.util.Log.i(tag, msg);
		}
	}

	/**
	 * @return void </br>
	 * @throws </br>
	 * @about version 1.0
	 * @Description Send a DEBUG log message
	 */
	public static void d(String tag, String msg) {

		if (Const.DEBUG) {
			android.util.Log.d(tag, msg);
		}
	}


	/**
	 * @return void </br>
	 * @throws </br>
	 * @about version 1.0
	 * @Description Send a WARN log message.
	 */
	public static void w(String tag, String msg) {

		if (Const.DEBUG) {
			android.util.Log.w(tag, msg);
		}
	}

	/**
	 * @return void </br>
	 * @throws </br>
	 * @about version 1.0
	 * @Description Send an ERROR log message.
	 */
	public static void e(String tag, String msg) {

		if (Const.DEBUG) {
			android.util.Log.e(tag, msg);
		}
	}

	/**
	 * @return void </br>
	 * @throws </br>
	 * @about version 1.0
	 * @Description Send an ERROR log message.
	 */
	public static void e(String tag, String msg, Throwable tr) {

		if (Const.DEBUG) {
			android.util.Log.e(tag, msg, tr);
		}
	}

	/**
	 * @return void </br>
	 * @throws </br>
	 * @about version 1.0
	 * @Description Send a VERBOSE log message.
	 */
	public static void v(String tag, String msg) {

		if (Const.DEBUG) {
			android.util.Log.v(tag, msg);
		}
	}

	public static void showToast(final Context mContext, final String content) {
		((Activity) mContext).runOnUiThread(new Runnable() {

			@Override
			public void run() {
				Toast toast = Toast.makeText(mContext, content,
						Toast.LENGTH_SHORT);
				toast.show();

			}
		});

	}

	public static void setTextViewContent(final Activity activity,
                                          final TextView textView, final String content) {
		activity.runOnUiThread(new Runnable() {

			@Override
			public void run() {
				if (textView != null) {
					textView.setText(content);
				}
			}
		});
	}

	public static void resizePerformanceResultToSdCard(String result, String inputUri, String outputUri) {
		String filePath = Environment.getExternalStorageDirectory() + "/NasGwServer/ResizePerformance.txt";
		File out = new File(filePath);
		out.getParentFile().mkdir();

		SimpleDateFormat df = new SimpleDateFormat("yyyy/MM/dd HH:mm:ss");
		Date date = new Date(System.currentTimeMillis());

		FileOutputStream fos;
		try {
			fos = new FileOutputStream(out, true);
			OutputStreamWriter osw = new OutputStreamWriter(fos, "UTF-8");
			BufferedWriter bw = new BufferedWriter(osw);
			String str = "----------" + df.format(date) + " (" + inputUri + ") " + "-----START------\n";
			str += df.format(date) + " (" + outputUri + ") :" + result + "\n";
			str += "----------" + df.format(date) + " (" + inputUri + ") " + "------END-------\n";
			bw.write(str);
			bw.flush();
			bw.close();
		} catch (Exception e) {
			e.printStackTrace();
		}
	}


}
