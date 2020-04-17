package com.eq.myapplication6_4_2020.mainact;

import android.app.Notification;
import android.app.NotificationChannel;
import android.app.NotificationManager;
import android.app.PendingIntent;
import android.content.Context;
import android.content.Intent;
import android.graphics.Bitmap;
import android.graphics.BitmapFactory;
import android.graphics.Color;
import android.os.Build;
import android.support.v4.app.NotificationCompat;
import android.support.v4.app.NotificationManagerCompat;
import android.support.v7.app.AppCompatActivity;
import android.os.Bundle;
import android.util.Log;
import android.view.Gravity;
import android.view.View;
import android.widget.Toast;

import com.eq.myapplication6_4_2020.R;

import java.io.IOException;
import java.io.InputStream;
import java.net.HttpURLConnection;
import java.net.MalformedURLException;
import java.net.URL;
import java.util.concurrent.atomic.AtomicInteger;

public class Main2Activity extends AppCompatActivity {

    public final static  String CHANNEL_ID = "Monojit Notification Channel";
    private final static AtomicInteger c = new AtomicInteger(0);
    private static Main2Activity instance;
    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        instance = this;
        setContentView(R.layout.activity_main2);
        createNotificationChannel();
        Log.i( this.getLocalClassName() ,  String.valueOf(Thread.currentThread().getStackTrace()[2].getLineNumber()) );
    }

    public static Main2Activity getActivityInstance ()
    {
       return instance;
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

    public void onSendNotifaction (View view )
    {
        Log.i( this.getLocalClassName() ,  String.valueOf(Thread.currentThread().getStackTrace()[2].getLineNumber()) );


        Intent intent = new Intent(this, MainActivity.class);
        intent.setFlags(Intent.FLAG_ACTIVITY_NEW_TASK | Intent.FLAG_ACTIVITY_CLEAR_TASK);
        PendingIntent pendingIntent = PendingIntent.getActivity(this, 0, intent, 0);

      // Bitmap myBitmap = BitmapFactory.decodeResource(getResources(), R.drawable.new_image);
       // Bitmap myBitmap = BitmapFactory.decodeFile("/Users/monojitchatterjee/Downloads/imageview_sample_image");
       /* try {
            Log.i( this.getLocalClassName() ,  String.valueOf(Thread.currentThread().getStackTrace()[2].getLineNumber()) );
            URL url = new URL("https://api.androidhive.info/images/sample.jpg");
            HttpURLConnection connection = (HttpURLConnection) url.openConnection();
            connection.setDoInput(true);
            connection.connect();
            InputStream in = connection.getInputStream();
            myBitmap = BitmapFactory.decodeStream(in);

        }
        catch (MalformedURLException e) {
            Log.i( this.getLocalClassName() ,  String.valueOf(Thread.currentThread().getStackTrace()[2].getLineNumber()) );
            e.printStackTrace();
        } catch (IOException e) {
            Log.i( this.getLocalClassName() ,  String.valueOf(Thread.currentThread().getStackTrace()[2].getLineNumber()) );
            e.printStackTrace();
        }*/

        Notification builder = new NotificationCompat.Builder(getApplicationContext(), CHANNEL_ID)
                .setSmallIcon(R.drawable.notification_icon)
                .setContentTitle("My notification")
                .setContentText("Much longer text that cannot fit one line...")
                .setLargeIcon(getBitMap (R.drawable. new_image ))
               // .setStyle(new NotificationCompat.BigTextStyle()
               //         .bigText("Much longer text that cannot fit one line..."))
                .setStyle(new NotificationCompat.BigPictureStyle()
                        .bigPicture(getBitMap (R.drawable. new_image )).bigLargeIcon(null))

                      //  .bigPicture(BitmapFactory.decodeResource(getResources(), R.drawable.mipmap)))
               .setPriority(NotificationCompat.PRIORITY_HIGH)
               .setContentIntent(pendingIntent)
                .setBadgeIconType(NotificationCompat.BADGE_ICON_SMALL)
                .setAutoCancel(true)
                .setColor(Color.BLUE)
                .setDefaults(Notification.DEFAULT_SOUND).build();
        Log.i( this.getLocalClassName() ,  String.valueOf(Thread.currentThread().getStackTrace()[2].getLineNumber()) );


        notifyAndroid ( builder, this );

        displayToast( "Sending picture notification ..");
    }

    public void onGoMain4Activity( View view )
    {
        Log.i( this.getLocalClassName() ,  String.valueOf(Thread.currentThread().getStackTrace()[2].getLineNumber()) );
        Intent newIntent = new Intent( this, Main4Activity.class);
        startActivity(  newIntent );
    }

    public void onSendInBoxNotif ( View view )
    {
        Log.i( this.getLocalClassName() ,  String.valueOf(Thread.currentThread().getStackTrace()[2].getLineNumber()) );

        Notification notification = new NotificationCompat.Builder(this, CHANNEL_ID)
                .setSmallIcon(R.drawable.new_image)
                .setContentTitle("5 New mails from " + "Alien")
                .setContentText("subject")
                .setLargeIcon(getBitMap (R.drawable. new_image ))
                .setStyle(new NotificationCompat.InboxStyle()
                        .addLine("I am coming to earth to teach people of earth")
                        .addLine("How to make better world"))
                .build();

        notifyAndroid( notification, this );
        displayToast( "Sending InBoxStyle notification ..");
    }

    public void onSendMessagingStyle ( View view )
    {
        Log.i( this.getLocalClassName() ,  String.valueOf(Thread.currentThread().getStackTrace()[2].getLineNumber()) );
        NotificationCompat.MessagingStyle.Message message1 =
                new NotificationCompat.MessagingStyle.Message("Hey Monojit How Can I help you",
                        1586831136,
                        "Alien");
        NotificationCompat.MessagingStyle.Message message2 =
                new NotificationCompat.MessagingStyle.Message("Please let me help to get Liberated from this world",
                        1586831136, "");

        Notification notification = new NotificationCompat.Builder(getApplicationContext(), CHANNEL_ID)
                .setSmallIcon(R.drawable.new_image)
                .setStyle(new NotificationCompat.MessagingStyle("Monojit")
                        .addMessage(message1)
                        .addMessage(message2))
                .build();

        notifyAndroid( notification, this );
        displayToast( "Sending MessagingStyle notification ..");
    }

    public Bitmap getBitMap ( int ResourseId )
    {

        return  BitmapFactory.decodeResource(getResources(), ResourseId );

    }

    public void notifyAndroid ( Notification builder, Context context )

    {
        NotificationManagerCompat notificationManager = NotificationManagerCompat.from(context);

// notificationId is a unique int for each notification that you must define
        notificationManager.notify( getNotificationID(), builder );

    }

    public void onClose ( View view )
    {
        Log.i( this.getLocalClassName() ,  String.valueOf(Thread.currentThread().getStackTrace()[2].getLineNumber()) );
        finish();
    }

    public static int getNotificationID() {
        return c.incrementAndGet();
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
