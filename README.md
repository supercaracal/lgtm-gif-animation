# LGTM GIF ANIMATION

Append LGTM frame to gif animation.

![lgtm](lgtm.gif "LGTM")

<!-- base64:lgtm.gif
R0lGODlhgABAAPcAAAAAAAAAMwAAZgAAmQAAzAAA/wArAAArMwArZgArmQArzAAr/wBVAABVMwBV
ZgBVmQBVzABV/wCAAACAMwCAZgCAmQCAzACA/wCqAACqMwCqZgCqmQCqzACq/wDVAADVMwDVZgDV
mQDVzADV/wD/AAD/MwD/ZgD/mQD/zAD//zMAADMAMzMAZjMAmTMAzDMA/zMrADMrMzMrZjMrmTMr
zDMr/zNVADNVMzNVZjNVmTNVzDNV/zOAADOAMzOAZjOAmTOAzDOA/zOqADOqMzOqZjOqmTOqzDOq
/zPVADPVMzPVZjPVmTPVzDPV/zP/ADP/MzP/ZjP/mTP/zDP//2YAAGYAM2YAZmYAmWYAzGYA/2Yr
AGYrM2YrZmYrmWYrzGYr/2ZVAGZVM2ZVZmZVmWZVzGZV/2aAAGaAM2aAZmaAmWaAzGaA/2aqAGaq
M2aqZmaqmWaqzGaq/2bVAGbVM2bVZmbVmWbVzGbV/2b/AGb/M2b/Zmb/mWb/zGb//5kAAJkAM5kA
ZpkAmZkAzJkA/5krAJkrM5krZpkrmZkrzJkr/5lVAJlVM5lVZplVmZlVzJlV/5mAAJmAM5mAZpmA
mZmAzJmA/5mqAJmqM5mqZpmqmZmqzJmq/5nVAJnVM5nVZpnVmZnVzJnV/5n/AJn/M5n/Zpn/mZn/
zJn//8wAAMwAM8wAZswAmcwAzMwA/8wrAMwrM8wrZswrmcwrzMwr/8xVAMxVM8xVZsxVmcxVzMxV
/8yAAMyAM8yAZsyAmcyAzMyA/8yqAMyqM8yqZsyqmcyqzMyq/8zVAMzVM8zVZszVmczVzMzV/8z/
AMz/M8z/Zsz/mcz/zMz///8AAP8AM/8AZv8Amf8AzP8A//8rAP8rM/8rZv8rmf8rzP8r//9VAP9V
M/9VZv9Vmf9VzP9V//+AAP+AM/+AZv+Amf+AzP+A//+qAP+qM/+qZv+qmf+qzP+q///VAP/VM//V
Zv/Vmf/VzP/V////AP//M///Zv//mf//zP///wAAAAAAAAAAAAAAACH5BAEAAPwALAAAAACAAEAA
AAj/AAEIHEiwoMGDCBMqXMiwocOHECNKnEixosWLGDNq3Mixo8ePIEOKHEmypMmTKFOqXMmypcuX
MGPKnEmzps2bOHPq3Mmzp8+fQIPiDBDjRowYFQ+IQbNFhcEDRosejUp1KlKiRhViRVozzr6v+4JF
DAAnGtiv+hYNFGD2rNu3+xR5/Sr2IFuwhLqeLQdRR1u4wpwKOAW38D59hOYepnLQCFjEesEqeujY
8Nd5Ku5adqtP7tldBjUfnkxT8T5aDiFsBstO9OrDic/qY0wQiOy8pc/qauhaXy0YAWQsk13Ih5jj
x9WcHYYcuQrTYQm63od75p2zpBdWRktbYIBQYIUd/7Rwtm7B67K7b0dbXaZp1Au/P24vUPU+8Qcf
7D0IfR87gdN1FtlX2SWUwF//HZQDMgmRB5Z5BPUH2XrsDbjPbgvZ9xV8EekHFmgG9effdF/RB5Np
BSK0nYASaRidQehxVgxc+mhhIYcJKTbbRB5+xVeIrz2WIkwxxhUfeF/l091WVY1XHn+PDceZJvNZ
iGFC8nE3kIhGFtTjPiAWpFg+ZLzFThBnmfgSikeCpeSWNJroIoRwolUFkuytqKZL7zFkWnUiQubl
fkBy9+V/tqE15JrY+dmoQEVWWZCDdB0Uo5IBELYYAEdIOpNpVya03joDEXWUcpIZ9GWYET7GmH11
JetaYk1FLlrQgWBhZlCnFRY0J5RJdlebp+6dhSNCrh0LgKyCEvTlj2K62piQFtpakKyH2UiQngZR
+mK0wR6EZq+fnsXOUsehgZy6MACoKVq1OBWADlISqOpnwG5q0Hp7tsQlXBxSaFizA/0Ko5vCDsRs
vyz9+xYkw65W471gQduqlgYxay1LkVp2rBF/AeZUt08Wus+buxIbk8NuKYsAlW8No21++Jq8Y8Zp
1mSqVTxLxRVCMiAXRkM+j1zQVu0etPMNQjXt9NNQRy311FRXbfXVWGet9dZcd+3112CHLfbYZJdt
9tlop6021gEBADs=
-->

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
