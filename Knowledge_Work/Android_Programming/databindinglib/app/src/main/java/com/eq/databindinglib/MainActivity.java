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
    ViewBindingTwoWay viewbindingtwoway;

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);

      //  MyLayoutBinding binding = MyLayoutBinding
        myMap = new HashMap<>(2);

        myBinding = ActivityMainBinding.inflate(getLayoutInflater());
        setContentView(myBinding.getRoot());
        myMap.put(0, "My Testing of Data Binding");
        viewbindingtwoway = new ViewBindingTwoWay( "the two day binding");
        myBinding.setTwowaydatabinding( viewbindingtwoway);
        myBinding.setDesire1( myMap);
    }

    public void onButtonPressed (View view)
    {


        myMap.put(0, "I am desir" + index );
        if ( index < 100 ) {
           // myBinding.setDesire(sName + " >> " + index);
            index = index + 1;
            viewbindingtwoway.data_s.set("two way " + ( index ));


        }
        else {
            index = 0;
            viewbindingtwoway.data_s.set("two way set nothing");

        }
        myBinding.setDesire1(myMap);
    }


    // the view which will be having both will be impacted

    @BindingAdapter( {"android:onClick", "android:text" , "specific"} )
    public static void setOnClick(TextView view, View.OnClickListener listener, String text, String spec) {
        view.setOnClickListener(listener);
        //view.setText(text);

            view.setText(text);
            if ( text == spec )
            {
                Log.i ("Binding", "two matched");
            }
            Log.i ("Binding", "else onClickListener and clickable -> "+ view.getText() + view.getLineCount() );





    }
    public  void onTextViewPressed ( View view )
    {
        Log.i("Binding", "onTextViewPressed Main");
    }



}
