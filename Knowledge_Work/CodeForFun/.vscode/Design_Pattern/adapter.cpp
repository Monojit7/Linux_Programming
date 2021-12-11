/*

Adapter pattern provide bridge between two different incompatible interface.
This comes under structural design pattern and combines the capabilities 
of two independent interfaces

*/


#include <iostream>
#include <list>

using namespace std;


class MediaPlayer
{
    public:

    virtual void play () = 0;
};



class AdvanceMediaPlayer 
{
    public :

    virtual void play( string filename ) = 0;
   // virtual void playVLC () = 0;
};


class MP4Player : public AdvanceMediaPlayer
{
    public:

    MP4Player ( )
    {

    }

    void play ( string filename )
    {
        cout << "playing MP4 " << filename << endl;
    }

};

class VLClayer : public AdvanceMediaPlayer
{
    public:

    VLClayer ( )
    {

    }

    void play ( string filename )
    {
        cout << "playing VLC --> " << filename <<  endl;
    }

};

class MediaAdapter : public MediaPlayer
{
    public:

    AdvanceMediaPlayer* mAdvanceMediaPlayer = nullptr;
    string audioType = {};
    string filename = {};

    MediaAdapter  ( string audioType, string filename )
    {
        this->audioType = audioType;
        this->filename = filename;

        if ( audioType == "MP4")
        {
            mAdvanceMediaPlayer = new MP4Player;
        }
        else if ( audioType == "VLC")
        {
            mAdvanceMediaPlayer = new VLClayer;
        }
    }

    void play ()
    {
        if ( mAdvanceMediaPlayer )
        mAdvanceMediaPlayer->play( this->filename );
    }
};

class AudioPlayer : public MediaPlayer
{
    public:
    MediaAdapter* mMediaAdapter = nullptr;
    string audioType = {};
    string filename = {};
    AudioPlayer ( string audioType , string filename)
    {
        this->audioType = audioType;
        this->filename = filename;
        if ( audioType == "MP3")
        {
            cout << "I have the capability to play mp3 hence playing " <<  filename << endl;

        }
        else 
        {
            mMediaAdapter = new MediaAdapter ( audioType , filename );
            
        }
    }

    void play ()
    {
        if ( mMediaAdapter )
        mMediaAdapter->play();
        else
        cout << "playing MP3 " << this->filename << endl;
    }
};

int main ()
{
    AudioPlayer* mAudioPlayer = new AudioPlayer ( "MP3", "kaho na pyaar hain");
    mAudioPlayer->play();
    mAudioPlayer = new AudioPlayer ("MP4", " Bhige honth tere ");
    mAudioPlayer->play();
    mAudioPlayer = new AudioPlayer ("VLC", " Bengali XYZ ");
    mAudioPlayer->play();
    delete mAudioPlayer;
}





