#!/usr/bin/env bash
#
# Usage: ./generate_images <background_file_to_use> <landscape*|portrait*>
#
# Default values are: ./generate_images pics/0.png landscape

set -e

SCRIPT_DIR=$( cd -- "$( dirname -- "${BASH_SOURCE[0]}" )" &> /dev/null && pwd )
IMAGES_DIR=$SCRIPT_DIR/../ui

BACKGROUND=${1-"$IMAGES_DIR/pics/0.png"}
VARIANT=${2-"landscape"}

magick_command() {
  if command -v magick >/dev/null 2>&1; then
    pushd "$IMAGES_DIR"
      magick "$@"
    popd
  elif command -v convert >/dev/null 2>&1; then
    pushd "$IMAGES_DIR"
      convert "$@"
    popd
  else
    docker run --rm -v "${IMAGES_DIR}:/imgs" dpokidov/imagemagick "$@"
  fi
}

# Cleanup
rm -f "$IMAGES_DIR"/"$VARIANT"/*.png "$IMAGES_DIR"/"$VARIANT"/*.jpg || true
mkdir -p "$IMAGES_DIR/$VARIANT"

if [[ $VARIANT = landscape* ]]; then
  echo "Generating Landscape images"
else
  echo "Generating Portrait images"
fi

# 0: Background
cp "$BACKGROUND" "$IMAGES_DIR/$VARIANT/0.png"

# 1-15 Weather icons
for i in {1..15}; do
  if [[ $VARIANT = landscape* ]]; then
    magick_command "$VARIANT/0.png" "pics/$i.png" -geometry +20+45 -composite -background black -flatten -crop 100x100+20+45 +repage "$VARIANT/$i.png"
  else
    magick_command "$VARIANT/0.png" "pics/$i.png" -geometry +20+154 -composite -background black -flatten -crop 100x100+20+154 +repage "$VARIANT/$i.png"
  fi
done

# 17 light.colorwheel
# 19,20 cover.coverslider
# 21 light.tempslider
# 22,23 light.light_b_press
# 24,25 light.temp_b_press
# 26,27 light.color_b_pres
# 28,29 light.lightslider
if [[ $VARIANT = landscape* ]]; then
  magick_command "$VARIANT/0.png" "pics/17.png" -geometry +106+68 -composite -background black -flatten -crop 200x200+106+68 +repage "$VARIANT/17.png"
  magick_command "$VARIANT/0.png" "pics/19.png" -geometry +183+67 -composite -background black -flatten -crop 83x233+183+67 +repage "$VARIANT/19.png"
  magick_command "$VARIANT/0.png" "pics/20.png" -geometry +183+67 -composite -background black -flatten -crop 83x233+183+67 +repage "$VARIANT/20.png"
  magick_command "$VARIANT/0.png" "pics/21.png" -geometry +164+67 -composite -background black -flatten -crop 85x235+164+67 +repage "$VARIANT/21.png"
  magick_command "$VARIANT/0.png" "pics/22.png" -geometry +389+74 -composite -background black -flatten -crop 50x50+389+74 +repage "$VARIANT/22.png"
  magick_command "$VARIANT/0.png" "pics/23.png" -geometry +389+74 -composite -background black -flatten -crop 50x50+389+74 +repage "$VARIANT/23.png"
  magick_command "$VARIANT/0.png" "pics/24.png" -geometry +389+138 -composite -background black -flatten -crop 50x50+389+138 +repage "$VARIANT/24.png"
  magick_command "$VARIANT/0.png" "pics/25.png" -geometry +389+138 -composite -background black -flatten -crop 50x50+389+138 +repage "$VARIANT/25.png"
  magick_command "$VARIANT/0.png" "pics/26.png" -geometry +389+201 -composite -background black -flatten -crop 50x50+389+201 +repage "$VARIANT/26.png"
  magick_command "$VARIANT/0.png" "pics/27.png" -geometry +389+201 -composite -background black -flatten -crop 50x50+389+201 +repage "$VARIANT/27.png"
  magick_command "$VARIANT/0.png" "pics/28.png" -geometry +164+67 -composite -background black -flatten -crop 85x235+164+67 +repage "$VARIANT/28.png"
  magick_command "$VARIANT/0.png" "pics/29.png" -geometry +164+67 -composite -background black -flatten -crop 85x235+164+67 +repage "$VARIANT/29.png"
else
  magick_command "$VARIANT/0.png" "pics/17.png" -geometry +61+211 -composite -background black -flatten -crop 200x200+61+211 +repage "$VARIANT/17.png"
  magick_command "$VARIANT/0.png" "pics/19.png" -geometry +119+143 -composite -background black -flatten -crop 83x233+119+143 +repage "$VARIANT/19.png"
  magick_command "$VARIANT/0.png" "pics/20.png" -geometry +119+143 -composite -background black -flatten -crop 83x233+119+143 +repage "$VARIANT/20.png"
  magick_command "$VARIANT/0.png" "pics/21.png" -geometry +117+194 -composite -background black -flatten -crop 85x235+117+194 +repage "$VARIANT/21.png"
  magick_command "$VARIANT/0.png" "pics/22.png" -geometry +25+71 -composite -background black -flatten -crop 50x50+25+71 +repage "$VARIANT/22.png"
  magick_command "$VARIANT/0.png" "pics/23.png" -geometry +25+71 -composite -background black -flatten -crop 50x50+25+71 +repage "$VARIANT/23.png"
  magick_command "$VARIANT/0.png" "pics/24.png" -geometry +134+71 -composite -background black -flatten -crop 50x50+134+71 +repage "$VARIANT/24.png"
  magick_command "$VARIANT/0.png" "pics/25.png" -geometry +134+71 -composite -background black -flatten -crop 50x50+134+71 +repage "$VARIANT/25.png"
  magick_command "$VARIANT/0.png" "pics/26.png" -geometry +243+71 -composite -background black -flatten -crop 50x50+243+71 +repage "$VARIANT/26.png"
  magick_command "$VARIANT/0.png" "pics/27.png" -geometry +243+71 -composite -background black -flatten -crop 50x50+243+71 +repage "$VARIANT/27.png"
  magick_command "$VARIANT/0.png" "pics/28.png" -geometry +117+194 -composite -background black -flatten -crop 85x235+117+194 +repage "$VARIANT/28.png"
  magick_command "$VARIANT/0.png" "pics/29.png" -geometry +117+194 -composite -background black -flatten -crop 85x235+117+194 +repage "$VARIANT/29.png"
fi

# 39,40 Fan settings
for i in 39 40; do
  if [[ $VARIANT = landscape* ]]; then
    magick_command "$VARIANT/0.png" "pics/$i.png" -geometry +183+67 -composite -background black -flatten -crop 83x233+183+67 +repage "$VARIANT/$i.png"
  else
    magick_command "$VARIANT/0.png" "pics/$i.png" -geometry +119+143 -composite -background black -flatten -crop 83x233+119+143 +repage "$VARIANT/$i.png"
  fi
done

# 32 Splash screen
# 33,34 Settings page
# 37 Climate controls
# 46,47 Home Alt
for i in 32 33 34 37 46 47; do
  if [[ $VARIANT = landscape* ]]; then
    magick_command "$VARIANT/0.png" "pics/$i.png" -geometry +0+0 -composite -background black -flatten -crop 480x320 +repage "$VARIANT/$i.png"
  else
    magick_command "$VARIANT/0.png" "pics/$i-portrait.png" -geometry +0+0 -composite -background black -flatten -crop 320x480 +repage "$VARIANT/$i.png"
  fi
done

# Keep these with black background, either as they are on a black screen or they don't have transparency (e.g. sliders)

# 16: Slider cursor on Settings page
# 18: Slider cursor on Cover, Fan & Light pages
# 30,31: Indicators - Not in use
# 38: Slider on Climate page
# 44,45: Alarm
# 35,36,41,42,43: Unused?
for i in 16 18 {30..31} 35 36 38 {41..45}; do
  cp "${IMAGES_DIR}/pics/$i.png" "${IMAGES_DIR}/$VARIANT/$i.png"
done
