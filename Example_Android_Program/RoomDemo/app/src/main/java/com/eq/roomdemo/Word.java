package com.eq.roomdemo;

import android.util.Log;

import androidx.annotation.NonNull;
import androidx.room.ColumnInfo;
import androidx.room.Entity;
import androidx.room.PrimaryKey;

@Entity ( tableName = "word_table")
public class Word {

    @PrimaryKey
    @NonNull
    @ColumnInfo (name = "word")

    private String mWord;

    public Word (@NonNull String word ) {
        this.mWord = word ;
        Log.i("Roomflow",  String.valueOf(this.getClass())+ " constructor" + word ) ;
    }
    public String getWord () {
        Log.i("Roomflow",  String.valueOf(this.getClass())+ " getWord " + this.mWord);
        return this.mWord;
    }
}

