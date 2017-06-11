#/usr/bash
echo "Updating github documentation!"

printf "\n\n"
echo "update documentation..."
python documentation_build.py

printf "\n\n"
echo "copy documentation assets out of the repo..."
cp -r software/build/html ../sphinxDocumentation
cp -r documents/doxygenDocumentation/html ../doxygenDocumentation

printf "\n\n"
echo "switch branches..."
git checkout gh-pages

printf "\n\n"
echo "copy the built documentation into the gh-pages branch..."
cp -r ../sphinxDocumentation documents
cp -r ../doxygenDocumentation documents
