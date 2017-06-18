# CMSSW

Introduction to CMSSW: http://cms-sw.github.io

If you are a neuphysics member you should **not** have to repeat the installation steps. 

## Installing CMSSW on Rivanna 
These instructions were combined from the instructions for the [Texas A&M Brazos cluster](http://mitchcomp.physics.tamu.edu/mitchcomp/guides/admin/CMSSWSystemInstallGuide.pdf) and this [twiki](https://twiki.cern.ch/twiki/bin/view/CMSPublic/SDTCMSSW_aptinstaller) (you will need a CERN account to view this link). If my account ever gets suspended on Rivanna someone will have to repeat these steps in their scratch directory.

### Dependencies 

There are a variety of depencies that CMSSW needs to run. Lucky for us all of them are already installed on Rivanna. These packages include which, wget, tk, tcsh, zsh, freetype, fontconfig, compat-libstdc++-33, libidn, libXmu, libXcursor, libXrandr, libXft, mesa-libGLU,libXi, libXinerama, libXpm. You can check if a package is installed on your computer by using the rpm command 
```bash
$ rpm -qa | grep wget
```
Note that this command will only find the application if it is owned by a package and is installed as an rpm. According to Rivanna's help desk, this is often not the case for most systems. For example, on Rivanna zsh is not owned by any package. We will have to change the setup script shortly to account for this.  

**Important**

To get the latest versiions of CMSSW, we will need to load a more recent version of gcc. 
```bash
$ module load gcc/5.4.0
```

### Setting up the environment

This section assumes you are using the bash shell. If you use csh you will have to use setenv instead of export. 

**Step 1** -  Make your cmssw directory. The install will be large, so I did this in scratch. 
```bash
$ mkdir cmssw
```
**Step 2** -  Set environmental variables

```bash
$ export VO_CMS_SW_DIR=/scratch/$USER/cmssw
$ export LANG=C
```

**Step 3** - Get and edit bootstrap script
Download bootstrap.sh using 
```bash
$ wget -O $VO_CMS_SW_DIR/bootstrap.sh http://cmsrep.cern.ch/cmssw/repos/bootstrap.sh
```
This script uses the rpm command mentioned earlier to search for dependecies. Because zsh is not owned by a package on Rivanna, this script will be unable to find zsh and return an error as is. To fix this, open bootstrap.sh in your favorite editor and go to line 1503. At this point change the for loop to read as follows. 

```bash
for p in $requiredSeeds; do
    if [ $p != 'zsh' ]
    then
        rpm -q $p >/dev/null || missingSeeds="$missingSeeds $p"
    fi
done
```
We simply just told the script to not check for zsh. Don't run the script just yet. 

**Step 4** - Set your architecture 

Your architecture is a combination of your OS, your processor type, and compiler. To use the latest versions of cmssw, use

```bash
$ export SCRAM_ARCH=slc6_amd64_gcc530
```
Rivanna uses a 64bit processor, and uses CentOS 6.8, which I guess is close enough to scientific linux. We loaded version 5.4.0 of gcc earlier, so that should be good too. 

**Step 5** - Run the script (only should do one time per architecture)
Execute bootstrap.sh using

```bash
sh -x $VO_CMS_SW_DIR/bootstrap.sh setup -path $VO_CMS_SW_DIR -arch $SCRAM_ARCH >& $VO_CMS_SW_DIR/bootstrap_$SCRAM_ARCH.log
```
Remember if you log out between steps 1-5 you will have to reset your environmental variables. This script will then run in the background while producing a log file you can periodically open. If the script is successful, you should see something like

....
**Everything completed correctly.**
....

at the end of the log file. 
### Installing

Nothing too complicated here, just two commands. You will need to do this for every future release you install. In the second command you select the version of cmssw you want. 
```bash
$ $VO_CMS_SW_DIR/common/cmspkg -a $SCRAM_ARCH update
$ $VO_CMS_SW_DIR/common/cmspkg -a $SCRAM_ARCH install cms+cmssw+CMSSW_8_0_21
```
This will take a while. 

**(optional)** - changing permissions

We can change the group of the cmssw directory using 
```bash
$ chgrp <shared-group> cmssw
```
In our case <shared-group> is neuphysics. Next, we can change permissions with

```bash
$ chmod g+rx cmssw
```

## Using CMSSW

**Step 1** setting up environment

it is likely best to have the following in your ~/.bash_profile

```bash
umask 0022 #default permissions
export VO_CMS_SW_DIR=/scratch/emc5ud/cmssw
export SCRAM_ARCH=slc6_amd64_gcc530   #choose the desired scram_arch
source $VO_CMS_SW_DIR/cmsset_default.sh
ulimit -s 11000 #
```

**Step 2** getting your appropriate version of CMSSW

```bash
scram p CMSSW_8_0_21 #again, your version of choice
```
THe versions available to you depends on what I have installed on the system and your architecture. As of writing this, I only have version 8_0_21 installed on rivanna. Contact me at emc5ud@virginia.edu if you want a different version. 

### Running Jobs
 TBA
