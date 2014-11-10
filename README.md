PiFi
====

This is an open source method to get your Raspberry Pi running Raspbian connected to your home WiFi network.  

Some requirements:
- There is an updated Raspbian pi image that you will need to use in order for this to work successfully.  This image sets up the
Pi configurations so that it immediately starts up running it's own WiFi AP and running a DHCP server so your client can join.
- A raspberry pi with a WiFi USB device connected
- A client that can execute http GET and POST requests to http://192.168.1.1
- If you would like to test then you will need to build and run the daemon by building the daemon.c file
    gcc -Wall daemon.c -o daemon
- The daemon process will be running on the raspberry pi image provided so unless you want to edit that then there is no need
build or update this.

Design:
- The overall theme in this design is for the raspberry pi to initially host it's own WiFi AP and web server so you can create
client devices running http as a client to switch the pi to WiFi client mode and pass the WiFi credentials of the WiFi network
you would like the pi to join.
- There are two main resources hosted on the raspberry pi's web server
    - http://192.168.1.1/networks
    - http://192.168.1.1/token
- There is a process that is running all of the time on the raspberry pi called "daemon" so DO NOT KILL this process.  This 
process is reading a file called "hellothere.txt" which is updated when a client POSTs information to the raspberry pi web 
server.  When there is a state occurred in this file the daemon process then configures the pi with the proper configurations to
either start the AP + web server or join the appropriate WiFi AP as a client.

Web Resources:
- JSON schema
{
    "state": "AP",
    "token": "null",
    "ssid": "null",
    "pass_phrase": "null",
    "security_mode": "null"
}
"state" can be the following strings : "AP" or "client"
"token" can be whatever string you wish to use for some kind of user authentication token
"ssid" is a string of the ssid you wish the pi to join
"pass_phrase" is a string of the pass phrase of the wifi network you wish to join
"security_mode" can be : "wpa2", "wpa", "disabled"

- http://192.168.1.1/networks
  - GET : when you perform an http GET operation at this resource you will receive a list of surrounding networks and the security
  mode of each network.  This can help your client so that they do not need to manually enter the ssid they wish to join.
  - POST : when you POST to this resource you must have the JSON schema listed above in the body of the POST operation.  The i
  information listed in the body will be used for the the pi to take action, if state is "client" it will take the relative
  information and attempt to join that wifi network.
  
- http://192.168.1.1/token
  - GET : TBD
  - POST : when you POST to this resource you must have the JSON schema listed above in the body of the POST operation.  The only
  data that will be updated is the "token" field of the JSON data.
  - This resource simply updates the token in the file "hellothere.txt" and the user can use this token for whatever they like.
  
  
  Work To Be Done:
  - Right now the daemon doesn't use a "real" JSON parser so the form of the JSON data isn't very flexible.  In the future we should
  use a "real" JSON parser so that data is more flexible.
  
  
