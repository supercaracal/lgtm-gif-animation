# LGTM GIF ANIMATION

Append LGTM frame to gif animation.

## Build

```
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
