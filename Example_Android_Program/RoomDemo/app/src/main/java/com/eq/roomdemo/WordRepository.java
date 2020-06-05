package com.eq.roomdemo;

import android.app.Application;

import androidx.lifecycle.LiveData;

import java.util.List;

public class WordRepository {
    private WordDao mWordDao;
    private LiveData<List<Word>> mAllWords;
    WordRepository (Application application )
    {
        WordRoomDatabase db = WordRoomDatabase.getDataBase(application);
        mWordDao = db.wordDao();
        mAllWords = mWordDao.getAllAlphabetizedWords();
    }

    LiveData<List<Word>> getmAllWords () { return  mAllWords; }
    void insert ( Word word )
    {
        WordRoomDatabase.dataBaseExecutor.execute(()->{
            mWordDao.insert( word );

                }

        );
        }
    }

