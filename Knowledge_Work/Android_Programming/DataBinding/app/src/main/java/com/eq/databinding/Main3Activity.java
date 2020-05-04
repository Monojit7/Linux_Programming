package com.eq.databinding;

import androidx.appcompat.app.AppCompatActivity;
import androidx.databinding.DataBindingUtil;

import android.os.Bundle;

import com.eq.databinding.databinding.ActivityMain3Binding;

public class Main3Activity extends AppCompatActivity {

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        ActivityMain3Binding ActivityBinding = DataBindingUtil.setContentView(this, R.layout.activity_main3 );
        MyObservable observableImpl = new MyObservable( "tanaya", "chhatna", "12", "24");
        ActivityBinding.setUser( observableImpl);
    }
}
