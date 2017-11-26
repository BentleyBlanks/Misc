# color grading

# usage

- generate color grading map
- generate 3d lut

# command

``` bash
Command:
    -grading        generate color grading map
    -lut            generate 3d lut

-grading:
    <save_path>     file extension must be bmp
    <grading_size>  must be power of two
    [dpi]           default value 72

-lut:
    <src_path>      file extension must be bmp
    <dst_path>      file extension must be bmp
    <lut_path>      file extension must be dds
    <grading_size>  must be power of two

    Note: bmp just support 24-bit uncompress version
```

# algorithm

[color grading algorithm description](http://www.lonelywaiting.com/color-grading/)