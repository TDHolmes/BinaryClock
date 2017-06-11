#/usr/bash
echo "Make sure you've run `python documentation_build.py` to make sure everything is up to date!"

# update documentation
python documentation_build.py

# copy documentation assets out of the repo
cp -r software/build/html ../sw_docs
cp -r documents/doxygenDocumentation/html ../fw_docs

# switch branches
git checkout gh-pages

# copy the built documentation into the gh-pages branch
cp -r ../sw_docs software_documentation
cp -r ../fw_docs firmware_documentation
