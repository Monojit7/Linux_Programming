package com.eq.viewmodeltoastdemo;

import android.app.Application;
import android.os.SystemClock;
import android.util.Log;
import android.view.Gravity;
import android.widget.TextView;
import android.widget.Toast;

import androidx.annotation.Nullable;
import androidx.lifecycle.AndroidViewModel;
import androidx.lifecycle.LiveData;
import androidx.lifecycle.MutableLiveData;
import androidx.lifecycle.Observer;
import androidx.lifecycle.ViewModel;

import java.util.Timer;
import java.util.TimerTask;

/**
 * A ViewModel used for the {@link ChronoActivity3}.
 */
public class LiveDataTimerViewModel extends AndroidViewModel implements Observer {

    private static final int ONE_SECOND = 1000;

    MutableLiveData<Long> mElapsedTime = new MutableLiveData<>();

    private long mInitialTime;
    private final Timer timer;

    public LiveDataTimerViewModel(Application application ) {
        super(application);
        mInitialTime = SystemClock.elapsedRealtime();
        timer = new Timer();

        // Update the elapsed time every second.
        timer.scheduleAtFixedRate(new TimerTask() {
            @Override
            public void run() {
                final long newValue = (SystemClock.elapsedRealtime() - mInitialTime) / 1000;
                // setValue() cannot be called from a background thread so post to main thread.
                mElapsedTime.postValue(newValue);
            }
        }, ONE_SECOND, ONE_SECOND);

        subscribe();

    }

    public LiveData<Long> getElapsedTime() {
        return mElapsedTime;
    }

    private void subscribe() {


        getElapsedTime().observeForever( this );
    }

    @Override
    protected void onCleared() {
        super.onCleared();
        timer.cancel();
        getElapsedTime().removeObserver(this);
    }

    @Override
    public void onChanged(Object aLong) {



        Log.d("ChronoActivity3", "Updating timer" );
        Toast t = Toast.makeText(getApplication().getApplicationContext(),
                "displaying  time" + aLong.toString()  ,
                Toast.LENGTH_SHORT);
        t.setGravity(Gravity.TOP | Gravity.CENTER_HORIZONTAL, 0, 180);
        t.show();
    }
}
