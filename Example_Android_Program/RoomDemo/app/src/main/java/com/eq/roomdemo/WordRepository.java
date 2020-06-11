package com.eq.roomdemo;

import android.app.Application;
import android.util.Log;

import androidx.lifecycle.LiveData;

import java.util.List;

public class WordRepository {
    private WordDao mWordDao;
    private LiveData<List<Word>> mAllWords;
    WordRepository (Application application )
    {
        Log.i("Roomflow",  String.valueOf(this.getClass())+ " constructor" );

        //getting the single instance of RoomDatabase
        WordRoomDatabase db = WordRoomDatabase.getDataBase(application);

        //this data access object is a abstract class constructor
        mWordDao = db.wordDao();

        //here the down query "SELECT * from word_table ORDER By word ASC" runs
        mAllWords = mWordDao.getAllAlphabetizedWords();
    }

    LiveData<List<Word>> getmAllWords () {
        Log.i("Roomflow",  String.valueOf(this.getClass())+ " getmAllWords" );
        return  mAllWords;
    }
    void insert ( Word word )
    {
        Log.i("Roomflow",  String.valueOf(this.getClass())+ " insert" );
        WordRoomDatabase.dataBaseExecutor.execute(()->{
            mWordDao.insert( word );
            Log.i("Roomflow",  String.valueOf(this.getClass())+ " dataBaseExecutor.execute" );

                }

        );
        }
    }

