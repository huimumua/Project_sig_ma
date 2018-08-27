package com.askey.firefly.zwave.control.utils;

import android.content.Context;
import android.content.SharedPreferences;

import java.lang.reflect.InvocationTargetException;
import java.lang.reflect.Method;
import java.util.Map;

public class PreferencesUtils {

    public static final String FILE_NAME = "zwave_share_data";
 
    /**
     * 
     * @param context 
     * @param key 
     * @param object 
     */ 
    public static void put(Context context, String key, Object object) {
 
        SharedPreferences sp = context.getSharedPreferences(FILE_NAME,
                Context.MODE_PRIVATE);
        SharedPreferences.Editor editor = sp.edit();
 
        if (object instanceof String) {
            editor.putString(key, (String) object);
        } else if (object instanceof Integer) {
            editor.putInt(key, (Integer) object);
        } else if (object instanceof Boolean) {
            editor.putBoolean(key, (Boolean) object);
        } else if (object instanceof Float) {
            editor.putFloat(key, (Float) object);
        } else if (object instanceof Long) {
            editor.putLong(key, (Long) object);
        } else { 
            editor.putString(key, object.toString());
        } 
        SharedPreferencesCompat.apply(editor);
    } 
 
 
    /**
     * 
     * @param context 
     * @param key 
     * @param defaultObject 
     * @return 
     */ 
    public static Object get(Context context, String key, Object defaultObject)
    { 
        SharedPreferences sp = context.getSharedPreferences(FILE_NAME,
                Context.MODE_PRIVATE);
 
        if (defaultObject instanceof String) {
            return sp.getString(key, (String) defaultObject);
        } else if (defaultObject instanceof Integer) {
            return sp.getInt(key, (Integer) defaultObject);
        } else if (defaultObject instanceof Boolean) {
            return sp.getBoolean(key, (Boolean) defaultObject);
        } else if (defaultObject instanceof Float) {
            return sp.getFloat(key, (Float) defaultObject);
        } else if (defaultObject instanceof Long) {
            return sp.getLong(key, (Long) defaultObject);
        } 
        return null; 
    } 
 
    /**
     * @param context 
     * @param key 
     */ 
    public static void remove(Context context, String key) {
        SharedPreferences sp = context.getSharedPreferences(FILE_NAME,
                Context.MODE_PRIVATE);
        SharedPreferences.Editor editor = sp.edit();
        editor.remove(key);
        SharedPreferencesCompat.apply(editor);
    } 
 
    /**
     * @param context 
     */ 
    public static void clear(Context context) {
        SharedPreferences sp = context.getSharedPreferences(FILE_NAME,
                Context.MODE_PRIVATE);
        SharedPreferences.Editor editor = sp.edit();
        editor.clear();
        SharedPreferencesCompat.apply(editor);
    } 
 
    /**
     * @param context 
     * @param key 
     * @return 
     */ 
    public static boolean contains(Context context, String key) {
        SharedPreferences sp = context.getSharedPreferences(FILE_NAME,
                Context.MODE_PRIVATE);
        return sp.contains(key);
    } 
 
    /**
     * 
     * @param context 
     * @return 
     */ 
    public static Map<String, ?> getAll(Context context) {
        SharedPreferences sp = context.getSharedPreferences(FILE_NAME,
                Context.MODE_PRIVATE);
        return sp.getAll();
    } 
 
    /**
     * 
     * @author zhy 
     * 
     */ 
    private static class SharedPreferencesCompat { 
        private static final Method applyMethod = findApplyMethod();

        @SuppressWarnings({ "unchecked", "rawtypes" })
        private static Method findApplyMethod() {
            try { 
                Class clz = SharedPreferences.Editor.class;
                return clz.getMethod("apply");
            } catch (NoSuchMethodException e) {
            } 
            return null; 
        } 
 
        /**
         * 
         * @param editor 
         */ 
        public static void apply(SharedPreferences.Editor editor) {
            try {
                if (applyMethod != null) {
                    applyMethod.invoke(editor);
                    return; 
                } 
            } catch (IllegalArgumentException e) {
            } catch (IllegalAccessException e) {
            } catch (InvocationTargetException e) {
            }
            editor.commit();
        } 
    } 
}