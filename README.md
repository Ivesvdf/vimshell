# VimShell #
## What is it ##
An executable that replaces vimrun.exe in Vim for Windows installations and is able to run any shell (cygwin, puttycyg, ...) while standard vimrun.exe can only run cmd.exe

## Installation ##
  * Navigate to your vim installation (default: C:\program files\vim\vim72)
  * Find the file named "vimrun.exe" in your vim folder.
  * Rename this file to "vimrun.exe.bak"
  * Download the VimShell vimrun.exe [here ](http://vimshell.googlecode.com/files/vimrun.exe) and place it where the old vimrun.exe used to be
  * In that same folder, create a file called "shell.txt" and fill it as detailed in the next section.
  * Add the following lines to your .vimrc (sometimes this is optional and sometimes it isn't, based on what shell you're using so just do it ;-))

## Addition to your .vimrc ##
```
set shellquote=
set shellslash  
set shellxquote=
set shellpipe=2>&1\|tee
set shellredir=>%s\ 2>&1
```

## shell.txt ##
This file should consist of 3 lines, the first line containing the command that will be executed when parameters are given (i.e. when you type `:!echo bla` in Vim. The second line will contain the command that is executed when you wish to silently execute a command. The third line will contain the command that will be executed when no parameters are given (i.e. when you type `:!` in Vim).

An example shell.txt file (the one I use)
```
"C:\Program files\PuttyCyg\putty.exe" -cygterm "bash -i '#F;read;#'"
"C:\Program files\PuttyCyg\putty.exe" -cygterm "bash '#F#'"
"C:\Program files\PuttyCyg\putty.exe" -cygterm -
```
Where #F...something...# will be **the file your shell needs to read commands from**.

Vimshell now works with files instead of inline strings. This assures everything works 100% of the time. Instead of the old #QQQQ# you now enter #F# in your shell.txt to reference the file your shell should read from. If you wish to append (prepending isn't possible at the moment (and would be impossible with this syntax)) something at the end of this file, do so like this: #Fxxx# to append xxx. For example, something useful to append would be ;read; on the first line of your shell.txt, so your shell will pauze after running
