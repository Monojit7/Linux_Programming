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

        /*if (Build.VERSION.SDK_INT >= Build.VERSION_CODES.M) {
            String[] sPerms = {Manifest.permission.READ_EXTERNAL_STORAGE, Manifest.permission.WRITE_EXTERNAL_STORAGE, Manifest.permission.GET_ACCOUNTS_PRIVILEGED, Manifest.permission.GET_ACCOUNTS_PRIVILEGED};
            requestPermissions(sPerms, 23);
        }*/
        // Get the MenuItem for the action item
        MenuItem actionMenuItem = menu.findItem(R.id.app_bar_search);
        Log.i( this.getLocalClassName() , String.valueOf(Thread.currentThread().getStackTrace()[2].getLineNumber()));

        MenuItem shareItem = menu.findItem(R.id.actionprovider_id);
        ShareActionProvider myShareActionProvider =
                (ShareActionProvider) MenuItemCompat.getActionProvider(shareItem);
       //Bitmap bm = BitmapFactory.decodeResource(this.getResources(), R.mipmap.dog_icon_foreground);


        Uri contentUri = null;



      //  try{

            Bitmap largeIcon = BitmapFactory.decodeResource(getResources(), R.mipmap.brent_goose_icon_new_foreground );



            ByteArrayOutputStream bytes = new ByteArrayOutputStream();
            largeIcon.compress(Bitmap.CompressFormat.PNG, 100, bytes);

            // you can create a new file name "test.jpg" in sdcard folder.
          //  File f = new File(Environment.getExternalStorageDirectory() + File.separator + "test.png");



            File imagePath = new File(Environment.getExternalStorageDirectory(), "images");
            File newFile = new File(imagePath, "default_image.png");
            contentUri = getUriForFile(getApplicationContext(), "com.eq.actionbar.testprovider", newFile);

            grantUriPermission( getPackageName(), contentUri, FLAG_GRANT_READ_URI_PERMISSION  | FLAG_GRANT_WRITE_URI_PERMISSION);

            Log.i( this.getLocalClassName() , String.valueOf(Thread.currentThread().getStackTrace()[2].getLineNumber()) + "uri =" + contentUri + "  " + Environment.getExternalStorageDirectory().toString());

            //newFile.createNewFile();

            // write the bytes in file
           // FileOutputStream fo = new FileOutputStream(newFile);
           // fo.write(bytes.toByteArray());

            // remember close de FileOutput
           // fo.close();
     //   }
     //   catch (IOException e) {
            // TODO Auto-generated catch block
       //     e.printStackTrace();
       //}

        Intent shareIntent = new Intent(Intent.ACTION_SEND);
        shareIntent.setType("image/*");

        shareIntent.putExtra(Intent.EXTRA_SUBJECT, "Hi"); //set your subject
        shareIntent.putExtra(Intent.EXTRA_TEXT, "How are you"); //set your message

        Log.i( getLocalClassName(), contentUri.toString() );

        //shareIntent.putExtra(Intent.EXTRA_STREAM, Uri.parse("android.resource://" + C.PROJECT_PATH + "/drawable/" + R.mipmap.dog_icon_foreground);

        shareIntent.putExtra(Intent.EXTRA_STREAM, contentUri);



        shareIntent.setFlags( Intent.FLAG_GRANT_READ_URI_PERMISSION);



        myShareActionProvider.setShareIntent(shareIntent);

       // ImageView image = findViewById( R.id.psdimage1);
       // image.setImageURI( contentUri);

        //startActivity(Intent.createChooser(shareIntent, "Share Image"));
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
