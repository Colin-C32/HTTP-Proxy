CPSC441 Assignment 1:

To run the code you must follow these steps:
1) Type 'ifconfig' into the cmd line and look for ensd192 or (eno1) and copy the inet ip number
    should be something similar to 136.159.5.27 if this is run on the linux.cpsc.ucalgary.ca server
2)Take the ip we obtained in step one and define it as PORT at the top of the trial.cpp file (can just replace other ip if they are different)
3)go to your proxy settings and manually set up http proxy with this same ID and use port # 1247
4)Run g++ trial.cpp in cmd line
5)Run ./a.out if you wish to block anything you can add keywords like such
-> to block Floppy run ./a.out Floppy
-> to block Floppy and curling ./a.out Flopy curling