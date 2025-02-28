import os
import subprocess

def RunWindows():
    subprocess.call(['build-windows.bat'])

def RunLinux():
    subprocess.call(['chmod', '+x', 'build-linux.sh'])
    subprocess.call('./build-linux.sh') 

if __name__ == "__main__":
    # If windows, then call build-windows.bat. Otherwise, call build-linux.sh
    if (os.name == "nt"):
        RunWindows()
        
    else:
        RunLinux()