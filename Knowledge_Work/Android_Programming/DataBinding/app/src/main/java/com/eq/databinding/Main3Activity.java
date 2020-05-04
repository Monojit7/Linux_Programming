package com.eq.databinding;

import androidx.appcompat.app.AppCompatActivity;
import androidx.databinding.DataBindingUtil;

import android.os.Bundle;
import android.view.View;

import com.eq.databinding.databinding.ActivityMain3Binding;

public class Main3Activity extends AppCompatActivity {

    public MyObservable observableImpl;
    public static Integer index = 0;

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        ActivityMain3Binding ActivityBinding = DataBindingUtil.setContentView(this, R.layout.activity_main3 );
        observableImpl = new MyObservable( "tanaya", "chhatna", "12", "24");
        ActivityBinding.setUser( observableImpl);

    }

    public  void ButtonOnClick ( View view )
    {
        index = index + 1;
        observableImpl.user1.put( 1 , index.toString());
    }
}
