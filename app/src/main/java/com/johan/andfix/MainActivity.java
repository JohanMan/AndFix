package com.johan.andfix;

import android.os.Bundle;
import android.support.v7.app.AppCompatActivity;
import android.view.View;

import java.lang.reflect.Method;

public class MainActivity extends AppCompatActivity {

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);
        testFix();
    }

    public void fix(View view) {
        Error error = new Error();
        error.say();
    }

    private void testFix() {
        try {
            Class errorClass = Error.class;
            Method errorMethod = errorClass.getMethod("say");
            Class rightClass = Right.class;
            Method rightMethod = rightClass.getMethod("say");
            Fix.fix(errorMethod, rightMethod);
        } catch (NoSuchMethodException e) {
            e.printStackTrace();
        }

    }

}
