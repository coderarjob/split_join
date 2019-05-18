# File splitter and joiner

The idea of this splitter and joiner came because I had to move a few large
files from one computer to another via a shared folder that has much less free
space than the size of the files that I wanted to transfer.

This application takes the large file and creates multiple smaller files of the
size you specify.

## Usage
`
split [-j | -s -sz <filesize (mb|gb|kb)> source destination
`

### Options:

* **-j** : Joins multiple files into one larger file. The path to the **1st** split 
file is specifed in __source__ and the path of the destination file in 
__destination__
* **-s** : Splits the file specified in the __source__ into many smaller files
 as one specied in __destination__.
* **-sz** : Size of each split is specied as `100mb`. You can use GB, MB, KB with a 
number to specify size. If only a number is entered unit **bytes** is assumed.

## Examples:
`
split Windows.iso -s -sz 700mb splitted/Windows.iso
`
Splits Windows.iso into multiples files 700MB of size each inside the splitted
direcctory as _Windows.iso.1_, _Windows.iso.2_ and so on.

`
split -j splitted/Windows.iso.1 Windows.iso
`
Joins the multiple splitted files into one large file.

#### Notes:

This was a quick application to program. Feel free to contribute or clone it to
make it your own. 
