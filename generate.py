import os
import subprocess
import sys
def DoesBuildFolderExist():
    return os.path.exists("build")

def RemoveBuildFolder():
    os.rmdir("build")


def GenerateProjectStep():
    print("Generating project...")

    if DoesBuildFolderExist() == False:
        os.mkdir("build")
    os.chdir("build")
    subprocess.check_call(['cmake', '..', '-G', 'Visual Studio 17 2022', '-DCMAKE_TOOLCHAIN_FILE=build\conan_toolchain.cmake'])
    print("Done generating project.\n")
    

def BuildProject(type):
    print("Building project of type " + type + "...")
    subprocess.check_call(['cmake', '--build', '.', '--config=' + type])
    print("Done building project.\n")

lockConfigFileExists = os.path.exists("cache/lock-configure.lock")

if lockConfigFileExists == False:
    #os.chdir("..")

    GenerateProjectStep()

    print("Building project...")

    # Packages are installed if the respective file (installedDebug.txt for debug, installedRelease.txt for release, installedDistribution.txt for distribution) is in the cache folder
    debugPackagesInstalled = os.path.exists("..\\cache\\installedDebug.pkgstate")
    print("Debug packages installed: " + str(debugPackagesInstalled))

    releasePackagesInstalled = os.path.exists("..\\cache\\installedRelease.pkgstate")
    print("Release packages installed: " + str(releasePackagesInstalled))

    distributionPackagesInstalled = os.path.exists("..\\cache\\installedDistribution.pkgstate")
    print("Distribution packages installed: " + str(distributionPackagesInstalled))

    installedDebug = debugPackagesInstalled
    installedRelease = releasePackagesInstalled
    installedDistribution = distributionPackagesInstalled

    if installedDebug == True:
        print("Building debug packages and project...")
        BuildProject("Debug")
        print("Done building debug packages and project.\n")

    if installedRelease == True:
        print("Building release packages and project...")
        BuildProject("Release")
        print("Done building release packages and project.\n")

    if installedDistribution == True:
        print("Building distribution packages and project...")
        BuildProject("Distribution")
        print("Done building distribution packages and project.\n")

    print("Done building project.\n")

    print("Atlas is now configured and built. Go to the build folder, and click on the Techstorm.sln solution to begin working. Happy coding, you magnificent developer! :)")

else:
    print("Atlas is already configured. To force reconfiguration, delete the file cache/lock-configure.lock \n")