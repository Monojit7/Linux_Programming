#include <spot/harman_kpimarker.h>
#include <core.h>
#include <playengine/MediaPlaybackEngine.h>
#include <playengine/MediaPlayerStubImpl.hpp>
#include <playengine/MediaSettingsStubImpl.hpp>
#include <browseengine/MediaBrowserStubImpl.h>
#include <detectionengine/MediaManagerStubImpl.h>
#include<audiomanagerinterface/audiomanagerinterface.h>
#include<audiomanagerinterface/RoutingControlStubImpl.hpp>
#include<statemachine/sourceswitch.h>
#include<config/ConfigParser.h>
#include<config/MediaPresSettings.h>
#include<MediaPresCore.h>
#include <systemd/sd-daemon.h>
#ifndef PLATFORM_WIN32
#include <unistd.h>
#include <stdio.h>
#include <string.h>
#include <systemd/sd-daemon.h>
#include <execinfo.h>
#include <signal.h>
#endif


#define FILE_PATH_SIZE 513

#ifdef PLATFORM_WIN32
char configFile[FILE_PATH_SIZE] = "..\\..\\modules\\media\\src\\config\\mediapres.cfg";
#else
char configFile[FILE_PATH_SIZE] = "/etc/mediapres.cfg";
#endif

#ifdef PLATFORM_WIN32

BOOL WINAPI HandleTermination(_In_ DWORD dwCtrlType)
{
	CommonAPI::Runtime::get()->unregisterService("local", "com.harman.mmpres.MediaPlayer", "mmpres.playengine");
	CommonAPI::Runtime::get()->unregisterService("local", "com.harman.mmpres.MediaBrowser", "mmpres.browser");
	CommonAPI::Runtime::get()->unregisterService("local", "com.harman.mmpres.MediaManager", "mmpres.mediamanager");
	CommonAPI::Runtime::get()->unregisterService("local", "com.harman.mmpres.MediaSettings", "mmpres.mediasettings");
	CommonAPI::Runtime::get()->unregisterFactory("dbus");

	return FALSE;
}

#endif


static void full_write(int fd, const char *buf, size_t len)
{
        while (len > 0) {
                ssize_t ret = write(fd, buf, len);

                if ((ret == -1) && (errno != EINTR))
                        break;

                buf += (size_t) ret;
                len -= (size_t) ret;
        }
}

void print_backtrace(int sig )
{
        static const char start[] = "BACKTRACE ------------\n";
        static const char end[] = "----------------------\n";

        void *bt[1024];
        int bt_size;
        char **bt_syms;
        int i;
        int fd = open ("/tmp/mmpres_backtrace", O_RDWR | O_CREAT | O_SYNC,S_IRUSR | S_IWUSR);
        if(fd == -1){
        	LOG_INFO(LOG_CONTEXT_MEDIA, "<<< %s %d <symbols : Cannot open output file>", __FUNCTION__, __LINE__);
        	exit(1);
        }
        bt_size = backtrace(bt, 1024);
        bt_syms = backtrace_symbols(bt, bt_size);
        full_write(fd, start, strlen(start));
        for (i = 1; i < bt_size; i++) {
                size_t len = strlen(bt_syms[i]);
                LOG_INFO(LOG_CONTEXT_MEDIA, "<<< %s %d <symbols : %s>", __FUNCTION__, __LINE__, bt_syms[i] );
                full_write(fd , bt_syms[i], len);
                full_write(fd, "\n", 1);
        }
        full_write(fd, end, strlen(end));
    free(bt_syms);
    close(fd);
}

int main(int argc, char *argv[])
{
#ifdef PLATFORM_WIN32
	// Need to run termination handler on Windows. 
	// Failure to do this results in failed service de-registration. 
	// Add all the service cleanup in this. 
	SetConsoleCtrlHandler(HandleTermination, TRUE);
#endif

    sd_notify(0, "STATUS=MediaPresCtrl initiated");
	std::unordered_map<uint16_t, const char*> loggingContexts = {
		{ LOG_CONTEXT_MEDIA,"Media" }
		,{ LOG_CONTEXT_MEDIADETECTION, "Detection" }
		,{ LOG_CONTEXT_MEDIAPLAYBACK, "Playback" }
		,{ LOG_CONTEXT_MEDIABROWSE, "Browse" }
		,{ LOG_CONTEXT_MPRESPLUGIN, "MpresPlugin" }
		,{ LOG_CONTEXT_RCD, "RCD" }		
	};

	Log::initLogging("MPRS", "Media Pres Control", loggingContexts); \


    LOG_INFO(LOG_CONTEXT_MEDIA, "<<< Going to initialise core and subscribe for watch callbacks");
	PresCtrl::initPresControl(RequestList);
	CommonAPI::Runtime::setProperty("LogContext", "MPRS");
	CommonAPI::Runtime::setProperty("LibraryBase", "mmpres");

  #ifndef PLATFORM_WIN32
	int confpath;
	while ((confpath = getopt((argc), &argv[0], "c:")) != EOF)
	{
		switch (confpath)
		{
		case 'c':
			strncpy(configFile, optarg, FILE_PATH_SIZE-1);
			break;

		default:
			break;
		}
	}
  #endif

	/**
	*  Create default tasks / requests here
	*/
    KPIMARKER(306, "AV_MEDIAPRES-START");
	FacadeHelper<ConfigParser>()->setConfigFile(configFile);
	FacadeHelper<MediaPresSettings>();
/*	if (FacadeHelper<MediaPresSettings>()->getAMInteraction())
	{
		FacadeHelper<AudioManagerInterface>();
	}
	FacadeHelper<MediaDetectionEngine>();
	FacadeHelper<MediaPlaybackEngine>();
	FacadeHelper<MediaBrowseEngine>();
	FacadeHelper<SourceSwitch>();*/

	FacadeHelper<MediaDetectionEngine>();
	FacadeHelper<MediaPlaybackEngine>();
	FacadeHelper<MediaBrowseEngine>();

	if (FacadeHelper<MediaPresSettings>()->getAMInteraction())
	{
		FacadeHelper<AudioManagerInterface>();
	}
	FacadeHelper<SourceSwitch>();
	FacadeHelper<MediaPresCore>();
    FacadeHelper< RemoteCDPlaybackEngine > ();
	CommonAPIServiceAbstraction<v1::com::harman::mmpres::MediaManagerStubImpl> mmService("local", "mmpres.mediamanager");
	CommonAPIServiceAbstraction<v1::com::harman::mmpres::MediaPlayerStubImpl> playerService("local", "mmpres.playengine");
	CommonAPIServiceAbstraction<v3::com::harman::mmpres::MediaBrowserStubImpl> browserService("local", "mmpres.browser");
	CommonAPIServiceAbstraction<v1::com::harman::mmpres::MediaSettingsStubImpl> msService("local", "mmpres.mediasettings");
    signal ( SIGSEGV, print_backtrace );
    signal ( SIGABRT, print_backtrace );
	
	if (FacadeHelper<MediaPresSettings>()->getAMInteraction())
	{
		CommonAPIServiceAbstraction<v1::org::genivi::am::routinginterface::RoutingControlStubImpl> routingControlService("local", "mmpres.routingControl");
	}

    //sd_notify(0, "READY=1");
	PresCtrl::mainLoop(  );
	return 0;
}

