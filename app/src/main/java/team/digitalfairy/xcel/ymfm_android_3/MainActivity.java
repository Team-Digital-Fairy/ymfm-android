package team.digitalfairy.xcel.ymfm_android_3;

import androidx.appcompat.app.AppCompatActivity;

import android.os.Bundle;

import java.io.File;
import java.util.concurrent.Executors;
import java.util.concurrent.ScheduledExecutorService;
import java.util.concurrent.TimeUnit;

import team.digitalfairy.xcel.ymfm_android_3.VGMParser;

public class MainActivity extends AppCompatActivity {

    private final ScheduledExecutorService ex = Executors.newScheduledThreadPool(2);

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);

        //File f = new File("/sdcard/test.vgm");

        // Given the permission, open some local file
        //VGMParser vp = new VGMParser(f);
        // Periodic Task
        //ex.scheduleAtFixedRate(vp,0,22, TimeUnit.MICROSECONDS);

    }
}