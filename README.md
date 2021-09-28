# Things All Players Enjoy (TAPE)

### Universal metadata format, media tagging, and archive tool.

Tape attaches metadata to files. You can also create archives. And attach metadata to archives. Use the same file extension as the original file to create long, "playable archives" that media players will play. Even do some very basic video editing from the command line.

<img style="float:right" src="https://encrypted-tbn2.gstatic.com/images?q=tbn:ANd9GcRGxXrtxYkj6fjku-t4RsWpM8n8UUbsAKN7qpKCWd1eon1KlwFV">Brought to you by [TheNerdShow.com](http://thenerdshow.com).

## The Vision of Tape

* **Simple**. Create archives, like zip.
* **Compatible** Tape archives are viewable with media players and viewers.
* **Versatile**. Any types of files and data can be joined with tape.
* **Reversible**. Archives contain an inventory catalog at the end, for later extraction.
* **Stable**. Tape is a command-line tool. Graphical interfaces may follow.
* **Private**. Files, media, and metadata stay on your device(s), and are never collected.
* **Fast**. No waiting for a large application or cloud to process and analyze your data.
* **Free**. Copy, share, even sell and support Tape, but please link to the [author's web site](http://thenerdshow.com).

## Mission Statement: Sticking It All Together.

* Universal metadata: Attach Author and References, so you know whom to credit.
* Tape can join whole folders of media into playable archives.
* Add your custom intro or outro clips to videos.
* Arrange pre-made media slides, chapters, or scenes like LEGO&trade; bricks.
* Combine individual images into playable movies.
* No more "mystery files." 
* Attach text or audio clips to digital photos. Who was that in the picture?
* Remind yourself what research project a file belongs to.
* With tape, your data stays with the file, wherever it goes.

## How It Works

Did you know `cat` can string media files together into one big, long playlist?

And that most media players will play it?

Tape is sort of like that. Except tape can also take them apart again.

**Why not just use `tar`?** I could have used `tar` for this. But `tar` adds extra overhead and padding to files that make media players refuse to play them. It was important to me that `tape` archives be *playable*, and thus, to remain compatible with media players, they can even keep the same file extensions as the original file(s). You are free to use a different file extension if you want, however. The .tape extension works for me.

**Why not use Adobe XMP?** I did not want extra files kicking around with the same names but different extensions. I did not want hidden files or separate databases. I did not want a "standardized data model of serialization with core properties for the definition and processing of extensible metadata," whatever that means. I did not want to require my users to learn XML (although there is no restriction from attaching XML files with Tape, if they want to). I especially did not want a bureau of standards or commercial paywalls controlling my metadata.

## Examples

Using `tape` to combine `t.mp3`, `a.mp3`, and `pe.mp3` into a new `tape.mp3`.

`tape t.mp3 a.mp3 pe.mp3 tape.mp3`

Notice that `tape.mp3` has the `.mp3` file extension, so media players will play it. For best results, all media files in the archive should share the same bit rate and encoding. Use other programs to edit the ID3 tags, or simply use `tape` to attach a database file or text file containing all the metadata you want to add about the archive at the end. There are no restrictions on the type of metadata to include, or not include. The file, `pe.mp3` could even be a voice recording of the metadata. Or we could tape a text file onto the end. It is all up to you.

**Note on combining different types of media**. You can put any kind of files and data into a tape archive. There are no restrictions. But if making a media archive, just remembetr that players will stop playing when they encounter a different type of media in the stream. Even when combining the same types of media files there could be hiccups that will stop some players from continuing, such as different bitrate, frame size, or compression scheme. The free console media player, [mpg123](https://www.mpg123.de/) calls these "Frankenstein streams". Tools like [ffmpeg](https://ffmpeg.org/) may be used to convert archive members into the same format, if necessary.

## Tape can make archival streams out of `.jpg`, `gif`, `.mp4`...

*Any* media may be taped together. But tape archives are unique among archive formats.

* The first file or document in Tape archives is never compressed.
* Tape archives may keep the original file extension.
* Using the same file extension makes tape archives playable, and viewable.
* The first image or video in the archive serves as a preview.
* Existing viewers may only show the first thumbnail.
* Newer players should be able to flip through all media in the archive.
* Newer viewers should be able to play the animations in web pages.

A note on compatibility with editors. You could, hypothetically, use a Word document as the first file in a tape archive. Then you could open the entire archive with Word. But we would advise against this. You should extract documents separately, before editing. Otherwise, the archive will be corrupted upon saving the document. Current versions of Word and Excel do not know what to do with tape archives. This is true with other programs as well. Maybe someday, if this tool catches on, newer apps will become "tape-aware."

## Create archives and Playable "Tapes" 
```

tape [options] [media] [attachments] ... output_file

-c optionally compress attachments with LZMA
      
```
## Extraction
```
tape [options] [attachments] ... input_file

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

List files in a tape archived named, `bar.mp4`.

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

Use ffplay to play "hidden" `foo.mp3` inside an archive named, `bar.mp4`.

`tape -p foo.mp3 bar.mp4 | ffplay`

## Get It Now

You should start playing with tape immediately. Download from https://github.com/themanyone/tape or use [git](https://git-scm.com/download/win):

`git clone https://github.com/themanyone/tape.git`

## Other Metadata Alternatives

Tape allows us to attach any kind of data to files in a more or less uniform way, even metadata. But maybe we don't want to wantonly tape metadata onto files when there are already standards for that. *Or maybe we do because there are too many competing standards.* Here is what we need to know about metadata along with its advantages and disadvantages.

* [How to tell where a file was downloaded from in Windows](http://superuser.com/questions/695507/find-out-where-a-file-was-downloaded-from)
* [What EXIF Photo Data Is, How to Find It & How to Understand It](http://www.makeuseof.com/tag/exif-photo-data-find-understand/)
* [ID3 metadata tags](https://en.wikipedia.org/wiki/ID3)
* [Metadata means "data about data"](https://en.wikipedia.org/wiki/Metadata)
* [About Adobe XMP metadata XML syntax for document serialization and embedding](https://en.wikipedia.org/wiki/Extensible_Metadata_Platform)
* [More metadata formats, oh my](http://www.ifla.org/book/export/html/8817)

## Build System Configuration

We use the standard [GNU Build System](https://en.wikipedia.org/wiki/GNU_Build_System), just like everyone else who is into [free and open source (FOSS) software](http://www.freeopensourcesoftware.org/index.php?title=Main_Page). Free software is available for Windows, Linux, Android, iPhones, and almost any computer or device. Open source software is distributed with [source code](https://en.wikipedia.org/wiki/Source_code) that may need to be compiled before use. Different [compilers and build systems](https://en.wikipedia.org/wiki/Compiler) exist for each type of system.

### Windows Development

The best, free way to get started with development for Windows is with [Cygwin](http://www.cygwin.com/), [MinGW](http://blog.florianwolters.de/educational/2013/11/21/Installing-MinGW/) or MSYS (available through MinGW's mingw-get). Either one will compile free software. To build true windows programs and DLLs, use [MinGW](http://www.mingw.org/), or [MinGW via Cygwin](https://www.ntu.edu.sg/home/ehchua/programming/howto/Cygwin_HowTo.html). One may build Windows programs on Cygwin without using MinGW, but they will depend on `Cygwin.dll` to run.

*Installing Minimalist GNU for Windows (MinGW) on Windows*

* [unofficial howto](http://blog.florianwolters.de/educational/2013/11/21/Installing-MinGW/)

* [official documentation](http://www.mingw.org/wiki/howto_install_the_mingw_gcc_compiler_suite)

*Installing Cygwin*

* [unofficial howto](https://www.ntu.edu.sg/home/ehchua/programming/howto/Cygwin_HowTo.html)

* [official documentation](http://www.cygwin.com/)

Note: It is certainly possible to use [Microsoft Visual Studio (MSVC)](https://folti.blogs.balabit.com/2009/08/compiling-autoconfmake-projects-under-msvc-part-one/) to build GNU software, but the task is not recommended for beginners.

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
