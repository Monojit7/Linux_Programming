package com.eq.roomdemo;

import android.app.Application;
import android.util.Log;

import androidx.annotation.NonNull;
import androidx.lifecycle.AndroidViewModel;
import androidx.lifecycle.LiveData;

import java.util.List;

public class WordViewModel extends AndroidViewModel {

    private WordRepository mWordRepository;
    private LiveData<List<Word>> mAllWords;

    public WordViewModel(@NonNull Application application) {
        super(application);
        mWordRepository = new WordRepository( application);
        mAllWords = mWordRepository.getmAllWords();
        Log.i("Roomflow",  String.valueOf(this.getClass())+" constructor" );
    }

    LiveData<List<Word>> getmAllWords ()
    {
        Log.i("Roomflow",  String.valueOf(this.getClass())+ " getmAllWords" );

        return this.mAllWords ;
    }

    public void insert ( Word word ) {
        mWordRepository.insert( word) ;
        Log.i("Roomflow",  String.valueOf(this.getClass())+ " insert" );
    }

}
