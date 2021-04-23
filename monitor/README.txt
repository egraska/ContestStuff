For the monitor to compile you may have to install libssl-dev

Each team client and the monitor gets a copy of client.conf, a copy of
ca.crt, a copy of run.client and a copy of stop.client, plus one each of
clientX.key, and clientX.crt.  There is also an email.txt file which is
used by the Contest Administrator to distribute these files.  Finally, a
Parms file is also provided.  

Files ca.crt, clientX.key, clientX.crt are in a subdirectory named 'keys'.
Files client.conf, run.client, stop.client, run.player, stop.player,
email.txt, and this README.txt and the keys subdirectory are in a
directory named <player-name> (ex: "Peter_Smith") where <player-name>
is taken from the player property list which is developed .

Some modifications to these files may be necessary as stated below.

Three things to (maybe) change in client.conf
---------------------------------------------
1. currently: remote <some-domain-name> 1194  may need to be changed to

         remote <ip-address-of-server> 1194

     that is, the ip address of the host where 'sudo openvpn server.conf'
     is run.  If the Contest Administrator correctly entered this address
     in the configuration tool the correct <ip-address-of-server> is
     already in client.conf.  The address of the host behind a firewall
     must be used if the host address inside an organization's perimeter
     is different from the host address as seen from outside the
     perimeter.

2. currently: ;socks-proxy 127.0.0.1 8080 may need to be changed to
     socks-proxy 127.0.0.1 8080 if a socks proxy is used.  If this is
     done make sure the line
     
         remote <ip-address-of-openvpn-server> 1194

     is used.  Making this change requires a change in run.client as
     below.  A socks proxy is likely needed when a Client tries to
     join a contest VPN from outside an organization's security
     perimeter while the OpenVPN server is inside the perimeter.

3. currently: there is a section that looks like this:

     ca keys/ca.crt
     cert keys/clientX.crt
     key keys/clientX.key

   where X is a number from 0 to 50.  Check that clientX.crt and
   clientX.key match the names in the keys subdirectory.  If not,
   the Contest Administrator needs to be contacted.

Two things to consider in run.client
------------------------------------
1. The line

     ssh -N -f -T -D 8080 visitor@helios.ececs.uc.edu

   makes a socks proxy connection to be used from outside the UC
   perimeter.  This line is likely commented (it is assumed no proxy
   is needed).  If a socks proxy is needed and is not forbidden then
   just uncomment that line by removing #.  Also uncomment the
   following 'sleep 1'.  If a change is made here then a corresponding
   change in run.client, as noted above, must be made.  You will
   have to find a socks proxy to allow entry to your organization's
   server and replace visitor@helios.ececs.uc.edu with that.

2. A running openvpn process may interfere with the openvpn process that
   run.client will spawn.  If that happens, and it is not detrimental to do
   so, uncomment the 'sudo killall openvpn' line and the following 'sleep 1'
   line to kill those running openvpn processes before running your openvpn
   process.

About the 'Parms' file
----------------------
The Parms file contains all the information that is needed by a Client to
access a contest.  This includes contest start time and end time in Unix
seconds, the Client's username and password, the ip address (vpn) and port
the Client's Passive Server will listen on, the ip address (vpn) and port
the Monitor will listen on, the Diffie-Hellman public p and g parameters,
and the Monitor's public RSA key.  If you are using the ready-made Client
the Parms file should be placed in the top level of the Client's directory.
Otherwise, you must use the information in the Parms file to set up your
own Client.

To Log into a contest
---------------------
This assumes the host on which the Client is to be run is connected
to a network that includes the monitor.  Check the manual and
directory server for how to do this with OpenVPN.

The contest monitor should be started by the Contest Administrator.
The Contest Administrator should have sent the monitor's location 
and port in a file named 'Parms'.   In particular, the monitor
information shows up in 'Parms' like this:

   # Monitor Location
   10.8.0.99
   # Monitor Port
   8180

Other information needed to log in. that is supplied by the 'Parms'
file is username and password which looks like this:

   # Username
   Peter_Piper
   # Password
   natural_selection

and the IP address of the Client's Passive Server and the port it is
listening to which looks like this:

   # Server Port
   20000
   # Server Location
   10.8.0.50

If using the ready-made Client (client.jar file in this package) the
Client will expect to have 'Parms' in the same directory as client.jar
and will read these necessary parameters when the Client is launched.
Otherwise, it is up to the Player to add functions that read the Parms
file and use the information in it (includes username, password ...)

To launch the ready-made Client either use 'run.player' in this
directory or run 'java -jar client.jar' or click on the client.jar
icon in a file browser.

In the window that opens, click on both 'Connect' buttons in the upper
left part of the window to start the Passive Server and Active Client
connections.  Their indicators will turn green, having been read.
Choose 'Start Encryption' to use encryption transmission of messages.
Then click the 'IDENT' button, then the 'PASSWORD' button then the
'HOST_PORT' button.  The Client will be logged in or some error message
will indicate a problem.  See the "Join the competition" section of
the manual for details and graphics.
