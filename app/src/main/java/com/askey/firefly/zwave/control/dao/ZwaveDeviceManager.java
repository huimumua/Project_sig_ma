package com.askey.firefly.zwave.control.dao;

import android.content.Context;
import android.database.sqlite.SQLiteDatabase;


import com.askey.firefly.zwave.control.utils.Const;

import org.greenrobot.greendao.query.QueryBuilder;

import java.util.List;


/**
 * Created by skysoft on 2017/1/11.
 */

public class ZwaveDeviceManager {


    private volatile static ZwaveDeviceManager mInstance;
    private DaoMaster.DevOpenHelper openHelper;
    private Context context;

    private ZwaveDeviceManager(){}
    public ZwaveDeviceManager(Context context) {
        this.context = context;
        openHelper = new DaoMaster.DevOpenHelper(context, Const.DBPATH, null);
    }

    /**
     * 获取单例引用
     *
     * @param context
     * @return
     */
    public static ZwaveDeviceManager getInstance(Context context) {
        if (mInstance == null) {
            synchronized (ZwaveDeviceManager.class) {
                if (mInstance == null) {
                    mInstance = new ZwaveDeviceManager(context);
                }
            }
        }
        return mInstance;
    }

    /**
     * 获取可读数据库
     */
    private SQLiteDatabase getReadableDatabase() {
        if (openHelper == null) {
            openHelper = new DaoMaster.DevOpenHelper(context,  Const.DBPATH, null);
        }
        SQLiteDatabase db = openHelper.getReadableDatabase();
        return db;
    }

    /**
     * 获取可写数据库
     */
    public SQLiteDatabase getWritableDatabase() {
        if (openHelper == null) {
            openHelper = new DaoMaster.DevOpenHelper(context,  Const.DBPATH, null);
        }
        SQLiteDatabase db = openHelper.getWritableDatabase();
        return db;
    }

    /**
     * 插入一条记录
     *
     */
    public void insertZwaveDevice(ZwaveDevice zwaveDevice) {
        ZwaveDeviceDao zwaveDeviceDao = getZwaveDeviceDao();
        zwaveDeviceDao.insert(zwaveDevice);
    }

    /**
     *
     */
    public void insertZwaveDeviceList(List<ZwaveDevice> zwaveDeviceList) {
        if (zwaveDeviceList == null || zwaveDeviceList.isEmpty()) {
            return;
        }
        ZwaveDeviceDao zwaveDeviceDao = getZwaveDeviceDao();
        zwaveDeviceDao.insertInTx(zwaveDeviceList);
    }

    /**
     * 删除一条记录
     *
     * @param id
     */
    public void deleteZwaveDevice(long id) {
        ZwaveDeviceDao zwaveDeviceDao = getZwaveDeviceDao();
        zwaveDeviceDao.deleteByKey(id);
//        User user = userDao.queryBuilder().where(UserDao.Properties.UserId.eq(id)).build().unique();
    }

    /**
     * 删除多条记录
     *
     * @param ids
     */
    public void deleteZwaveDevice(Iterable<Long> ids) {
        ZwaveDeviceDao zwaveDeviceDao = getZwaveDeviceDao();
        zwaveDeviceDao.deleteByKeyInTx(ids);

    }

    /**
     * 删除所有记录
     *
     */
    public void deleteAll() {
        ZwaveDeviceDao zwaveDeviceDao = getZwaveDeviceDao();
        zwaveDeviceDao.deleteAll();

    }

    /**
     * 更新一条记录
     *
     */
    public void updateZwaveDevice(ZwaveDevice zwaveDevice) {
        ZwaveDeviceDao zwaveDeviceDao = getZwaveDeviceDao();
        ZwaveDevice upDateZwaveDevice = zwaveDeviceDao.queryBuilder()
                .where(ZwaveDeviceDao.Properties.ZwaveId.eq(zwaveDevice.getZwaveId())).build().unique();
        if (upDateZwaveDevice != null) {
            upDateZwaveDevice.setHomeId(zwaveDevice.getHomeId());
            upDateZwaveDevice.setNodeId(zwaveDevice.getNodeId());
            upDateZwaveDevice.setName(zwaveDevice.getName());
            upDateZwaveDevice.setNodeInfo(zwaveDevice.getNodeInfo());
            upDateZwaveDevice.setAddress(zwaveDevice.getAddress());
            zwaveDeviceDao.update(upDateZwaveDevice);
        }
    }

    /**
     */
    public List<ZwaveDevice> queryZwaveDeviceList() {
        ZwaveDeviceDao zwaveDeviceDao = getZwaveDeviceDao();
        QueryBuilder<ZwaveDevice> qb = zwaveDeviceDao.queryBuilder();
        List<ZwaveDevice> list = qb.list();
        return list;
    }

    /**
     */
    public ZwaveDevice queryZwaveDevices(long id) {
        ZwaveDeviceDao zwaveDeviceDao = getZwaveDeviceDao();
        QueryBuilder<ZwaveDevice> qb = zwaveDeviceDao.queryBuilder();
        qb.where(ZwaveDeviceDao.Properties.ZwaveId.eq(id));
        List<ZwaveDevice> list = qb.list();
        return list.get(0);
    }
    /**
     */
    public ZwaveDevice queryZwaveDevices(String homeId,int nodeId) {
        ZwaveDeviceDao zwaveDeviceDao = getZwaveDeviceDao();
        QueryBuilder<ZwaveDevice> qb = zwaveDeviceDao.queryBuilder();
        qb.where(ZwaveDeviceDao.Properties.HomeId.eq(homeId),ZwaveDeviceDao.Properties.NodeId.eq(nodeId));
//        qb.and(ZwaveDeviceDao.Properties.HomeId.eq(homeId),ZwaveDeviceDao.Properties.NodeId.eq(nodeId));
        List<ZwaveDevice> list = qb.list();
        if (list != null && list.size() > 0) {
            return list.get(0);
        } else {
            return null;
        }
    }

    public ZwaveDeviceDao getZwaveDeviceDao() {
        DaoMaster daoMaster = new DaoMaster(getWritableDatabase());
        DaoSession daoSession = daoMaster.newSession();
        ZwaveDeviceDao zwaveDevice = daoSession.getZwaveDeviceDao();
        return  zwaveDevice;
    }
}
