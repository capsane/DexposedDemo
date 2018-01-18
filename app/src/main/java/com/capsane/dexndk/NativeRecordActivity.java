package com.capsane.dexndk;

import android.support.v7.app.AppCompatActivity;
import android.os.Bundle;
import android.view.View;
import android.widget.Button;
import android.widget.TextView;

public class NativeRecordActivity extends AppCompatActivity implements View.OnClickListener {

    private static final String TAG = "NativeRecordActivity";

    private TextView textViewNative;

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_native_record);

        Button buttonStartRecordNative = findViewById(R.id.button_start_record_native);
        Button buttonStopRecordNative = findViewById(R.id.button_stop_record_native);
        Button buttonHookNative = findViewById(R.id.button_hook_native);

        buttonStartRecordNative.setOnClickListener(this);
        buttonStopRecordNative.setOnClickListener(this);
        buttonHookNative.setOnClickListener(this);

    }

    @Override
    public void onClick(View view) {
        switch (view.getId()) {
            case R.id.button_start_record_native:
                break;

            case R.id.button_stop_record_native:
                break;

            case R.id.button_start_play_native:
                break;

            case R.id.button_stop_play_native:
                break;

            case R.id.button_hook_native:
                break;

            default:
                break;
        }
    }
}
