package com.johan.andfix;

import java.lang.reflect.Method;

/**
 * Created by johan on 2019/2/22.
 */

public class Fix {

    static {
        System.loadLibrary("fix");
    }

    public static void fix(Method src, Method dest) {
        try {
            calculateMethodLength();
            replaceMethod(src, dest);
        } catch (NoSuchMethodException e) {
            e.printStackTrace();
        }
    }

    private static void calculateMethodLength() throws NoSuchMethodException {
        Class helperClass = FixHelper.class;
        Method method1 = helperClass.getMethod("method1");
        Method method2 = helperClass.getMethod("method2");
        calculateMethodLength(method1, method2);
    }

    private static native void calculateMethodLength(Method method1, Method method2);

    private static native void replaceMethod(Method src, Method dest);

}
