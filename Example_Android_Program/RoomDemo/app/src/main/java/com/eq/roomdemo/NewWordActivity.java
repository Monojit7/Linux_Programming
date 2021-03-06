package com.eq.roomdemo;

import androidx.appcompat.app.AppCompatActivity;

import android.content.Intent;
import android.os.Bundle;
import android.text.TextUtils;
import android.util.Log;
import android.view.View;
import android.widget.Button;
import android.widget.EditText;

public class NewWordActivity extends AppCompatActivity {
    public static final String EXTRA_REPLY = "com.example.android.wordlistsql.REPLY";
    private EditText mEditText;

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_new_word);
        mEditText = findViewById(R.id.edit_word);
        final Button button = findViewById(R.id.button_save);
        Log.i("Roomflow",  String.valueOf(this.getClass())+ " onCreate" );
        button.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View view) {
                Intent replyIntent = new Intent();
                Log.i("Roomflow",  String.valueOf(this.getClass())+ " onCreate" );

                if (TextUtils.isEmpty(mEditText.getText()))
                {
                    setResult(RESULT_CANCELED, replyIntent);
                }
                else {
                    String word = mEditText.getText().toString();
                    replyIntent.putExtra(EXTRA_REPLY, word);
                    setResult( RESULT_OK, replyIntent);

                }

                finish();
            }
        });
    }
}