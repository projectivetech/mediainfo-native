# MediaInfoNative

This gem provides a native ruby extension to interface with the [Mediainfo library](http://mediaarea.net/en/MediaInfo/).

# Usage

The gem is designed as a drop-in replacement for the [Mediainfo gem](https://github.com/FlavourSys/mediainfo) (original version [here](https://github.com/greatseth/mediainfo)), but uses the MediaInfoLib directly in order to improve performance and reliability. Whenever possible the original API has been made available.

## Disable continuous file names detection

MediaInfo performs some detection on continuous file names (e.g., image sequences) and might therefore for example report `video?` as `true` where you expect it to be `false`. If you want to disable this feature, instantiate with:

```
MediaInfoNative::MediaInfo.new(ignore_continuous_file_names: true)
```

# MediaInfoLib

The MediaInfoLib does not automatically install the `libmediainfo.pc` file. You need to do this manually until I care to make the gem not fail when it doesn't exist.

# License

The gem is licensed under the MIT License. See the [LICENSE](https://raw2.github.com/FlavourSys/mediainfo-native/master/LICENSE) file for more information.

Parts of the code have been reused from the old Mediainfo or at least are heavily inspired from the old code (esp. the convenience API). The Mediainfo gem is licensed under the [WTF Public License](http://www.wtfpl.net/txt/copying/) and we guess it is conformant with the terms of that license to re-license the code under the MIT license.
