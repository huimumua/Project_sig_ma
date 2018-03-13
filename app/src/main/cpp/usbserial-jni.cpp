#include <string.h>
#include <stdint.h>
#include <jni.h>
#include <android/log.h>

#define LOG_TAG "USBSERIAL-JNI"

#define ALOGI(...) __android_log_print(ANDROID_LOG_INFO, LOG_TAG, __VA_ARGS__)
#define ALOGD(...) __android_log_print(ANDROID_LOG_DEBUG, LOG_TAG, __VA_ARGS__)
#define ALOGE(...) __android_log_print(ANDROID_LOG_ERROR, LOG_TAG, __VA_ARGS__)
#define ALOGW(...) __android_log_print(ANDROID_LOG_WARN, LOG_TAG, __VA_ARGS__)

#define NELEM(x) ((int) (sizeof(x) / sizeof((x)[0])))

static jclass  UsbSerialClass;
static jobject UsbSerialObject;
static JavaVM  *UsbSerialVM = NULL;

static jmethodID openMethodID  = NULL;
static jmethodID writeMethodID = NULL;
static jmethodID readMethodID  = NULL;
static jmethodID checkMethodID = NULL;
static jmethodID closeMethodID = NULL;

static jmethodID openAPMMethodID          = NULL;
static jmethodID writeAPMMethodID         = NULL;
static jmethodID readAPMMethodID          = NULL;
static jmethodID getAPMBcdDeviceMethodID  = NULL;
static jmethodID closeAPMMethodID         = NULL;

static void check_and_clear_exceptions(JNIEnv* env, const char* method_name)
{
    if (!env->ExceptionCheck())
    {
        return;
    }

    ALOGE("An exception was thrown by '%s'.", method_name);
    env->ExceptionClear();
}

static JNIEnv* getJNIEnv(int* needsDetach)
{
    *needsDetach = 0;

    JNIEnv* env;

    if(UsbSerialVM == NULL)
    {
        return NULL;
    }

    if (UsbSerialVM->GetEnv((void**) &env, JNI_VERSION_1_4) != JNI_OK)
    {
        env =  NULL;
    }

    if (env == NULL)
    {
        JavaVMAttachArgs args = {JNI_VERSION_1_4, NULL, NULL};

        int result = UsbSerialVM->AttachCurrentThread(&env, (void*) &args);

        if (result != JNI_OK)
        {
            ALOGE("thread attach failed: %#x", result);
            return NULL;
        }

        *needsDetach = 1;
    }

    return env;
}

static void detachJNI()
{
    if(UsbSerialVM == NULL)
    {
        return;
    }

    int result = UsbSerialVM->DetachCurrentThread();

    if (result != JNI_OK)
    {
        ALOGE("thread detach failed: %#x", result);
    }
}

extern "C" int UsbSerial_Destroy()
{
    int needDetach;

    JNIEnv* env = getJNIEnv(&needDetach);

    if(env == NULL)
    {
        return -1;
    }

    if(UsbSerialObject != NULL)
    {
        env->DeleteGlobalRef(UsbSerialObject);
    }

    if(UsbSerialClass != NULL)
    {
        env->DeleteGlobalRef(UsbSerialClass);
    }

    if(needDetach)
    {
        detachJNI();
    }

    return 0;
}

extern "C" int UsbSerial_Open()
{
    if (UsbSerialClass == NULL || UsbSerialObject == NULL)
    {
        return -1;
    }

    int needsDetach, result;

    JNIEnv *env = getJNIEnv(&needsDetach);

    if(openMethodID == NULL)
    {
        openMethodID = env->GetMethodID(UsbSerialClass, "Open", "()I");

        if(openMethodID == NULL)
        {
            result = -1;

            if(needsDetach)
            {
                detachJNI();
            }

            return result;
        }
    }

    result = env->CallIntMethod(UsbSerialObject, openMethodID);

    check_and_clear_exceptions(env, __FUNCTION__);

    if(needsDetach)
    {
        detachJNI();
    }

    return result;
}

extern "C" void UsbSerial_WriteData(uint8_t *data, int size)
{
    if(UsbSerialClass == NULL || UsbSerialObject == NULL)
        return;

    int needsDetach;
    JNIEnv *env = getJNIEnv(&needsDetach);

    if(env == NULL)
    {
        return;
    }

    if(writeMethodID == NULL)
    {
        writeMethodID = env->GetMethodID(UsbSerialClass, "Write", "([B)V");

        if(writeMethodID == NULL)
        {
            if(needsDetach)
            {
                detachJNI();
            }

            return;
        }
    }

    jbyteArray bytes = env->NewByteArray(size);
    env->SetByteArrayRegion(bytes, 0, size, (jbyte*)data);
    env->CallVoidMethod(UsbSerialObject, writeMethodID, bytes);
    check_and_clear_exceptions(env, __FUNCTION__);
    env->DeleteLocalRef(bytes);

    if(needsDetach)
    {
        detachJNI();
    }
}

