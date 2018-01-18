package com.capsane.dexposeddemo;


import android.os.Environment;
import android.support.v7.app.AppCompatActivity;
import android.os.Bundle;
import android.util.Log;
import android.widget.TextView;
import android.view.View;
import android.widget.Button;
import android.widget.Toast;

import com.taobao.android.dexposed.DexposedBridge;
import com.taobao.android.dexposed.XC_MethodReplacement;




/**
 * @author capsane
 */

public class MainActivity extends AppCompatActivity implements View.OnClickListener {

    // Used to load the 'native-lib' library on application startup.
    static {
        System.loadLibrary("native-lib");
    }

    private AudioRecordManger mAudioRecordManger;

    private AudioTrackManager mAudioTrackManager;

    private TextView textView;

    private static final String TAG = "MainActivity";

    public static final String DIR = "/sdcard/data/";

    public static final String PATH = DIR + "DexNDK.pcm";



    @Override
    protected void onCreate(Bundle savedInstanceState) {

        // Check whether current device is supported (also initialize Dexposed framework if not yet)
        if (DexposedBridge.canDexposed(this)) {
            Toast.makeText(this, "can be dexposed !", Toast.LENGTH_SHORT).show();
        }

        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);

        mAudioRecordManger = AudioRecordManger.getInstance();

        mAudioTrackManager = AudioTrackManager.getInstance();

        Button buttonStartRecord = findViewById(R.id.button_start_record);
        Button buttonStopRecord = findViewById(R.id.button_stop_record);
        Button buttonStartPlay = findViewById(R.id.button_start_play);
        Button buttonStopPlay = findViewById(R.id.button_stop_play);
        Button buttonHook = findViewById(R.id.button_hook);
        buttonStartRecord.setOnClickListener(this);
        buttonStopRecord.setOnClickListener(this);
        buttonStartPlay.setOnClickListener(this);
        buttonStopPlay.setOnClickListener(this);
        buttonHook.setOnClickListener(this);

        textView = findViewById(R.id.text_view);
        textView.setText(stringFromJNI());
        textView.append("\n");
        textView.append("录音文件： " + PATH + "\n");

    }

    @Override
    public void onClick(View view) {
        switch (view.getId()) {
            case R.id.button_start_record:
                Log.e(TAG, "click start recording");
                mAudioRecordManger.startRecord(PATH);
                break;

            case R.id.button_stop_record:
                Log.e(TAG, "click stop recording");
                mAudioRecordManger.stopRecord();
                break;

            case R.id.button_start_play:
                Log.e(TAG, "click start playing");
                mAudioTrackManager.startPlay();
                break;

            case R.id.button_stop_play:
                Log.e(TAG, "click stop playing");
                mAudioTrackManager.stopPlay();
                break;

            case R.id.button_hook:
                Log.e(TAG, "click hook...");
                Toast.makeText(MainActivity.this, "hook...", Toast.LENGTH_SHORT).show();
                hookSystemLog(view);
//                mAudioRecordManger.hookAudioStartRecording();
//                mAudioRecordManger.hookAudioStartRecordingToJNI();
                break;

            default:
                break;

        }
    }


    /**
     * Replace the system Log.e method. The original hooked method is
     * public static int e(String tag, String msg)
     * @param view
     */
    public void hookSystemLog(View view) {
        // 参数含义： Java类，hooke method name, param1, param2..., XC_MethodReplacement()
        DexposedBridge.findAndHookMethod(Log.class, "e", String.class, String.class, new XC_MethodReplacement() {
            @Override
            protected Object replaceHookedMethod(MethodHookParam param) throws Throwable {
                String tag = (String) param.args[0];    // 原函数的第1个参数
                String msg = (String) param.args[1];    // 原函数的第2个参数
                textView.append(tag + "::::" + msg + "\n");
                // 原函数的返回值，除非原函数返回值类型为void,否则不能省略或者返回null
                return new Integer(1);
            }
        });
    }




    /**
     * A native method that is implemented by the 'native-lib' native library,
     * which is packaged with this application.
     */
    public native String stringFromJNI();

//    private native final int native_start(int syncEvent, int sessionId);

}
