package com.eq.databinding;

import androidx.databinding.BaseObservable;
import androidx.databinding.Bindable;
import androidx.databinding.ObservableField;
import androidx.databinding.ObservableInt;


/**
 * Created on : Mar 16, 2019
 * Author     : AndroidWave
 */
public class MyObservable extends BaseObservable {
    private String name;
    private String address;
    private String followerCount;
    private String followingCount;
    public MyObservable(String name, String address, String followerCount, String followingCount) {
        this.name = name;
        this.address = address;
        this.followerCount = followerCount;
        this.followingCount = followingCount;
    }
    @Bindable
    public String getName() {
        return name;
    }
    public void setName(String name) {
        this.name = name;
        notifyPropertyChanged(BR.name);
    }
    @Bindable
    public String getAddress() {
        return address;
    }
    public void setAddress(String address) {
        this.address = address;
        notifyPropertyChanged(BR.address);
    }
    public String getFollowerCount() {
        return followerCount;
    }
    public void setFollowerCount(String followerCount) {
        this.followerCount = followerCount;
    }
    public String getFollowingCount() {
        return followingCount;
    }
    public void setFollowingCount(String followingCount) {
        this.followingCount = followingCount;
    }
}
