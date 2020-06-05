package com.eq.roomdemo;

import androidx.appcompat.app.AppCompatActivity;
import androidx.lifecycle.Observer;
import androidx.lifecycle.ViewModelProvider;
import androidx.recyclerview.widget.LinearLayoutManager;
import androidx.recyclerview.widget.RecyclerView;

import android.content.Intent;
import android.os.Bundle;
import android.view.View;
import android.widget.Toast;

import com.google.android.material.floatingactionbutton.FloatingActionButton;

import java.util.List;

public class MainActivity extends AppCompatActivity {

    private WordViewModel mWordViewModel;
    public static final int NEW_WORD_ACTIVITY_REQUEST_CODE = 1;

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);
       mWordViewModel = new ViewModelProvider(this).get(WordViewModel.class);

        RecyclerView recyclerView = findViewById(R.id.recyclerview);
        final WordListAdapter wordListAdapter = new WordListAdapter(this);
        recyclerView.setAdapter(wordListAdapter);
        recyclerView.setLayoutManager(new LinearLayoutManager(this));
        Observer<List<Word>> wordObserved = new Observer<List<Word>>() {
            @Override
            public void onChanged(List<Word> words) {
                  wordListAdapter.setWords(words);
            }
        };
        mWordViewModel.getmAllWords().observe(this, wordObserved);
        FloatingActionButton fab = findViewById(R.id.fab);
        fab.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View view) {
                Intent intent = new Intent(MainActivity.this, NewWordActivity.class);
                startActivityForResult(intent, NEW_WORD_ACTIVITY_REQUEST_CODE);
            }
        });
    }

    public void onActivityResult (int requestCode , int resultCode, Intent data ) {

        super.onActivityResult(requestCode, resultCode, data);
        if ( requestCode == NEW_WORD_ACTIVITY_REQUEST_CODE && resultCode == RESULT_OK)
        {
            Word word = new Word(data.getStringExtra(NewWordActivity.EXTRA_REPLY));
            mWordViewModel.insert(word);
        }
        else
        {
            Toast.makeText(getApplicationContext(), R.string.empty_not_saved, Toast.LENGTH_SHORT).show();
        }
    }
}
