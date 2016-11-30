# Things All People Enjoy (TAPE)
<img style="float:right" src="https://encrypted-tbn2.gstatic.com/images?q=tbn:ANd9GcRGxXrtxYkj6fjku-t4RsWpM8n8UUbsAKN7qpKCWd1eon1KlwFV">

Stick it all together. Compatible with current players!

    * Stick a bunch of movies or music together
    * Write on the back of digital photos
    * Attach reference info to downloads
    * Remind people where that sh't came from

# How it works

Did you know `cat` can string media files together?

And that most media players will play them?

Tape is sort of like that.

# Tape is better than cat
Things on tape

    * can be extracted later
    * can be optionally compressed

For example, `tape` can combine `t.mp3, a.mp3, `and` pe.mp3` into `tape.mp3`.

`tape t.mp3 a.mp3 pe.mp3 tape.mp3`

With different types of media, players should stop after the first one.

Make sure to give the last file the same file extension as the first, or it defeats the purpose of having a playable archive format.

# Tape also works on `.jpg`, `gif`, `.mp4`...

Most media may be taped together

    * The first one serves as a preview
    * Existing viewers only show the first image
    * Newer viewers will be able to flip through
    * New players will be able to read animations

# create playable "tapes"
`tape [options] [media] [attachments] ... output_media`

      -c optionally compress attachments with LZMA
      
      The first file remains playable, and is never compressed.

#Extraction
`tape [options] [attachments] ... input_media`

      -l List media attachments
      
      -x eXtract [attachments] from input_media
      
      -p extract [attachments] from input_media to pipe
      
      -h Help
      
# More examples
List files in bar.mp4
        tape -l bar.mp4
        
        foo.mp3, 20405
        bar.mp4, 102080
        
        (The numbers represent file size.)
        
Use ffplay to play foo.mp3 that has been attached to bar.mp4

        tape -p foo.mp3 bar.mp4 | ffplay

# Build Instructions

        ./configure
        make

        make install
        
Anchor may be used to generate tape.c from tape.anch. The build should not ask for it, but if it does, anch may be found here:
http://anch.org/anchor.html
