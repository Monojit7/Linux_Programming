package com.eq.myapplication6_4_2020.mainact;

import android.app.Notification;
import android.app.NotificationChannel;
import android.app.NotificationManager;
import android.app.PendingIntent;
import android.content.Intent;
import android.os.Build;
import android.support.v4.app.NotificationCompat;
import android.support.v4.app.NotificationManagerCompat;
import android.support.v4.app.TaskStackBuilder;
import android.support.v7.app.AppCompatActivity;
import android.os.Bundle;
import android.util.Log;
import android.view.View;
import android.widget.RemoteViews;

import com.eq.myapplication6_4_2020.R;

public class Main5Activity extends AppCompatActivity {

    public static final String CHANNEL_ID = "monojit custom channel" ;

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main5);
        createNotificationChannel();
    }

    public void onSendCustomNotif ( View view )
    {

        Log.i( this.getLocalClassName() ,  String.valueOf(Thread.currentThread().getStackTrace()[2].getLineNumber()) );





        RemoteViews notificationLayout         =   new RemoteViews(getPackageName(), R.layout.noti_small);
        RemoteViews notificationLayoutExpanded =   new RemoteViews(getPackageName(), R.layout.noti_big);

        notificationLayout.setTextViewText(R.id.textcollapse_title, "Hello changed collapsed notification");
        notificationLayoutExpanded.setImageViewResource(R.id.ImageViewExpanded, R.drawable.try_name_new);

        Intent clickedintent = new Intent( this, notification_receiver.class);
        PendingIntent pendingIntent = PendingIntent.getBroadcast(this, 0, clickedintent, 0);
        notificationLayoutExpanded.setOnClickPendingIntent( R.id.ImageViewExpanded, pendingIntent);



        Notification builder = new NotificationCompat.Builder(this, CHANNEL_ID)
                .setSmallIcon(R.drawable.notification_icon)
                .setStyle(new NotificationCompat.DecoratedCustomViewStyle() )
                .setCustomContentView(notificationLayout)
                .setCustomBigContentView(notificationLayoutExpanded) .build();

        NotificationManagerCompat notificationManager = NotificationManagerCompat.from(this);

// notificationId is a unique int for each notification that you must define
        notificationManager.notify( 1, builder );
        //.setColor(Color.BLUE)
        //.setDefaults(Notification.DEFAULT_SOUND).build();
        Log.i( this.getLocalClassName() ,  String.valueOf(Thread.currentThread().getStackTrace()[2].getLineNumber()) );

    }

    public void createNotificationChannel() {
        // Create the NotificationChannel, but only on API 26+ because
        // the NotificationChannel class is new and not in the support library



        if (Build.VERSION.SDK_INT >= Build.VERSION_CODES.O) {
            CharSequence name =    getString(R.string.channel_name_custom);
            String description = getString(R.string.channel_description);
            int importance = NotificationManager.IMPORTANCE_HIGH;
            NotificationChannel channel = new NotificationChannel(CHANNEL_ID, name, importance);
            channel.setDescription(description);
            channel.setShowBadge(true);
            // Register the channel with the system; you can't change the importance
            // or other notification behaviors after this
            NotificationManager notificationManager = getSystemService(NotificationManager.class);
            notificationManager.createNotificationChannel(channel);
        }
    }
}
