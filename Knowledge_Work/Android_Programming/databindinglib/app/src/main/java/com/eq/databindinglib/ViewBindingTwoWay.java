package com.eq.databindinglib;
import android.database.Observable;
import android.util.Log;

import androidx.databinding.BaseObservable;
import androidx.databinding.Bindable;
import androidx.databinding.ObservableField;

public class ViewBindingTwoWay extends BaseObservable {
    public String data;
    public ObservableField<String> data_s = new ObservableField<>();

    public ViewBindingTwoWay(String data )
    {
        this.data = data;
        data_s.set(this.data);

    }

    @Bindable
    public String getData()
    {
        Log.i("ViewDataBinding", "getData" + data);
        return data;
    }

    public void setData ( String data )
    {
        Log.i("ViewDataBinding", "setData" + data );
        this.data = data;
        notifyPropertyChanged(BR.data);
        data_s.set(this.data);
    }
}
