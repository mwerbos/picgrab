g++ `Magick++-config --cxxflags --cppflags` -O0 \
  -o pixelgrab -g pixelgrab.cpp \
  `Magick++-config --ldflags --libs`
