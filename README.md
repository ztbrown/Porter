![status badge](https://travis-ci.org/ztbrown/GoHttp.svg?branch=master)

Porter
======
Porter is a simple web server written in C for educational purposes. This web server runs on GNU/Linux.

## What is it?
This web server is far from complete. The plan is to modularize [GoHttp](https://github.com/fekberg/GoHttp), write tests, add features and eventually produce a small library that can be included in future projects. 

## What are its capabilities?
It currently supports GET so you can use it to receive any files that correspond with the mime types in mime.types. 

## Prerequisites
- gcc
- Make
- c99
- Docker

## How do I run it?

1. Download the source
2. Compile the source using `make` 
3. Run from root directory with `./main`
4. Run the tests with `make check`

## Command line arguments
You can start the web server with the following command line arguments:

	-p port number
	-d run as daemon
	-l log file

## What about configuration?
You can open httpd.conf and change the following:

	wwwroot /home/frw/public_html/
	port 7000

# Credit
This project was originally forked from [GoHttp](https://github.com/fekberg/GoHttp), a really great resource. 

From fekberg: "If it weren't for the course in Advance UNIX Programming that I took at Blekinge Institute of Technology I would never have written this. It all originated from a question on [StackOverflow](http://stackoverflow.com/questions/409087/creating-a-web-server-in-pure-c) from 2009 where I asked for information on how to write a simple web server in C."
