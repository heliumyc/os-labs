# scheduler

**NOTE: MUST USE GCC-9.1 OR HIGHER**

if run on the school server, please do
```$xslt
module unload
module load gcc-9.1
```
and make sure the version is correct

Although I specified this in Makefile, but it seems it dynamically 
links to some lib when running rather than compiling. I think it might has
something to do with getopt function. If gcc is of default version (4.8), 
on the school server, it always raised xxxx.so xxxx.lib not found.