package com.eq.myapplication6_4_2020.mainact;

import android.content.BroadcastReceiver;
import android.content.Context;
import android.content.Intent;
import android.support.v4.app.NotificationManagerCompat;
import android.view.Gravity;
import android.widget.Toast;

public class notification_receiver extends BroadcastReceiver {

    private static  Context receiver_context = null;
    @Override
    public void onReceive(Context context, Intent intent) {
        // TODO: This method is called when the BroadcastReceiver is receiving

        receiver_context = context ;
        displayToast ("Custom Notification clicked");

        NotificationManagerCompat notificationManager = NotificationManagerCompat.from(context);
        notificationManager.cancel(1);

    }

    public void displayToast ( String sData)
    {
        Toast t = Toast.makeText(receiver_context,
                sData,
                Toast.LENGTH_SHORT);
        t.setGravity(Gravity.TOP | Gravity.CENTER_HORIZONTAL, 0, 180);
        t.show();

    }
}
