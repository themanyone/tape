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
* **Free**. Copy, share, even sell and support Tape, but please link to the [author's web site](http://thenerdshow.com).

## Mission Statement: Putting It All Together.

* Bundle a bunch of movies or music into one playable file.
* Easily take them apart again.
* Make videos longer without editing the original.
* Put an intro and outro onto videos without editing.
* Arrange pre-made media scenes like lego bricks.
* Attach reference info to downloads.
* Attach stories to those digital photos.
* Remind people where that sh't came from.
* Memories, man. Memories.

## How It Works

Did you know `cat` can string media files together?

And that most media players will play them?

Tape is sort of like that.

## Tape is better than cat

* Taped attachments can be easily extracted.
* Tape can compress attachments with lzma.

For example, `tape` can combine `t.mp3`, `a.mp3`, and `pe.mp3` into a new `tape.mp3`.

`tape t.mp3 a.mp3 pe.mp3 tape.mp3`

In this case `tape.mp3` has the same file extension as `t.mp3` does, so the resulting archive will be playable (recommended).

**Note on combining different types of media**. This is fine to do. Most players will stop playing when they encounter a different type of media in the stream though. Even when combining the same types of media files there could be hiccups that will stop some players from continuing, such as different bitrate, frame size, or compression scheme. The free console media player, [mpg123](https://www.mpg123.de/) calles these "Frankenstein streams". Tools like ffmpeg may be used to convert attachments into the same format, if necessary.

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

## Get It Now

You should start playing with tape immediately. Download from https://github.com/themanyone/tape or use [git](https://git-scm.com/download/win):

`git clone https://github.com/themanyone/tape.git`

## Metadata Alternatives

Tape allows us to attach any kind of data to files in a more or less uniform way, even metadata. But maybe we don't want to wantonly tape metadata onto files when there are already standards for that. *Or maybe we do, because there are too many competing standards.* Here is what we need to know about metadata along with its advantages and disadvantages.

* [How to tell where a file was downloaded from in Windows](http://superuser.com/questions/695507/find-out-where-a-file-was-downloaded-from)
* [What EXIF Photo Data Is, How to Find It & How to Understand It](http://www.makeuseof.com/tag/exif-photo-data-find-understand/)
* [ID3 metadata tags](https://en.wikipedia.org/wiki/ID3)
* [Metadata means "data about data"](https://en.wikipedia.org/wiki/Metadata)
* [More metadata formats](http://www.ifla.org/book/export/html/8817)

## Build System Configuration

Everyone who considers themselves at all technically inclined should gain access to a [GNU Build System](https://en.wikipedia.org/wiki/GNU_Build_System) post haste in order to avail themselves of the abundance of [free and open source (FOSS) software](http://www.freeopensourcesoftware.org/index.php?title=Main_Page) available for Windows, Linux, Android, iPhones, and many other types of computers and devices. FOSS software is distributed as [source code](https://en.wikipedia.org/wiki/Source_code) and must be compiled before use. Different [compilers and build systems](https://en.wikipedia.org/wiki/Compiler) exist for each type of system.

### Windows Development

The best, free way to get started with development for Windows is with [Cygwin](http://www.cygwin.com/), [MinGW](http://blog.florianwolters.de/educational/2013/11/21/Installing-MinGW/) or MSYS (available through MinGW's mingw-get). Either one will compile free software. To build true windows programs and DLLs, use [MinGW](http://www.mingw.org/), or [MinGW via Cygwin](https://www.ntu.edu.sg/home/ehchua/programming/howto/Cygwin_HowTo.html). One may build Windows programs on Cygwin without using MinGW, but they will depend on `Cygwin.dll` to run.

*Installing Minimalist GNU for Windows (MinGW) on Windows*

* [unofficial howto](http://blog.florianwolters.de/educational/2013/11/21/Installing-MinGW/)

* [official documentation](http://www.mingw.org/wiki/howto_install_the_mingw_gcc_compiler_suite)

*Installing Cygwin*

* [unofficial howto](https://www.ntu.edu.sg/home/ehchua/programming/howto/Cygwin_HowTo.html)

* [official documentation](http://www.cygwin.com/)

Note: It is also possible to use [Microsoft Visual Studio (MSVC)](https://folti.blogs.balabit.com/2009/08/compiling-autoconfmake-projects-under-msvc-part-one/) to build FOSS software, but the task is not recommended for beginners.

### Linux Development

Different brands of Linux each have their own way of installing software, including compilers and build systems.

*Ubuntu* [Setting up a build system on Ubuntu](https://help.ubuntu.com/community/CompilingEasyHowTo)

*RHEL/CentOS and Fedora* [How to Install GNU GCC (C and C++ Compiler) and Development Tools](http://www.tecmint.com/install-c-c-compiler-and-development-tool-in-centos-fedora-redhat/)

## Dependencies

Install `xz-devel` or `xz-dev` via whatever package manager came with the build system installation above.

Ubuntu: `lzma and lzma-dev`

## Compiling Free Software

*These steps are universally applicable to almost all free software builds*

```
./autogen.sh to regenerate configure

./configure
make

make install
```
*Anchor may be used to generate tape.c from tape.anch. The build should not ask for it, but if it does, Anchor may be found here:*
http://anch.org/anchor.html
