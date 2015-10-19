*************************************
****            nytwd            ****
**** Not Your Typical Web Daemon ****
*************************************

Code written by Dustin LaMiaux - SilenceOfTheLam

This is a simple C implementation of a web server daemon that is not very configurable nor informative. Currently the server can accept connections on a socket, receive bytes from the stream, print what the client said, and close the connection. This project is for my own personal usage to learn more about the HTTP protocol.

Most of this code is adapted from Beej's Guide to network programming and Zed Shaw's learn C the hard way (espiecially his debug macros in dbg.h). 