extern "C" int UsbSerial_ReadData(uint8_t *data, int len)
{
    if(UsbSerialClass == NULL || UsbSerialObject == NULL)
        return -1;

    int needsDetach;
    JNIEnv *env = getJNIEnv(&needsDetach);

    if(env == NULL)
    {
        return -1;
    }

    jint size;

    if(readMethodID == NULL)
    {
        readMethodID = env->GetMethodID(UsbSerialClass, "Read", "([BI)I");

        if(readMethodID == NULL)
        {
            size = -1;

            if(needsDetach)
            {
                detachJNI();
            }

            return size;
        }
    }

    jbyteArray bytes = env->NewByteArray(len);
    size = env->CallIntMethod(UsbSerialObject, readMethodID, bytes, len);
    check_and_clear_exceptions(env, __FUNCTION__);
    env->GetByteArrayRegion(bytes, 0, size, (jbyte*)data);
    env->DeleteLocalRef(bytes);

    if(needsDetach)
    {
        detachJNI();
    }

    return size;
}

extern "C" int UsbSerial_Check()
{
    if(UsbSerialClass == NULL || UsbSerialObject == NULL)
        return -1;

    int needsDetach, result;
    JNIEnv *env = getJNIEnv(&needsDetach);

    if(env == NULL)
    {
        return -1;
    }

    if(checkMethodID == NULL)
    {
        checkMethodID = env->GetMethodID(UsbSerialClass, "Check", "()I");

        if(checkMethodID == NULL)
        {
            result = -1;

            if(needsDetach)
            {
                detachJNI();
            }

            return result;
        }
    }

    result = env->CallIntMethod(UsbSerialObject, checkMethodID);
    check_and_clear_exceptions(env, __FUNCTION__);

    if(needsDetach)
    {
        detachJNI();
    }

    return result;
}

extern "C" void UsbSerial_Close()
{
    if(UsbSerialClass == NULL || UsbSerialObject == NULL)
        return;

    int needsDetach;
    JNIEnv *env = getJNIEnv(&needsDetach);

    if(env == NULL)
    {
        return;
    }

    if(closeMethodID == NULL)
    {
        closeMethodID = env->GetMethodID(UsbSerialClass, "Close", "()V");

        if(closeMethodID == NULL)
        {
            if(needsDetach)
            {
                detachJNI();
            }

            return;
        }
    }

    env->CallVoidMethod(UsbSerialObject, closeMethodID);
    check_and_clear_exceptions(env, __FUNCTION__);

    if(needsDetach)
    {
        detachJNI();
    }
}

extern "C" int UsbSerial_APMOpen()
{
    if (UsbSerialClass == NULL || UsbSerialObject == NULL)
    {
        return -1;
    }

    int needsDetach, result;

    JNIEnv *env = getJNIEnv(&needsDetach);

    if(openAPMMethodID == NULL)
    {
        openAPMMethodID = env->GetMethodID(UsbSerialClass, "OpenApm", "()I");

        if(openAPMMethodID == NULL)
        {
            result = -1;

            if(needsDetach)
            {
                detachJNI();
            }

            return result;
        }
    }

    result = env->CallIntMethod(UsbSerialObject, openAPMMethodID);

    check_and_clear_exceptions(env, __FUNCTION__);

    if(needsDetach)
    {
        detachJNI();
    }

    return result;
}

extern "C" void UsbSerial_APMWriteData(uint8_t *data, int size)
{
    if(UsbSerialClass == NULL || UsbSerialObject == NULL)
        return;

    int needsDetach;
    JNIEnv *env = getJNIEnv(&needsDetach);

    if(env == NULL)
    {
        return;
    }

    if(writeAPMMethodID == NULL)
    {
        writeAPMMethodID = env->GetMethodID(UsbSerialClass, "WriteApm", "([B)V");

        if(writeAPMMethodID == NULL)
        {
            if(needsDetach)
            {
                detachJNI();
            }

            return;
        }
    }

    jbyteArray bytes = env->NewByteArray(size);
    env->SetByteArrayRegion(bytes, 0, size, (jbyte*)data);
    env->CallVoidMethod(UsbSerialObject, writeAPMMethodID, bytes);
    check_and_clear_exceptions(env, __FUNCTION__);
    env->DeleteLocalRef(bytes);

    if(needsDetach)
    {
        detachJNI();
    }
}

