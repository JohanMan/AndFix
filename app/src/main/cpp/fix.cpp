#include <jni.h>
#include <cassert>
#include <cstring>
#include <android/log.h>

#define FIX_CLASS "com/johan/andfix/Fix"

#define TAG "Fix"
#define LOGE(...) __android_log_print(ANDROID_LOG_ERROR,TAG,__VA_ARGS__)

// 保存一个方法的占用内存大小 每个方法占用的内存是相同的
size_t method_length;

/**
 * 计算方法占用的内存大小
 * 计算方式是取两个紧靠的方法内存地址差值：
 * 通过对Art虚拟机源码的分析，我们发现，每一个类的ArtMethod们在内存中是紧密排列在一起的，
 * 所以一个ArtMethod的大小，不就是相邻两个方法所对应的ArtMethod的起始地址的差值
 * method1 和 method2 必须是两个连续的方法 这样计算出来的差值才有效
 */
static void calculateMethodLength(JNIEnv* env, jclass clazz, jobject method1, jobject method2) {
    if (method_length != 0) {
        LOGE("method length %d", method_length);
        return;
    }
    size_t method1Ptr = (size_t) env->FromReflectedMethod(method1);
    size_t method2Ptr = (size_t) env->FromReflectedMethod(method2);
    method_length = method2Ptr - method1Ptr;
}

/**
 * 代替方法
 */
static void replaceMethod(JNIEnv* env, jclass clazz, jobject src, jobject dest) {
    if (method_length == 0) {
        return;
    }
    jmethodID srcMethod = env->FromReflectedMethod(src);
    jmethodID destMethod = env->FromReflectedMethod(dest);
    // 目标方法 代替 原始方法
    // 把 destMethod(修改过来正确的方法)的数据 复制到 srcMethod(原来错误的方法)中 实现方法代替
    // 注意两个方法名和参数要一致
    memcpy(srcMethod, destMethod, method_length);
}

/**
 * 函数映射表
 */
static JNINativeMethod methods[] = {
        /* java方法名 签名 c方法引用 */
        {"calculateMethodLength", "(Ljava/lang/reflect/Method;Ljava/lang/reflect/Method;)V", (void*) calculateMethodLength},
        {"replaceMethod", "(Ljava/lang/reflect/Method;Ljava/lang/reflect/Method;)V", (void*) replaceMethod}
};

/**
 * 动态注册 Native 方法
 */
static jint registerNativeMethods(JNIEnv* env) {
    jclass clazz = env->FindClass(FIX_CLASS);
    if (clazz == NULL) {
        return JNI_FALSE;
    }
    if (env->RegisterNatives(clazz, methods, sizeof(methods) / sizeof(methods[0])) < 0) {
        return JNI_FALSE;
    }
    return JNI_TRUE;
}

/**
 * JNI OnLoad 函数
 */
JNIEXPORT jint JNICALL JNI_OnLoad(JavaVM* vm, void* reserved) {
    JNIEnv* env;
    if (vm->GetEnv((void**) &env, JNI_VERSION_1_4) != JNI_OK) {
        return -1;
    }
    assert(env != NULL);
    if (!registerNativeMethods(env)) {
        return -1;
    }
    return JNI_VERSION_1_4;
}