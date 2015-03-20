# Pac
Simple Linux File Encryption
![encryption](http://i.imgur.com/zXr91qt.png)
---------------------------------------

#### Usage
```
clang++ pac.cpp -o pac (or gcc)
./pac [file] [optional 4 byte key] 
```
----

#### What is this
This is a command line utility to encrypt single files with the flick of the wrist. It uses Extended Tiny Encrytion Algorithym because XTEA is at the intersection of laziness and security. I figure that it is very simple and in addition to being an actual tool for Admins and Hackers alike, it is also a good reference. 

---

#### Details
I programmed this instead of doing an essay in Mr. Nelsons Government Class. While this project was initially limited to one classroom I have been able to succesfully compile this program in classes other than Government class. On a more serious note, I built this on an arch linux machine with clang. Windows and Mac support is easy to implement, but I am lazy.

---

####Security
*Is this a sound XTEA implementation*
Yes. 
Source: https://en.wikipedia.org/wiki/XTEA

*Is this secure?*                             
Yes.

*Mom is spying on me. will this protect me?*
Yes.

*Big Brother is spying on me. will this protect me?*
No.

*Should I use it in production?*
No. It hasnt been tested in lots of situations and it only utilizes a four byte encryption key.

 

