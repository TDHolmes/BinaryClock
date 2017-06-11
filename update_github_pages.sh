#/usr/bash
echo "Make sure you've run `python documentation_build.py` to make sure everything is up to date!"

# update documentation
python documentation_build.py

# copy documentation assets out of the repo
cp -r software/build/html ../sphinxDocumentation
cp -r documents/doxygenDocumentation/html ../doxygenDocumentation

# switch branches
git checkout gh-pages

# copy the built documentation into the gh-pages branch
cp -r ../sphinxDocumentation documents/sphinxDocumentation
cp -r ../doxygenDocumentation documents/doxygenDocumentation
