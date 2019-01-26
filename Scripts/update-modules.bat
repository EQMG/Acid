cd ../

git submodule update --init --recursive
git submodule foreach git fetch origin
git submodule foreach git reset --hard origin/master
git submodule foreach git pull origin master
