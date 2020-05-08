package com.eq.actionbar;

import androidx.appcompat.app.ActionBar;
import androidx.appcompat.app.AppCompatActivity;
import androidx.appcompat.widget.ShareActionProvider;
import androidx.appcompat.widget.Toolbar;
import androidx.core.view.MenuItemCompat;

import android.Manifest;
import android.content.Context;
import android.content.Intent;
import android.content.res.Resources;
import android.graphics.Bitmap;
import android.graphics.BitmapFactory;
import android.graphics.drawable.BitmapDrawable;
import android.net.Uri;
import android.os.Build;
import android.os.Bundle;
import android.os.Environment;
import android.provider.MediaStore;
import android.util.Log;
import android.view.Gravity;
import android.view.Menu;
import android.view.MenuItem;
import android.view.View;
import android.widget.ImageView;
import android.widget.Toast;

import java.io.ByteArrayOutputStream;
import java.io.File;
import java.io.FileNotFoundException;
import java.io.FileOutputStream;
import java.io.IOException;
import java.io.UnsupportedEncodingException;
import java.net.URI;

import static android.content.Intent.FLAG_GRANT_READ_URI_PERMISSION;
import static android.content.Intent.FLAG_GRANT_WRITE_URI_PERMISSION;
import static androidx.core.content.FileProvider.getUriForFile;

import static androidx.core.content.FileProvider.getUriForFile;

public class MainActivity extends AppCompatActivity {

    Uri fileUri;
    File newFile;
    Intent myShareIntent;

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);
        Toolbar myToolbar = (Toolbar) findViewById(R.id.my_toolbar);
        setSupportActionBar(myToolbar);

        ActionBar myActionBar = getSupportActionBar();

        myActionBar.setDisplayHomeAsUpEnabled(true);

    }

    @Override
    public boolean onCreateOptionsMenu(Menu menu) {
        // Inflate the menu; this adds items to the action bar if it is present.
        super.onCreateOptionsMenu(menu);
        getMenuInflater().inflate(R.menu.actionbar_items, menu);

        // Define the listener
        MenuItem.OnActionExpandListener expandListener = new MenuItem.OnActionExpandListener() {
            @Override
            public boolean onMenuItemActionCollapse(MenuItem item) {
                // Do something when action item collapses
                displayToast ("search action collapsed menu");
                return true;  // Return true to collapse action view
            }

            @Override
            public boolean onMenuItemActionExpand(MenuItem item) {
                // Do something when expanded
                displayToast ("search action expand menu");
                return true;  // Return true to expand action view
            }
        };

        // Get the MenuItem for the action item
        MenuItem actionMenuItem = menu.findItem(R.id.app_bar_search);

        // Removed ShareAction Provider, instead using share icon to share the item

        actionMenuItem.setOnActionExpandListener( expandListener) ;
        return true;
    }


    @Override
    public boolean onOptionsItemSelected(MenuItem item) {


        switch (item.getItemId()) {
            case R.id.first_id:
                // User chose the "Settings" item, show the app settings UI...
                displayToast ("first action bar touched");
                return super.onOptionsItemSelected(item);

            case R.id.second_id:
                // User chose the "Favorite" action, mark the current item
                // as a favorite...
                displayToast ("second action bar touched");
                Intent intent = new Intent( this, Main2Activity.class);
                startActivity( intent);
                return super.onOptionsItemSelected(item);


            case R.id.app_bar_search:
                // User chose the "Favorite" action, mark the current item
                // as a favorite...
                displayToast ("Search action bar touched");

                return super.onOptionsItemSelected(item);

            case R.id.actionprovider_id:
                // User chose the "Favorite" action, mark the current item
                // as a favorite...
                ;
                displayToast ("Shared action provider action bar touched");

                // Calling startActivity to open other application using createChooser method
                startActivity(Intent.createChooser(myShareIntent, "Sending attachment"));

                return super.onOptionsItemSelected(item);

            default:
                // If we got here, the user's action was not recognized.
                // Invoke the superclass to handle it.
                displayToast ("Default action bar touched");
                return super.onOptionsItemSelected(item);

        }
    }

    public void onNext ( View view )
    {
        Log.i( this.getLocalClassName() ,  Thread.currentThread().getStackTrace()[2].getMethodName() );
        Intent intent = new Intent( this, Main2Activity.class);
        startActivity( intent);
    }

    public void displayToast ( String sData)
    {
        Toast t = Toast.makeText(getApplicationContext(),
                sData,
                Toast.LENGTH_SHORT);
        t.setGravity(Gravity.TOP | Gravity.CENTER_HORIZONTAL, 0, 180);
        t.show();

    }

    private void getBitmap() {

        // Converting image to Bitmap
        try {
            Bitmap largeIcon = BitmapFactory.decodeResource(getResources(), R.mipmap.dog_icon_foreground);
            ByteArrayOutputStream bytes = new ByteArrayOutputStream();
            largeIcon.compress(Bitmap.CompressFormat.JPEG, 40, bytes);


            // Creating file for the image
            File imagePath = new File(Environment.getExternalStorageDirectory(),// for external storage
                    // we can create file in internal storage also
                    "Download");// child = path mentioned in filespath.xml under res/xml
            Log.i("MainActivity: ", "imagePath: " + imagePath);

            newFile = new File(imagePath, "dog_icon_round.jpeg");
            Log.i("MainActivity: ", "newFile: " + newFile);

            // write the bytes in file
            FileOutputStream fo = new FileOutputStream(newFile);
            fo.write(bytes.toByteArray());

            // close the FileOutputfo.close();
        } catch (IOException e) {
            // TODO Auto-generated catch block
            e.printStackTrace();
        }

        // Creating Uri using File provider to share with intent
        try {
            fileUri = getUriForFile(MainActivity.this,
                    "com.eq.actionbar.fileprovider", newFile);
            Log.i("MainActivity: ", "fileUri: " + fileUri);

        } catch (IllegalArgumentException e) {
            Log.e("File Selector",
                    "The selected file can't be shared: " + newFile.toString());
        }

        // Creating Intent to pass the attachment
        myShareIntent = new Intent(Intent.ACTION_SEND);
        myShareIntent.setType("image/jpeg");
        myShareIntent.putExtra(Intent.EXTRA_EMAIL, new String[]{"xyz@in.abc.com"});
        myShareIntent.putExtra(Intent.EXTRA_SUBJECT, "Hello!");
        myShareIntent.putExtra(Intent.EXTRA_STREAM, fileUri);

    }

    @Override
    protected void onStart() {
        super.onStart();
        getBitmap();
    }
}
