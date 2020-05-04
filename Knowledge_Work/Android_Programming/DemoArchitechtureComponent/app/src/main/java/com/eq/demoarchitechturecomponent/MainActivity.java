package com.eq.demoarchitechturecomponent;

import androidx.appcompat.app.AppCompatActivity;

import android.os.Bundle;
import android.view.View;

import com.eq.demoarchitechturecomponent.databinding.ActivityMainBinding;

public class MainActivity extends AppCompatActivity {

    private ActivityMainBinding myBinding ;
    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        myBinding = ActivityMainBinding.inflate( getLayoutInflater() );
        myBinding.myButton.setText("Love you Android");
        myBinding.myTextView.setText( "The reason for Love is you gave the opportunity to code!! Thats enough");
        setContentView(myBinding.getRoot());
    }
}
