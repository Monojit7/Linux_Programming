package com.eq.databinding;

import androidx.appcompat.app.AppCompatActivity;
import androidx.databinding.DataBindingUtil;

import android.content.Intent;
import android.os.Bundle;
import android.view.View;

import com.eq.databinding.databinding.ActivityMain2BindingImpl;

public class Main2Activity extends AppCompatActivity {

    private  ActivityMain2BindingImpl ActivityMain2BindingImpl_;

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        ActivityMain2BindingImpl_ = DataBindingUtil.setContentView(this, R.layout.activity_main2);
        MyUser user = new MyUser( "Monojit", null);
        ActivityMain2BindingImpl_.setUser(user);
    }

    public void buttonclicked(View view) {
        Intent intent = new Intent( this, Main3Activity.class);
        startActivity( intent );
    }
}


