CMUS Lyrics Plugin
==================

This is a simple cmus plugin that can synchronously display the lrc lyrics on 
a separated window.

How to Use
----------

For the first time using this plugin, you will need to type in the full 
executing path of cmus-remote (use "which" if you are not sure where it is) 
and the path of your lyrics folder. You can change your settings any time you
want by clicking main menu -> Preferences.

Please put your lrc files in the folder you specified and name them exactly 
the same as the music file related to that except for the filename extension.
For example, if your music file is "Enya - Only Time.ape", you should name 
your lrc file "Enya - Only Time.lrc".

When both cmus and the plugin are running, the lyrics window will show up if
matched lrc file is found, and will hide if it is not found.

Precision problem: 
The minimum precision of the position information given by cmus-remote is one
second, while the min precision of lrc is one centisecond, this difference 
may make the timing of the lyrics slightly imprecise, but it is not a serious
problem since there are five lines being shown at the same time.

How It Works
------------

A separated thread periodically grab status information of cmus by calling 
"cmus-remote -Q". It uses the file path information given by cmus-remote to 
form the path of the lrc file. If the path is different from the last one 
being stored and a file exists at that path, the file will be loaded into the 
Lyrics object. The thread will pass current time position to the object and 
the lyrics at that time will be returned and printed on the window.

Classes Implemented
-------------------

Lyrics:
A class that loads the lrc code into a string vector that stores the lines of 
lyrics and an integer time array that stores the relations from time 
(centisec) to index of lyrics array. The initial size of the time array is 
10 min * 60 min/sec * 100 centisec/sec * 2 bytes/short = 117 kb; searching for 
lyrics at any time positions will be constant time operation.

CmusStatus:
A class that grabs status information from cmus-remote.

These two classes are written in C++ so they can be used in any other projects
or unix-like systems.

Licensing
---------

CMUS Lyrics Plugin is licensed under the GNU General Public License version 3.

Yuwei Huang
2014.3.5