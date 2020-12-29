echo "building util.."
cd ../util
make clean
make

echo "building db"
cd ../db
make clean
make

echo "building responder"
cd ../responder
make clean
make all
make install
