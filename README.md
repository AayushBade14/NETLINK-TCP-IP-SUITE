# ------------------------------------------------------------------(NETLINK TCP SUITE)------------------------------------
## CREATOR:-
## Name: Aayush Raju Bade
# -------------------------------------------------------------------------------------------------------------------------

## [+] {OVERVIEW}:- This project implements a TCP/IP server-client model with features like:-
			
			(i) Chatting (Multi-Threading Not Implemented)
		      	(ii) Remote Access The Server Through Client (This Can Be Used To Implement Proxy-Chains For Pen-Testing)
		      	(iii) Rock-Paper-Scissors Game
			(iv) N-Map scans (Can Be Used To Gather Info On Target Machine Before Pen-Testing)

# --------------------------------------------------------------------------------------------------------------------------
## [+] {SETUP}:- 

NOTE:-     1) The Server And Client Code Must Be Executed On Computers Under Same Network

	   2) This Program Is Strictly Made For UNIX SYSTEMS (TO IMPLEMENT ON WINDOWS -- Refer FIX Section)
    
	   3) Make Sure gcc Compiler Is Installed (It's Pre-Installed on Linux Systems)
    
	   4) For Better N-map Scans Get Root Privileges On The Terminal: "sudo su" and enter your password, To Gain Root Privileges

(I) Copy The Server.c File To A Separate Folder On A System Which You Want To Be The Server And Client.c File To A separate Folder On A System Which You Want To Be The Client. This Can Also Be Done On A Single System, But The Execution Should Be Done In Two Separate Terminals.

(II) COMPILE THE Server.c CODE: "gcc Server.c -o server"

(III) COMPILE THE Client.c CODE: "gcc Client.c -o client"

(IV) NOW TO RUN THE SERVER CODE: "./server [PORT NO.]" , Enter The Desired Port No. And Make Sure It's Valid

(V) NOW TO RUN THE CLIENT CODE: "./client [SERVER IP] [SERVER PORT NO.]", Enter Server IP And Port No. (Make Sure It's Valid). 

    [#] If Running Both Server And Client On The Same System Enter LOOPBACK-ADDRESS (i.e 127.0.0.1): "./client 127.0.0.1 [SERVER PORT NO.]"

------------------------------------------------------------------------------------------------------------------------------

## [+] {FIX}:-

To Run On A Windows System , Install Cygwin-64. It Provides UNIX-Like Terminal Environment And Then Follow From The SETUP-SECTION

Installation Link:- https://www.cygwin.com/install.html

-------------------------------------------------------------------------------------------------------------------------------
