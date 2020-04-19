package com.eq.SecondApp;

import androidx.appcompat.app.ActionBar;
import androidx.appcompat.app.AppCompatActivity;

import android.app.Notification;
import android.app.NotificationChannel;
import android.app.NotificationManager;
import android.app.PendingIntent;
import android.content.Intent;
import android.graphics.drawable.Icon;
import android.os.Build;
import android.os.Bundle;
import android.util.Log;
import android.view.Gravity;
import android.view.View;
import android.widget.RadioButton;
import android.widget.Toast;

import androidx.appcompat.widget.Toolbar;

public class MainActivity extends AppCompatActivity {

    Toolbar myToolbar = null;
    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);
        myToolbar = findViewById(R.id.my_toolbar);
        setSupportActionBar(myToolbar);

        Log.i( this.getLocalClassName() ,  Thread.currentThread().getStackTrace()[2].getMethodName() );
    }



    public void onNext ( View view )
    {
        Log.i( this.getLocalClassName() ,  Thread.currentThread().getStackTrace()[2].getMethodName() );

    }

    public void onShowBubble ( View view )
    {
        Log.i( this.getLocalClassName() ,  Thread.currentThread().getStackTrace()[2].getMethodName() );




    }

    public void onRadioButtonClicked ( View view )
    {
        boolean checked = ((RadioButton) view).isChecked();
        ActionBar mActionBar = getSupportActionBar();
        Log.i( this.getLocalClassName() ,  Thread.currentThread().getStackTrace()[2].getMethodName() );
        switch(view.getId()) {
            case R.id.show_id:
                if (checked) {
                    // Pirates are the best
                    mActionBar.show();
                    displayToast("showing ActionBar");
                }
                    break;
            case R.id.hide_id:
                if (checked) {
                    // Ninjas rule
                    mActionBar.hide();
                    displayToast("Hiding ActionBar");
                }
                    break;
        }


    }

    public void displayToast ( String sData)
    {
        Toast t = Toast.makeText(getApplicationContext(),
                sData,
                Toast.LENGTH_SHORT);
        t.setGravity(Gravity.TOP | Gravity.CENTER_HORIZONTAL, 0, 180);
        t.show();

    }
}
