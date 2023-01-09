- dvAsl(Display Virtualization Android System Library) is a library which has APIs to control VirtIO pipeline w.r.t system resolution/rotation.
- dvAslTest is an OpenGL ES application developed for testing display virtualization on Android SRIOV stack. It will get/set the supported resolutions from QEMU EDID for the test cases.
- This application will isolate any opengl rendering issues for a specific resolution.

Build Steps:

	1. Copy the src code to '/vendor/intel/external' dir.
	2. To compile, give a 'mm' command in the dvAslTest directory. The library will get compiled on its own.

How to use : 

	- Launch the Android VM.
	- Push the binary via 'adb push <bin_name> /system/bin'.
	- From android console, navigate to '/system/bin'. (adb shell can also be used for a user-debug build)
	- Launch dvAslTest in any of the 2 ways : 
		1. ./dvAslTest
		2. ./dvAslTest <cmd-line-arg>
		
	- The dvAslTest will present you with 4 methods with the OpenGL ES APIs:
		1. Triangle Rotation Test
			a. It displays a triangle on the screen and rotates it for 90, 180, 270, 0 degrees of rotation.
		2. Color Pattern Test
			a. It displays color pattern frames (RGB) on the screen, each having a timestamp of 16.66ms for a total of 1500 frames.
		3. Desktop Rotation Test
			a. It displays the rotation of the desktop for 90, 180, 270, 0 degrees of rotation.
		4. Long Run Test
			a. It is a long run test, which is a combination of the above tests, in an endless time window, unless stopped by the user.
			b. For terminating the test, press 'Ctrl + C'.
		
	- There are 4 Command Line Arguments which can be passed, to launch any of the test directly: 
		1. trianglerotationtest
		2. colorpatterntest
		3. desktoprotationtest
		4. longruntest
	- Eg: ./dvAslTest trianglerotationtest

Note :

	1. Current Version - 1.0 This binary/library is compiled and tested for Android S.
	2. To run it in full-screen, just flash the VM without -full-screen in start_civ.sh and then use 'Ctrl+Alt+f' to make the VM full screen.
	3. To enable debug logs, add the "-DDVASL_DEBUG" flag in the "cflags" section of the Android.bp file in library (/dvAsl) and application (/dvAslTest).
