package com.eq.roomdemo;

import android.content.Context;
import android.util.Log;

import androidx.annotation.NonNull;
import androidx.room.Database;
import androidx.room.Room;
import androidx.room.RoomDatabase;
import androidx.sqlite.db.SupportSQLiteDatabase;

import java.util.concurrent.Executor;
import java.util.concurrent.ExecutorService;
import java.util.concurrent.Executors;

@Database(entities = {Word.class}, version = 1, exportSchema = false)
public abstract  class WordRoomDatabase extends RoomDatabase {
    public abstract WordDao wordDao();
    private static volatile WordRoomDatabase INSTANCE;
    private static final int NUMBER_OF_THREADS = 4;
    static final ExecutorService dataBaseExecutor =
            Executors.newFixedThreadPool( NUMBER_OF_THREADS );
    static WordRoomDatabase getDataBase ( Context context )
    {
       if (INSTANCE == null )
       {
          synchronized (WordRoomDatabase.class)
          {
              if (INSTANCE == null)
              {
                  Log.i("Roomflow",  WordRoomDatabase.class.getName() + " WordRoomDatabase" );
                  INSTANCE = Room.databaseBuilder(context.getApplicationContext(), WordRoomDatabase.class, "word_database").addCallback(sRoomDatabaseCallback).build();
              }
          }
       }
       return INSTANCE;
    }

    private static RoomDatabase.Callback sRoomDatabaseCallback = new RoomDatabase.Callback()
    {
        @Override
        public void onOpen(@NonNull SupportSQLiteDatabase db) {
            super.onOpen(db);
            Log.i("Roomflow",  String.valueOf(this.getClass())+ " onOpen" );
            dataBaseExecutor.execute(()->
            {
                Log.i("Roomflow",  String.valueOf(getClass())+ " onOpen dataBaseExecutor.execute" );
                WordDao dao = INSTANCE.wordDao();
               dao.deleteAll();
                Word word = new Word("Monojit");
                dao.insert(word);
                word = new Word("Chatterjee");
                dao.insert(word);
            }

            );
        }
    };
}
