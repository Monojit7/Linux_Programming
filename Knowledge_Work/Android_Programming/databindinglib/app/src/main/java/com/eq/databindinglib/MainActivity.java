package com.eq.databindinglib;

import androidx.appcompat.app.AppCompatActivity;
import androidx.databinding.BindingAdapter;

import android.os.Bundle;
import android.util.Log;
import android.view.Gravity;
import android.view.View;
import android.widget.TextView;
import android.widget.Toast;

import com.eq.databindinglib.databinding.ActivityMainBinding;

import java.util.HashMap;
import java.util.Map;

public class MainActivity extends AppCompatActivity {

    private ActivityMainBinding myBinding;
    private String sName = "My Testing of Data Binding";
    private Map<Integer, String> myMap;
    private int index = 0;
    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);

      //  MyLayoutBinding binding = MyLayoutBinding
        myMap = new HashMap<>(2);

        myBinding = ActivityMainBinding.inflate(getLayoutInflater());
        setContentView(myBinding.getRoot());
        myMap.put(0, "My Testing of Data Binding");
        myBinding.setDesire1( myMap);
    }

    public void onButtonPressed (View view)
    {
        index = index + 1;

        myMap.put(0, "I am desir" + index );
        if ( index <= 100 ) {
           // myBinding.setDesire(sName + " >> " + index);
            myBinding.setDesire1(myMap);
        }
        else {
            index = 0;
           // myBinding.setDesire(sName + " >> " + index);
            myBinding.setDesire1(myMap);
        }

    }

    @BindingAdapter("xyz")
    public static void setXyz(TextView view, String eq)
    {
        Log.i( "MainActivity", "" + eq  );
    }


}
