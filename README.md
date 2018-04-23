# LGTM GIF ANIMATION

Append LGTM frame to gif animation.

![lgtm](data:image/gif;base64,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)

## Installation

```
$ git clone https://github.com/supercaracal/lgtm-gif-animation.git
$ cd lgtm-gif-animation
$ make
```

## Usage

### If you need properties information

```
$ ./lgtm-gif-animation < input.gif > output.gif
```

### If you don't need properties information

```
$ ./lgtm-gif-animation < input.gif 1> output.gif 2> /dev/null
```

### If you want to test

```
$ ./lgtm-gif-animation < input.gif > /dev/null
```

## Support

`GIF89a` formatted animation file only

## Dependency

None

## See Also

* [Cover Sheet for the GIF89a Specification](https://www.w3.org/Graphics/GIF/spec-gif89a.txt)
* [ImageMagick/coders/gif.c](https://github.com/ImageMagick/ImageMagick/blob/master/coders/gif.c)
* [ImageMagick/MagickCore/compress.c](https://github.com/ImageMagick/ImageMagick/blob/master/MagickCore/compress.c)