extern "C" int UsbSerial_APMReadData(uint8_t *data, int len)
{
    if(UsbSerialClass == NULL || UsbSerialObject == NULL)
        return -1;

    int needsDetach;
    JNIEnv *env = getJNIEnv(&needsDetach);

    if(env == NULL)
    {
        return -1;
    }

    jint size;

    if(readAPMMethodID == NULL)
    {
        readAPMMethodID = env->GetMethodID(UsbSerialClass, "ReadApm", "([BI)I");

        if(readAPMMethodID == NULL)
        {
            size = -1;

            if(needsDetach)
            {
                detachJNI();
            }

            return size;
        }
    }

    jbyteArray bytes = env->NewByteArray(len);
    size = env->CallIntMethod(UsbSerialObject, readAPMMethodID, bytes, len);
    check_and_clear_exceptions(env, __FUNCTION__);
    env->GetByteArrayRegion(bytes, 0, size, (jbyte*)data);
    env->DeleteLocalRef(bytes);

    if(needsDetach)
    {
        detachJNI();
    }

    return size;
}

extern "C" int UsbSerial_APMGetBcdDevice()
{
    if(UsbSerialClass == NULL || UsbSerialObject == NULL)
        return -1;

    int needsDetach;
    JNIEnv *env = getJNIEnv(&needsDetach);

    if(env == NULL)
    {
        return -1;
    }

    if(getAPMBcdDeviceMethodID == NULL)
    {
        getAPMBcdDeviceMethodID = env->GetMethodID(UsbSerialClass, "GetApmBcdDevice", "()I");

        if(getAPMBcdDeviceMethodID == NULL)
        {
            if(needsDetach)
            {
                detachJNI();
            }

            return -1;
        }
    }

    int deviceId = env->CallIntMethod(UsbSerialObject, getAPMBcdDeviceMethodID);
    check_and_clear_exceptions(env, __FUNCTION__);

    if(needsDetach)
    {
        detachJNI();
    }

    return deviceId;
}

extern "C" void UsbSerial_APMClose()
{
    if(UsbSerialClass == NULL || UsbSerialObject == NULL)
        return;

    int needsDetach;
    JNIEnv *env = getJNIEnv(&needsDetach);

    if(env == NULL)
    {
        return;
    }

    if(closeAPMMethodID == NULL)
    {
        closeAPMMethodID = env->GetMethodID(UsbSerialClass, "CloseApm", "()V");

        if(closeAPMMethodID == NULL)
        {
            if(needsDetach)
            {
                detachJNI();
            }

            return;
        }
    }

    env->CallVoidMethod(UsbSerialObject, closeAPMMethodID);
    check_and_clear_exceptions(env, __FUNCTION__);

    if(needsDetach)
    {
        detachJNI();
    }
}

static void set_usbserial_object(JNIEnv *env, jobject object)
{
    UsbSerialObject = env->NewGlobalRef(object);
}

static const JNINativeMethod gMethods[] = {
        {"UsbSerial_Set_Object", "()V", (void *)set_usbserial_object},
};

static int registerNativeMethods(JNIEnv* env, const char* className,
                                 const JNINativeMethod* methods, int numMethods)
{
    jclass clazz;
    clazz = env->FindClass(className);

    if (clazz == NULL)
    {
        return JNI_FALSE;
    }

    if (env->RegisterNatives(clazz, methods, numMethods) < 0)
    {
        return JNI_FALSE;
    }

    UsbSerialClass = (jclass)env->NewGlobalRef(clazz);

    env->DeleteLocalRef(clazz);

    return JNI_TRUE;
}

JNIEXPORT jint JNI_OnLoad(JavaVM* g_javaVM, void* reserved)
{
    int status;

    if(g_javaVM == NULL)
    {
        return -1;
    }

    JNIEnv *env = NULL;

    status = g_javaVM->GetEnv((void **) &env, JNI_VERSION_1_4);

    if(status < 0)
    {
        status = g_javaVM->AttachCurrentThread(&env, NULL);

        if(status < 0)
        {
            env = NULL;
        }

        return -1;
    }

    if(registerNativeMethods(env, "com/askey/firefly/zwave/control/thirdparty/usbserial/UsbSerial",
                             gMethods, NELEM(gMethods)) < 0)
    {
        return -1;
    }

    UsbSerialVM = g_javaVM;

    return JNI_VERSION_1_4;
}