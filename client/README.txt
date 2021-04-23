File client.jar is the ready-made Client.  It needs a 'Parms' file in
the same directory as 'client.jar' if it is to run in a contest.  One
such file is included in the 'misc' directory as an example.  The
Parms file normally is made by the configurator in directory ../config.
If 'run.player' is run without the 'Parms' file the Client will open
and editable fields will allow you to specify username, password,
monitor host IP address, monitor listening port, Client's passive
server IP address, passive server's listening port.  See directory
'../config' for the configuration tool that creates 'Parms' files.

Source code for the Client is in directory src.  Type 'make' then hit
return to compile the code into client.jar.  The jar file is moved
to this directory when compilation is complete.  Copy 'client.jar' to
the player directories that are created by the configurator.  The
../config directory has a sample players.txt file in place for the
benefit of testing.  That file lists players in directories 'player1'
and 'player2'.  Those directories eventually are tarred, transported,
then untarred.  They contain everything except a working client: they
have a Parms file for a contest and perhaps keys for connecting to
an OpenVPN network.  To make the player directories ready for the
contest client.jar needs to be copied to the player1 and player2 
directories, wherever they are.
