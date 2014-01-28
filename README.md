# MediaInfoNative

This gem provides a native ruby extension to interface with the [Mediainfo library](http://mediaarea.net/en/MediaInfo/).

# Usage

The gem is designed as a drop-in replacement for the [Mediainfo gem](https://github.com/FlavourSys/mediainfo) (original version [here](https://github.com/greatseth/mediainfo)), but uses the MediaInfoLib directly in order to improve performance and reliability. Whenever possible the original API has been made available.

# License

The gem is licensed under the MIT License. See the [LICENSE](https://raw2.github.com/FlavourSys/mediainfo-native/master/LICENSE) file for more information.

Parts of the code have been reused from the old Mediainfo or at least are heavily inspired from the old code (esp. the convenience API). The Mediainfo gem is licensed under the [WTF Public License](http://www.wtfpl.net/txt/copying/) and we guess it is conformant with the terms of that license to re-license the code under the MIT license.
