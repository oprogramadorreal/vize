mkdir vc16-win64
cd vc16-win64

cmake -G"Visual Studio 16 2019" -DCMAKE_INSTALL_PREFIX=vc16-win64-deploy ../..

cd ..
pause