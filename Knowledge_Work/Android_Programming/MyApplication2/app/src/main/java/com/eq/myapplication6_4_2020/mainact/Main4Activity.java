package com.eq.myapplication6_4_2020.mainact;

import android.app.Notification;
import android.app.NotificationChannel;
import android.app.NotificationManager;
import android.app.PendingIntent;
import android.content.Intent;
import android.graphics.Color;
import android.os.Build;
import android.support.v4.app.NotificationCompat;
import android.support.v4.app.NotificationManagerCompat;
import android.support.v4.app.TaskStackBuilder;
import android.support.v7.app.AppCompatActivity;
import android.os.Bundle;
import android.util.Log;
import android.view.View;

import com.eq.myapplication6_4_2020.R;

public class Main4Activity extends AppCompatActivity {

    public final static  String CHANNEL_ID = "Monojit Notification new Channel";
    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main4);

        createNotificationChannel();
    }

    public void createNotificationChannel() {
        // Create the NotificationChannel, but only on API 26+ because
        // the NotificationChannel class is new and not in the support library



        if (Build.VERSION.SDK_INT >= Build.VERSION_CODES.O) {
            CharSequence name = getString(R.string.channel_name);
            String description = getString(R.string.channel_description);
            int importance = NotificationManager.IMPORTANCE_HIGH;
            NotificationChannel channel = new NotificationChannel(CHANNEL_ID, name, importance);
            channel.setDescription(description);
            // Register the channel with the system; you can't change the importance
            // or other notification behaviors after this
            NotificationManager notificationManager = getSystemService(NotificationManager.class);
            notificationManager.createNotificationChannel(channel);
        }
    }

    public void onSendNotif ( View view )
    {


        Log.i( this.getLocalClassName() ,  String.valueOf(Thread.currentThread().getStackTrace()[2].getLineNumber()) );


        Intent intent = new Intent(this, Main5Activity.class);
       // Intent intent1 = new Intent(this, Main3Activity.class);
      //  Intent intent2 = new Intent(this, MainActivity.class);

       // intent.setFlags(Intent.FLAG_ACTIVITY_);



        TaskStackBuilder taskStackBuilder = TaskStackBuilder.create(this);
        taskStackBuilder.addNextIntentWithParentStack(intent);
        //taskStackBuilder.addNextIntentWithParentStack(intent);
        PendingIntent pendingIntent = taskStackBuilder.getPendingIntent(0,PendingIntent.FLAG_UPDATE_CURRENT);

        Notification builder = new NotificationCompat.Builder(this, CHANNEL_ID)
                .setSmallIcon(R.drawable.notification_icon)
                .setContentTitle("My notification")
                .setContentText("Much longer text that cannot fit one line...")
                //.setLargeIcon(Main2Activity.getActivityInstance().getBitMap (R.drawable. new_image ))
                 .setStyle(new NotificationCompat.BigTextStyle()
                         .bigText("Much longer text that cannot fit one line..."))
             //  .setStyle(new NotificationCompat.BigPictureStyle()
                 //      .bigPicture(Main2Activity.getActivityInstance().getBitMap (R.drawable. new_image )).bigLargeIcon(null))

                //  .bigPicture(BitmapFactory.decodeResource(getResources(), R.drawable.mipmap)))
                .setPriority(NotificationCompat.PRIORITY_HIGH)
                .setContentIntent(pendingIntent).build();

        NotificationManagerCompat notificationManager = NotificationManagerCompat.from(this);

// notificationId is a unique int for each notification that you must define
        notificationManager.notify( 1, builder );
                //.setColor(Color.BLUE)
                //.setDefaults(Notification.DEFAULT_SOUND).build();
        Log.i( this.getLocalClassName() ,  String.valueOf(Thread.currentThread().getStackTrace()[2].getLineNumber()) );


       // Main2Activity.getActivityInstance().notifyAndroid ( builder, this );

       // Main2Activity.getActivityInstance().displayToast( "Sending  notification for Activity4..");
    }

    public PendingIntent getPendingIntent(Intent intent){

        TaskStackBuilder taskStackBuilder = TaskStackBuilder.create(this);
        taskStackBuilder.addNextIntentWithParentStack(intent);
        PendingIntent pendingIntent = taskStackBuilder.getPendingIntent(0,PendingIntent.FLAG_UPDATE_CURRENT);
        return pendingIntent;

    }

    public void onNextActivity ( View view )
    {
        Log.i( this.getLocalClassName() ,  String.valueOf(Thread.currentThread().getStackTrace()[2].getLineNumber()) );
        Intent intent = new Intent( this , Main5Activity.class );
        startActivity( intent );

    }
}
