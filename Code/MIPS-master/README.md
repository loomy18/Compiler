###~ Hierarchy ~

A unix development solution is wrapped with a visual studio solution. This encourages the power of VS while maintaining portability with unix sessions.

###~ Dependencies ~

**<  Windows  >**
* Cygwin
  * Packages :
```
ncurses (all)
make (all)
gcc (all)
flex (all)
bison (all)
```
* PATH += "C:\cygwin64\bin\;"

**<  Unix  >**
```
sudo apt-get update
sudo apt-get upgrade
sudo apt-get install flex bison
```
  
###~ Automation ~

Cygwin/Unix Shell :
```
cd /cygdrive/c
cd /*path-to-run-script/*
./unix_run
```
