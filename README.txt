**Instructions for Installing my S3FS***

1.install libfuse if not installed already: https://github.com/libfuse/libfuse

2.If you are on a linux machine, OpenSSL is already installed.
    However if you run '$openssl version' and see 1.0.2g,, download and install
    version 1.1.0g from here: https://www.openssl.org/source/

3. Download and install s3fs with these commands:

$ git clone https://github.com/s3fs-fuse/s3fs-fuse.git

$ cd s3fs-fuse

$ ./autogen.sh

$ ./configure

$ make

Now take the s3fs.cpp & fdcache.cpp files and Makefile I provided in /src/s3fs/and replace them with the ones in s3fs-fuse/src/. 

**If the makefile is not linking with your version of OpenSSL, you can link to it by adding -L <openssl directory> where g++ is called in the makefile.

run $ make in sfs-fuse/src

Option 1 (if you do not want to install system wide):

    open ~/.bashrc in a text editor.
    
    add "export PATH= ~/s3fs-fuse/src/:$PATH
    
    now you can run my executable from anywhere in the system without './'

Option 2:

$ make install

4. Create an AWS account and an s3 bucket here:

https://console.aws.amazon.com/console/home?region=us-east-1

5. Make and mount a directory with these commands:

$mkdir ~/s3-drive

$s3fs <bucket-name> ~/s3-drive

Now you can move files in and out of ~/s3-drive and they will be encrypted in the browser.

***Stand Alone RC4***

This package is for Stand-Alone encryption

1.Go into the rc4 folder and run $ make

2.now you can run ./rc4_cal <fileinput> <key> <fileoutput> <-e/-d> <-salt/-nosalt>

EXAMPLE: ./rc4_cal Land_Seal.jpg pass hidden.jpg -e -salt
