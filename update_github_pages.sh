#/usr/bash
echo "Updating github documentation!"

printf "\n\n"
echo "update documentation..."
python documentation_build.py

printf "\n\n"
echo "copy documentation assets out of the repo..."
cd software/build/html
cp -r ./ ../../../../sphinxDocumentation
cd ../../..
cd documents/doxygenDocumentation/html
cp -r ./ ../../../../doxygenDocumentation
cd ../../..

printf "\n\nwaiting..."
read
echo "switch branches..."
git checkout gh-pages

printf "\n\nwaiting..."
read
echo "copy the built documentation into the gh-pages branch..."
cd ..
cp -r sphinxDocumentation BinaryClock/documents
cp -r doxygenDocumentation BinaryClock/documents
cd BinaryClock
