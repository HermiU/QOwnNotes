#!/usr/bin/env bash
#
# This is the build script for the openSUSE Build Service (OBS)
# https://build.opensuse.org/package/show/home:pbek:QOwnNotes/qownnotes
#
# We will need some packages to execute this locally:
# sudo apt-get install osc xz
#
# A file ~/.oscrc will be generated upon first start of osc
#

# uncomment this if you want to force a version
#QOWNNOTES_VERSION=0.84.5

BRANCH=develop
#BRANCH=master

DATE=$(LC_ALL=C date +'%a, %d %b %Y %T %z')
PROJECT_PATH="/tmp/QOwnNotes-$$"
CUR_DIR=$(pwd)


echo "Started the OBS source packaging process, using latest '$BRANCH' git tree"

if [ -d $PROJECT_PATH ]; then
    rm -rf $PROJECT_PATH
fi

mkdir $PROJECT_PATH
cd $PROJECT_PATH

echo "Project path: $PROJECT_PATH"

# checkout the source code
git clone --depth=50 git@github.com:pbek/QOwnNotes.git QOwnNotes -b $BRANCH
cd QOwnNotes

# checkout submodules
git submodule update --init

# remove huge .git folder
rm -Rf .git

if [ -z $QOWNNOTES_VERSION ]; then
    # get version from version.h
    QOWNNOTES_VERSION=`cat src/version.h | sed "s/[^0-9,.]//g"`
else
    # set new version if we want to override it
    echo "#define VERSION \"$QOWNNOTES_VERSION\"" > src/version.h
fi

# set the release string
echo "#define RELEASE \"openSUSE Build Service\"" > src/release.h

# replace the version in the spec file
sed -i "s/VERSION-STRING/$QOWNNOTES_VERSION/g" obs/qownnotes.spec

# replace the version in the PKGBUILD file
sed -i "s/VERSION-STRING/$QOWNNOTES_VERSION/g" obs/PKGBUILD

# replace the version in the dsc file
sed -i "s/VERSION-STRING/$QOWNNOTES_VERSION/g" obs/qownnotes.dsc

changelogText="Released $QOWNNOTES_VERSION"

echo "Using version $QOWNNOTES_VERSION..."

qownnotesSrcDir="qownnotes-${QOWNNOTES_VERSION}"
cd ..

# rename the directory
mv QOwnNotes $qownnotesSrcDir

changelogPath=$qownnotesSrcDir/obs/qownnotes.bin

# create the changelog file
echo "-------------------------------------------------------------------" > $changelogPath
echo "$DATE - patrizio@bekerle.com" >> $changelogPath
echo "" >> $changelogPath
echo "- $changelogText" >> $changelogPath

cat $changelogPath

archiveFile="$qownnotesSrcDir.tar.xz"
archiveFileDeb="$qownnotesSrcDir.orig.tar.gz"

# archive the source code
echo "Creating archive $archiveFile..."
tar -cJf $archiveFile $qownnotesSrcDir

#echo "Creating archive $archiveFileDeb..."
#tar -czf $archiveFileDeb $qownnotesSrcDir

echo "Checking out OBS repository..."

# checkout OBS repository
osc checkout home:pbek:QOwnNotes desktop

obsRepoPath="home:pbek:QOwnNotes/desktop"

# remove other archives
echo "Removing old archives..."
cd $obsRepoPath
osc rm *.xz
osc rm *.bz2
osc rm *.tar.gz
cd ../..

# copying new files to repository
mv $archiveFile $obsRepoPath
#mv $archiveFileDeb $obsRepoPath
cp $qownnotesSrcDir/obs/qownnotes.bin $obsRepoPath
cp $qownnotesSrcDir/obs/qownnotes.spec $obsRepoPath
cp $qownnotesSrcDir/obs/qownnotes.rules $obsRepoPath
cp $qownnotesSrcDir/obs/PKGBUILD $obsRepoPath
cp $qownnotesSrcDir/src/debian/changelog $obsRepoPath/debian.changelog
cp $qownnotesSrcDir/src/debian/control $obsRepoPath/debian.control
cp $qownnotesSrcDir/src/debian/copyright $obsRepoPath/debian.copyright
cp $qownnotesSrcDir/src/debian/compat $obsRepoPath/debian.compat
cp $qownnotesSrcDir/src/debian/qownnotes.install $obsRepoPath/debian.qownnotes.install
cp $qownnotesSrcDir/obs/qownnotes.dsc $obsRepoPath

cd $obsRepoPath

# add all new files
osc add $archiveFile
#osc add $archiveFileDeb
#osc add qownnotes.bin
#osc add qownnotes.spec
#osc add PKGBUILD
#osc add debian.changelog
#osc add debian.control
#osc add debian.rules
#osc add debian.copyright
#osc add debian.compat
#osc add debian.qownnotes.install
#osc add qownnotes.dsc

echo "Committing changes..."

# commit changes
osc commit -m "$changelogText"

# remove everything after we are done
if [ -d $PROJECT_PATH ]; then
    rm -rf $PROJECT_PATH
fi
