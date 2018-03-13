package com.askey.firefly.zwave.control.utils;

import android.os.Environment;

/***
 * 常量配置类
 * Company: Chengdu Skysoft Info&Tech Co., Ltd.
 * Copyright ©2014-2018 成都天软信息技术有限公司
 * @since:JDK1.7
 * @version:1.0
 * @see
 * @author  charles
 ***/
public class Const {

	/**
	 * 是否是DEBUG模式
	 */
	public static final boolean DEBUG = true;
	public static final String PACKAGE_NAME = "com.askey.firefly.zwave.control";

	public static final String DATA_PATH = "/data"
			+ Environment.getDataDirectory().getAbsolutePath() + "/"
			+ PACKAGE_NAME  ;

	public static final String ZWCONTROL_CFG_PATH = DATA_PATH + "/" + "zw_api.cfg";
    public static final String FILE_PATH = DATA_PATH + "/";

    public static final String SAVE_NODEINFO_FILE = DATA_PATH + "/" +"zwController_nodeInfo.txt";

	public static final String DBPATH = DATA_PATH + "/" + "zwave.db";
}
