## 1. Set up the devkitPro toolchain ##

In order to develop anything for the Nintendo DS, you need the devkitPro cross-compilation toolchain. To set it up, please follow the [installation instructions](http://wiki.devkitpro.org/index.php/Getting_Started/devkitARM) on the [devkitPro webpage](http://www.devkitpro.org/). Yes, you do need all those libraries, except for maxmod and libfilesystem.

Since some updates of the devkitPro libraries have API changes, here's a list of the latest library versions tested to work with NitroTracker:

  * devkitARM [r30](https://code.google.com/p/nitrotracker/source/detail?r=30)
  * libnds 1.4.3
  * dswifi 0.3.12
  * libfat 1.0.7

## 2. Get required libraries ##

But first a word on source code management systems: I don't really like SVN, but I love git! For NitroTracker, I recommend using git-svn. There's instructions on how to get a Google Code SVN project as a git-svn project [here](http://google-opensource.blogspot.com/2008/05/develop-with-git-on-google-code-project.html).

I recommend putting all your DS development stuff (except for devkitPro) in a directory under your home directory, e.g.

**/home/miyamoto/code/dsdev/**.

### 2.1 libntxm ###

[libntxm](http://code.google.com/p/libntxm) is the heart of NitroTracker: The XM player. So, in order to work on NitroTracker, you also need the SVN version of libntxm. Check out the source from the SVN:

http://code.google.com/p/libntxm/source/checkout

### 2.2 libdsmi ###

[DSMI](http://dsmi.tobw.net) enables NitroTracker to send and receive MIDI via Wifi, enabling it to control synthesizer software on the PC, or letting some sequencer software on the PC trigger samples in NitroTracker. For libdsmi, it suffices if you download the latest release version from here:

http://code.google.com/p/dsmi/downloads/list

If you like the bleeding edge better, you can alternatively get the SVN version from:

http://code.google.com/p/dsmi/source/checkout

## 3. Getting the NitroTracker source and setting it up ##

As you might have guessed, you can get NitroTracker's source from here:

http://code.google.com/p/nitrotracker/source/checkout

Really, do use git-svn! Instructions are [here](http://google-opensource.blogspot.com/2008/05/develop-with-git-on-google-code-project.html).

You should now have three directories:

  * **/home/miyamoto/code/dsdev/dsmi**
  * **/home/miyamoto/code/dsdev/libntxm**
  * **/home/miyamoto/code/dsdev/nitrotracker**

The Makefiles of NitroTracker are configured such that when you compile NitroTracker, libdsmi and libntxm are compiled as well. So, a single **make** call will compile everything at once, and if you make updates to any of libraries, this single **make** call will compile the updated libraries and re-link NitroTracker.

To set up the build system, all you need to do is to specify the locations of libntxm and libdsmi in environment variables. The easiest way to do this is to declare these variables in your .bashrc:

```
export LIBDSMI=/home/miyamoto/code/dsdev/dsmi/ds/libdsmi/
export LIBNTXM=/home/miyamoto/code/dsdev/libntxm/libntxm/
```

Beware that **libdsmi** is in the subdirectory **ds/libdsmi/** of the **dsmi** project and **libntxm** is in the **libntxm** subfolder of the **libntxm** project.

Now, a simple

```
cd nitrotracker
make
```

should compile everything and leave you with a nice **nitrotracker.nds**

## 4. DLDI (optional) ##

Testing NitroTracker on your DS requires patching with a [DLDI](http://chishm.drunkencoders.com/DLDI/) patch. This patcher adds support for your specific flashcard to an nds binary that uses libfat. Chances are your flashcart supports DLDI patching out of the box and you don't have to do anything to be able to access the file system. If this isn't the case, get a DLDI patcher for your OS and a patch for your flashcard from [here](http://dldi.drunkencoders.com). You can then add a custom Makefile target (see below) to do all the patching for you.

## 5. Emulators (optional) ##

DS Emulators suck! Really, just don't trust them. Things that work on the hardware will not work on emulators. Things that work on emulators will not work on the hardware. When you test your DS software using only emulators, you're doomed!

That said, if you're cautious, emulators can be useful for quick tests. But remember to repeat your tests on hardware once in a while. Emulators I can recommend are:

  * [no$gba](http://nocash.emubase.de/gba.htm) (Windows, but works great in Wine)
  * [DeSmuME](http://desmume.org/) (Windows, Linux, OSX)

But as I said, better not use them.

## 6. Defining custom Makefile targets ##

You can create custom targets for DLDI patching, copying to flashcards, or running in an emulator. In order to do this, please do **not** edit the main Makefile (because then settings that only apply to you would be committed to the repository). Instead, create a file called **mytargets.mk** in the NitroTracker directory. If it exists, this file is included in the main Makefile, so targets you add there are available when building. Here's an example target for DLDI patching and copying to a GBAMP:

```
cp: $(TARGET).nds
    dlditool $(DLDIPATH)/mpcf.dldi $(TARGET).nds
    cp $(TARGET).nds /media/GBAMP/$(TARGET).nds
    pumount /media/GBAMP
```

As you can see this target also unmounts the CF card, so I can just do "make cp", pull out the flashcard and put it into my DS.