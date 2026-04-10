bash ../scripts/generate_images.sh pics/0.png landscape
if errorlevel 1 timeout /t 10
bash ../scripts/generate_images.sh pics/0-portrait.png portrait
if errorlevel 1 timeout /t 10
bash ../scripts/generate_images.sh pics/0-white.png landscape-white
if errorlevel 1 timeout /t 10
bash ../scripts/generate_images.sh pics/0-portrait-white.png portrait-white
if errorlevel 1 timeout /t 10
 
timeout /t 30


