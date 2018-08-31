How to download and use prescore
- Install git
- Create and add SSH key at https://stash1.harman.com/plugins/servlet/ssh/account/keys
	For details on creation and addition refer https://confluence.atlassian.com/bitbucketserver/creating-ssh-keys-776639788.html
- Now in a git-bash / terminal window type in the following
	git clone ssh://git@stash1.harman.com:7999/int_oakland/pg_core.git
- Above command will clone the git project into your workspace

Build prerequisites
- Prescontrollers can be built for
	Windows x86	: Requires Visual Studio 2015
	Orinoco target boards	: Requires VM with Orinoco sdk installed 

Build steps
	$cd modules/
	$mkdir build
	$cd prescore
if widows
	$cmake -G "Visual Studio 14 2015 Win64" ..
if linux
	$cmake –D CMAKE_SYSROOT=<path to sysroot>  ..
	$make
Output is available here as per the build env chosen
	$cd ../../deploy/

Run steps
	Make changes to the target dbus conf files <contact orinocopresctrl DL>
	Add/export following to environment of your IDE/shell
	DBUS_SYSTEM_BUS_ADDRESS=tcp:host=<ip address of target>,port=55556,family=ipv4
	DBUS_SESSION_BUS_ADDRESS=tcp:host=<ip address of target>,port=55556,family=ipv4


