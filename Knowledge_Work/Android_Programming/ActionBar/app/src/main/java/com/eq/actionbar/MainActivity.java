package com.eq.actionbar;

import androidx.appcompat.app.ActionBar;
import androidx.appcompat.app.AppCompatActivity;
import androidx.appcompat.widget.ShareActionProvider;
import androidx.appcompat.widget.Toolbar;
import androidx.core.view.MenuItemCompat;

import android.content.Context;
import android.content.Intent;
import android.content.res.Resources;
import android.graphics.Bitmap;
import android.graphics.BitmapFactory;
import android.graphics.drawable.BitmapDrawable;
import android.net.Uri;
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

public class MainActivity extends AppCompatActivity {

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
        Log.i( this.getLocalClassName() , String.valueOf(Thread.currentThread().getStackTrace()[2].getLineNumber()));

        MenuItem shareItem = menu.findItem(R.id.actionprovider_id);
        ShareActionProvider myShareActionProvider =
                (ShareActionProvider) MenuItemCompat.getActionProvider(shareItem);
       Bitmap bm = BitmapFactory.decodeResource(this.getResources(), R.mipmap.dog_icon_foreground);

      if ( bm == null )
          Log.i( this.getLocalClassName() , String.valueOf(Thread.currentThread().getStackTrace()[2].getLineNumber()));
      else
          Log.i( this.getLocalClassName() , String.valueOf(Thread.currentThread().getStackTrace()[2].getLineNumber()));



        Log.i( this.getLocalClassName() , String.valueOf(Thread.currentThread().getStackTrace()[2].getLineNumber()));

        ByteArrayOutputStream bytes = new ByteArrayOutputStream();
        Log.i( this.getLocalClassName() , String.valueOf(Thread.currentThread().getStackTrace()[2].getLineNumber()));
        bm.compress(Bitmap.CompressFormat.PNG, 100, bytes);
        Log.i( this.getLocalClassName() , String.valueOf(Thread.currentThread().getStackTrace()[2].getLineNumber()));
      //  String path = MediaStore.Images.Media.insertImage(this.getContentResolver(), bm, "Title", null);
        Log.i( this.getLocalClassName() , String.valueOf(Thread.currentThread().getStackTrace()[2].getLineNumber()));

      //  if ( path == null )
            Log.i( this.getLocalClassName() , String.valueOf(Thread.currentThread().getStackTrace()[2].getLineNumber()));


        Intent myShareIntent = new Intent(Intent.ACTION_SEND);
        myShareIntent.setType("png/image");
        String path = "";
        try {
            path = bytes.toString("UTF-8");
        } catch (UnsupportedEncodingException e) {
            e.printStackTrace();
        }

        myShareIntent.putExtra(Intent.EXTRA_STREAM, Uri.parse(path));



        //Log.i( this.getLocalClassName() , path );

        // Assign the listener to that action item
        myShareActionProvider.setShareIntent(myShareIntent);
        Log.i( this.getLocalClassName() , String.valueOf(Thread.currentThread().getStackTrace()[2].getLineNumber()));
        actionMenuItem.setOnActionExpandListener( expandListener) ;
        Log.i( this.getLocalClassName() , String.valueOf(Thread.currentThread().getStackTrace()[2].getLineNumber()));
       // ImageView imgView=(ImageView)findViewById(R.id.psdimage1);
        //Uri imgUri=Uri.parse("android.resource://my.package.name/"+R.drawable.image);
       // imgView.setImageURI(Uri.parse(path));
        Log.i( this.getLocalClassName() , String.valueOf(Thread.currentThread().getStackTrace()[2].getLineNumber()));
        return true;
    }


  /*  private Uri getImageUri(Context context, Bitmap inImage) {
        ByteArrayOutputStream bytes = new ByteArrayOutputStream();
        inImage.compress(Bitmap.CompressFormat.JPEG, 100, bytes);
        //String path = MediaStore.Images.Media.insertImage(context.getContentResolver(), inImage, "Title", null);
       // return Uri.parse(path);
    }*/


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
}
