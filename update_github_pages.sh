#/usr/bash
echo "Updating github documentation!"

function check_retcode() {
    if [ $? -ne 0 ]
    then
        echo "Failure! Are all your folders correct and changes committed in git?"
        exit -1
    fi
}

printf "\n\n"
echo "update documentation..."
python documentation_build.py
check_retcode

printf "\n\n"
echo "copy documentation assets out of the repo..."
cd software/build/html
cp -r ./ ../../../../sphinxDocumentation
check_retcode
cd ../../..
cd documents/doxygenDocumentation/html
cp -r ./ ../../../../doxygenDocumentation
check_retcode
cd ../../..

printf "\n\n"
echo "switch branches..."
git checkout gh-pages
check_retcode

printf "\n\n"
echo "copy the built documentation into the gh-pages branch..."
cd ..
cp -r sphinxDocumentation BinaryClock/documents
check_retcode
cp -r doxygenDocumentation BinaryClock/documents
check_retcode
cd BinaryClock

printf "\n\n"
echo "Adding the changes to git (if any)..."
git add .
check_retcode
git commit -m "[automated script] Updating documentation..."

printf "\n\n"
echo "Jumping back to master..."
git checkout master
check_retcode

echo "Done!"
