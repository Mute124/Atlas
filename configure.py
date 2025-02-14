import subprocess
import os
import sys

outputFolder = "build"
    
def IsConanInstalled():
    try:
        subprocess.check_call(['conan', '--version'])
        print("Conan is already installed\n")
        return True
    except:
        print("Conan is not installed\n")
        return False

def InstallConan():
    print("Installing Conan...")
    subprocess.check_call(['pip', 'install', 'conan'])
    print("Conan is now installed.\n")

def DoesBuildFolderExist():
    return os.path.exists(outputFolder)

def RemoveBuildFolder():

    os.rmdir(outputFolder)
    #os.chdir("build-scripts")

def CheckConanStep():
    print("Checking for Conan...")

    if IsConanInstalled() == False:
        InstallConan()

    print("Done checking for Conan.\n")

def InstallPackagesStep():
    print("Installing packages...")

    packageConfigsInstalled = [False, False, False]
    removeBuildDir = False
    

    for i in range(len(sys.argv)):
        if sys.argv[i] == "--debug":
            packageConfigsInstalled[0] = True
        elif sys.argv[i] == "--release":
            packageConfigsInstalled[1] = True
        elif sys.argv[i] == "--distribution":
            packageConfigsInstalled[2] = True
        elif sys.argv[i] == "--remove-build-folder":
            removeBuildDir = True

    if DoesBuildFolderExist() == True and removeBuildDir == True:
        print("Removing build folder...")
        print(os.curdir.capitalize())
        
        RemoveBuildFolder()
        print("Removed build folder.\n")

    profileExists = False
    try:
        profileExists = subprocess.check_call(['conan', 'profile', 'detect'])
    except:
        pass
    else:
        conanProfilePath = subprocess.getoutput(['conan', 'profile', 'path', 'default'])
            
        # Change compiler.cppstd in the file at conanProfilePath to compiler.cppstd=20
        fileTest = open(conanProfilePath, "r")
        lines = fileTest.readlines()
        print(lines)
        fileTest.close()
        fileTest = open(conanProfilePath, "w")
        for line in lines:
            if "compiler.cppstd=14" in line:
                line = line.replace("14", "20")
            elif "compiler.cppstd=17" in line:
                line = line.replace("17", "20")
            elif "compiler.cppstd=20" in line:
                break
            fileTest.write(line)
        fileTest.close()
    # os.chdir("..")



    if packageConfigsInstalled[0] == True:
        print("Installing debug packages...")
        subprocess.check_call(['conan', 'install', '.', '--output-folder='+outputFolder, '--build=missing', '-s', 'build_type=Debug'])
        
        # Create a file in the cache folder that indicates that debug packages are installed
        buildTypeIndicator = open("cache/installedDebug.pkgstate", "w")
        buildTypeIndicator.close()

        print("Done installing debug packages.\n")

    if packageConfigsInstalled[1] == True:
        print("Installing release packages...")
        subprocess.check_call(['conan', 'install', '.', '--output-folder='+outputFolder, '--build=missing', '-s', 'build_type=Release'])

        # Create a file in the cache folder that indicates that release packages are installed
        buildTypeIndicator = open("cache/installedRelease.pkgstate", "w")
        buildTypeIndicator.close()

        print("Done installing release packages.\n")


    if packageConfigsInstalled[2] == True:
        print("Installing distribution packages...")
        subprocess.check_call(['conan', 'install', '.', '--output-folder'+outputFolder, '--build=missing', '-s', 'build_type=Distribution'])

        # Create a file in the cache folder that indicates that distribution packages are installed
        buildTypeIndicator = open("cache/installedDistribution.pkgstate", "w")
        buildTypeIndicator.close()

        print("Done installing distribution packages.\n")

    print("Done installing packages.\n")
    return packageConfigsInstalled

print("Configuring Atlas...")

lockConfigFileExists = os.path.exists("cache/lock-configure.lock")

if lockConfigFileExists == False:
    CheckConanStep()
    InstallPackagesStep()
else:
    print("Atlas is already configured. To force reconfiguration, delete the file cache/lock-configure.lock \n")