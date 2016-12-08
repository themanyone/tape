
# Things All People Enjoy (TAPE)

###Stick files together with tape. Compatible with current players!
<img style="float:right" src="https://encrypted-tbn2.gstatic.com/images?q=tbn:ANd9GcRGxXrtxYkj6fjku-t4RsWpM8n8UUbsAKN7qpKCWd1eon1KlwFV">Brought to you by [TheNerdShow.com](http://thenerdshow.com).

* Stick a bunch of movies or music together.
* Take them apart again.
* Write on the back of digital photos.
* Attach reference info to downloads.
* Remind people where that sh't came from.

## The Vision of Tape

* **Simple**. Stick those files together with tape.
* **Compatible** with existing players and viewers. Everything works with tape!
* **Extendable**. Literally, almost any files can be stuck together with tape.
* **Reversible**. Put a catalog at the end, so files may be extracted later.
* **Stable**. Tape is a command line tool. Optional graphical interface coming soon.
* **Fast**. No waiting for a large application to load.
* **Free**. Copy, share, even sell Tape, but publish a link to the [author's web site](http://thenerdshow.com).

## Mission statement: Putting it all together.

* Bundle a bunch of movies or music into one playable file.
* Easily take them apart again.
* Make videos longer without editing the original.
* Put an intro and outro onto videos without editing.
* Arrange pre-made media scenes like lego bricks.
* Attach reference info to downloads.
* Attach stories to those digital photos.
* Remind people where that sh't came from.
* Memories, man. Memories.

## How it works

Did you know `cat` can string media files together?

And that most media players will play them?

Tape is sort of like that.

## Tape is better than cat
Things on tape

* can be extracted later
* can be optionally compressed with lzma

For example, `tape` can combine `t.mp3`, `a.mp3`, and `pe.mp3` into `tape.mp3`.

`tape t.mp3 a.mp3 pe.mp3 tape.mp3`

With different types of media some players may stop after the first one.

In this case `tape.mp3` has the same file extension as `t.mp3` does, so the resulting archive will be playable (recommended).

## Tape Also Works On `.jpg`, `gif`, `.mp4`...

Most media may be taped together

* The first one serves as a preview
* Existing viewers only show the first image
* Newer viewers will be able to flip through
* New players will be able to read animations

## Create Playable "Tapes" 
```
tape [options] [media] [attachments] ... output_media

-c optionally compress attachments with LZMA
      
The first file remains playable, and is never compressed.
```
##Extraction
```
tape [options] [attachments] ... input_media:

-c optionally compress attachments
(media remains playable, and is never compressed).

-k list with human-readable file sizes
-l List media attachments
-x eXtract [attachments] from input_media
-o output to [directory]
-p extract [attachments] from input_media to pipe
-h Help
```
## More Examples

List files in bar.mp4.

```
tape -k bar.mp4

Contents of bar.mp4:
2.4M    bar.mp4
3M      foo.mp3
3.4M    baz.vpx
49K     cover_art.jpg
2K      LICENSE
1K      README
```

(The numbers represent file sizes.)
        
Play bar.mp4 with any media player such as ffplay.

`ffplay bar.mp4`
        
Use ffplay to play "hidden" foo.mp3 attached to bar.mp4.

`tape -p foo.mp3 bar.mp4 | ffplay`

## Get it now

You should start playing with tape immediately. Download now from https://github.com/themanyone/tape

## Build System Configuration

Everyone should have access to a [GNU Build System](https://en.wikipedia.org/wiki/GNU_Build_System) in order to avail themselves of the abundance of [free and open source (FOSS) software](http://www.freeopensourcesoftware.org/index.php?title=Main_Page) available for Windows, Linux, Android, iPhones, and many other types of computers and devices. FOSS software is distributed as [source code](https://en.wikipedia.org/wiki/Source_code) and must be compiled before use. Different [compilers and build systems](https://en.wikipedia.org/wiki/Compiler) exist for each type of system.

###Windows development

The best free development systems for Windows are [Cygwin](http://www.cygwin.com/), [MinGW](http://blog.florianwolters.de/educational/2013/11/21/Installing-MinGW/) or MSYS (available through MinGW's mingw-get) to compile free software. To build true windows programs and DLLs, use [MinGW](http://www.mingw.org/), or [MinGW via Cygwin](https://www.ntu.edu.sg/home/ehchua/programming/howto/Cygwin_HowTo.html). One may build Windows programs on Cygwin without using MinGW, but they will depend on `Cygwin.dll` to run.

*Installing Minimalist GNU for Windows (MinGW) on Windows*

* [unofficial howto](http://blog.florianwolters.de/educational/2013/11/21/Installing-MinGW/)

* [official documentation](http://www.mingw.org/wiki/howto_install_the_mingw_gcc_compiler_suite)

*Installing Cygwin *

* [unofficial howto](https://www.ntu.edu.sg/home/ehchua/programming/howto/Cygwin_HowTo.html)

* [official documentation](http://www.cygwin.com/)

Note: It is also possible to use [Microsoft Visual Studio(MSVC)](https://folti.blogs.balabit.com/2009/08/compiling-autoconfmake-projects-under-msvc-part-one/) to build FOSS software, but the task is not recommended for beginners.

###Linux development

Different brands of Linux each have their own way of installing software, including compilers and build systems.

*Ubuntu* [Setting up a build system on Ubuntu](https://help.ubuntu.com/community/CompilingEasyHowTo)

*RHEL/CentOS and Fedora* [How to Install GNU GCC (C and C++ Compiler) and Development Tools](http://www.tecmint.com/install-c-c-compiler-and-development-tool-in-centos-fedora-redhat/)

## Dependencies

Install `xz-devel` or `xz-dev` via whatever package manager came with the build system installation above.

## Compiling free software

*These steps are applicable to most free software builds*

```
./autogen.sh to regenerate configure

./configure
make

make install
```
Anchor may be used to generate tape.c from tape.anch. The build should not ask for it, but if it does, anch may be found here:
http://anch.org/anchor.html
