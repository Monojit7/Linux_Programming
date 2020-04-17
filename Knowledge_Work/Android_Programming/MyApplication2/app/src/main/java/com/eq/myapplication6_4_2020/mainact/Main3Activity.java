package com.eq.myapplication6_4_2020.mainact;

import android.content.ActivityNotFoundException;
import android.content.Intent;
import android.content.pm.PackageManager;
import android.content.pm.ResolveInfo;
import android.net.Uri;
import android.os.Bundle;
import android.support.design.widget.FloatingActionButton;
import android.support.design.widget.Snackbar;
import android.support.v7.app.AppCompatActivity;
import android.support.v7.widget.Toolbar;
import android.util.Log;
import android.view.View;
import android.widget.EditText;
import android.widget.Toast;

import com.eq.myapplication6_4_2020.R;

import java.net.URI;
import java.util.List;

public class Main3Activity extends AppCompatActivity {


    EditText editText;
    EditText emailText;
    EditText recipient;
    EditText subjectText;

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main3);
        editText  = findViewById(R.id.textURL);
        emailText = findViewById ( R.id.EmailText );
        recipient = findViewById ( R.id.RecipientText );
        subjectText = findViewById ( R.id.subjectText );

        Log.i( this.getLocalClassName() ,     Thread.currentThread().getStackTrace()[2].getMethodName());
    }

    public void onClose ( View view )
    {
        Log.i( this.getLocalClassName() ,     Thread.currentThread().getStackTrace()[2].getMethodName());
       finish();
    }

    public void onURL ( View view )
    {
        Log.i( this.getLocalClassName() ,     Thread.currentThread().getStackTrace()[2].getMethodName());

        String URL = editText.getText().toString();
        Intent intent = new Intent( Intent.ACTION_VIEW , Uri.parse( URL ));
        startActivity( intent );
    }

    public void onNextPage ( View view )
    {
        Log.i( this.getLocalClassName() ,     Thread.currentThread().getStackTrace()[2].getMethodName());
        Intent intent = new Intent( this , Main2Activity.class );
        startActivity( intent );
    }

    public void EmailSend ( View view )
    {

        Log.i( this.getLocalClassName() ,     Thread.currentThread().getStackTrace()[2].getMethodName());
        String EmailContent = emailText.getText().toString();
        String RecipientName = recipient.getText().toString();
        String subject = subjectText.getText().toString();
        String chooser_title = getResources().getString(R.string.chooser_title );
        Log.i( this.getLocalClassName() ,  String.valueOf(Thread.currentThread().getStackTrace()[2].getLineNumber()) + " Recipient " + RecipientName  );
        //Intent sendIntent = new Intent();

        Intent sendIntent = new Intent(Intent.ACTION_SEND, Uri.fromParts(
                "mailto",RecipientName, null));

        sendIntent.putExtra(Intent.EXTRA_TEXT, EmailContent);
        sendIntent.putExtra(Intent.EXTRA_EMAIL, RecipientName);
        sendIntent.putExtra(Intent.EXTRA_SUBJECT , subject );
        sendIntent.setType("message/rfc822");

        sendIntent.setFlags( Intent.FLAG_ACTIVITY_NEW_TASK);


        Intent chooser = Intent.createChooser(sendIntent , chooser_title );
// Verify that the intent will resolve to an activity
        if (sendIntent.resolveActivity(getPackageManager()) != null) {
            startActivity(chooser);
        }


    }

    @Override
    protected void onRestart() {
        super.onRestart();
        Log.i( this.getLocalClassName() ,     Thread.currentThread().getStackTrace()[2].getMethodName());
    }

    @Override
    protected void onDestroy() {
        super.onDestroy();
        Log.i( this.getLocalClassName() ,     Thread.currentThread().getStackTrace()[2].getMethodName());
    }

    @Override
    protected void onStart() {
        super.onStart();
        Log.i( this.getLocalClassName() ,     Thread.currentThread().getStackTrace()[2].getMethodName());
    }

    @Override
    protected void onResume() {
        super.onResume();
        Log.i( this.getLocalClassName() ,     Thread.currentThread().getStackTrace()[2].getMethodName());
    }

    @Override
    protected void onPause() {
        super.onPause();
        Log.i( this.getLocalClassName() ,     Thread.currentThread().getStackTrace()[2].getMethodName());
    }

    @Override
    protected void onNewIntent(Intent intent) {
        super.onNewIntent(intent);
        Log.i( this.getLocalClassName() ,     Thread.currentThread().getStackTrace()[2].getMethodName());
    }

}
