import javax.swing.*;
import java.awt.*;
import java.awt.event.*;

public class Mouse {
   private Help a1 = null;
   
   public Mouse () { }

	public void xlate (int i) {
		switch (i) {
		case 0: encryption(); break;
		case 1: ident();  break;
		case 2: psswrd();  break;
		case 3: random_player_host_port();  break;
		case 4: war_declare();  break;
		case 5: host_port();  break;
		case 6: war_status();  break;
		case 7: change_password();  break;
		case 8: quit();  break;
		case 9: war_truce_offer();  break;
		case 10: alive();  break;
		case 11: sign_off();  break;
		case 12: delete_public_key();  break;
		case 13: synthesize();  break;
		case 14: public_key_zkp();  break;			
		case 15: make_certificate();  break;
		case 16: public_key_rsa();  break;			
		case 17: get_certificate();  break;
		case 18: get_game_idents();  break;			
		case 19: player_status_crack();  break;
		case 20: player_host_port();  break;
		case 21: player_status();  break;
		case 22: trade_request();	 break;
		case 100: username();  break;
      case 101: DHhalf();  break;
		case 102: passwd(); break;
		case 103: player_host_name(); break;
		case 104: server_port();  break;
		case 105: current_password();  break;
		case 106: authentication();  break;
      case 107: public_key_d();  break;
      case 108: public_key_modulus();  break;			
		case 200: active_player_list();  break;
		}
	}
   
   public void player_host_name () {
		exit();
      a1 = new Help("Server Host Name", false);
      a1.append(
"\n    The domain name or IP address of the machine that hosts"+
"\n    the Passive Server.  This and the space to the right make"+
"\n    up the arguments to the HOST_PORT command.");
      a1.setSize(500,120);
      a1.setVisible(true);
   }

   public void encryption () {
		exit();
      a1 = new Help("Start Encryption", true);
      a1.append(
"\n    Click to start Diffie-Hellman key exchange with Monitor"+
"\n    which results in a secret that is used to encrypt messages"+
"\n    between the Client and the Monitor.  This must be clicked"+
"\n    before starting the Active Client handshake (before IDENT"+
"\n    is executed).");
      a1.setSize(500,150);
      a1.setVisible(true);
   }

   public void ident () {
		exit();
      a1 = new Help("IDENT Command", true);
      a1.append(
"\n    Send a Client's identity to the Monitor.  This is the beginning of the"+
"\n    log in procedure in case the Monitor cannot contact a running Passive"+
"\n    Server.  A Client's identity is not a secret: like a username, it is the"+
"\n    means by which others (including the Monitor) identify the Client.\n"+
"\n    Sends to the Monitor: IDENT ARG1 ARG2\n"+
"\n    where ARG1 is the identity (aka username) and ARG2 is optional. If ARG2 is"+
"\n    sent it should be the Client's Diffie-Hellman half that is needed to do"+
"\n    shared-secret discovery for encryption.  ARG1 is taken from the 'username'"+
"\n    field, ARG2 is computed automatically.  The Monitor returns:\n"+
"\n       RESULT: IDENT ARG1.\n"+
"\n    ARG1 is optional and only appears if cryptography was requested and, if so,"+
"\n    ARG1 will be the Monitor's Diffie-Hellman half that combines with the Client's"+
"\n    to create a secret that is shared between the Client and the Monitor"+
"\n    (See the Cryptography section, Appendix G, of the manual for details).\n");
      a1.setSize(670,390);
      a1.setVisible(true);
   }

   public void psswrd () {
		exit();
      a1 = new Help("PASSWORD Command", true);
      a1.append(
"\n    Send a Client's password to the Monitor.  This is the second command in"+
"\n    the log in procedure in case the Monitor cannot contact a Passive Server.\n"+
"\n    This is only necessary after the Monitor has successfully received an IDENT"+
"\n    command from a Client and has responded with REQUIRE: PASSWORD.\n"+
"\n    Sends to the Monitor: PASSWORD ARG1\n"+
"\n    where ARG1 is the Client's password and is taken from the 'passwd' field."+
"\n    This is a bootstrap for secure communications with the Monitor later"+
"\n    so the Client should use other means to verify that the request came"+
"\n    from the Monitor.  The Monitor responds with:\n"+
"\n       RESULT: PASSWORD ARG1\n"+
"\n    which is sent in the message group following the command invocation."+
"\n    ARG1 is a string with no whitespace that is the Client-specific Monitor"+
"\n    password (sometimes called a 'cookie'). This 'cookie' is different from"+
"\n    the Client's own password and should be used with the ALIVE command"+
"\n    to identify the Client to the Monitor.");
      a1.setSize(660,430);
      a1.setVisible(true);
   }
                        
   public void host_port () {
		exit();
      a1 = new Help("HOST_PORT Command", false);
      a1.append(
"\n    Send to the Monitor the host and port on which the Client's Passive Server"+
"\n    can be reached.  This is the third and final command in the log in procedure"+
"\n    in case the Monitor cannot contact a Passive Server.\n"+
"\n    Sends to the Monitor: HOST_PORT ARG1 ARG2\n"+
"\n    This command is always sent by the Active Client to the Monitor.  ARG1 is the"+
"\n    fully qualified DNS name or IP address of the host where the Passive Server"+
"\n    process is running.  ARG1 is taken from the 'server host location' field. ARG2"+
"\n    is the port number where the Passive Server is listening and is taken from the"+
"\n    'server host port' field.  Port numbers should be between 2000 and 64000.  This"+
"\n    command succeeds only if there is currently a Passive Server process running"+
"\n    on the stated host and port. The Monitor returns:\n"+
"\n      RESULT: HOST_PORT HOST_NAME PORT\n"+
"\n    where HOST_NAME is the fully qualified DNS hostname or IP address that the"+
"\n    Monitor was able to identify as the Passive Server, and PORT is the port where"+
"\n    the Passive Server is listening");
      a1.setSize(680,410);
      a1.setVisible(true);
   }
                        
   public void alive () {
		exit();
      a1 = new Help("ALIVE Command", false);
      a1.append(
"\n    Needed after a successful IDENT command execution by the Active Client during"+
"\n    log in when the Monitor is able to connect with the Client's Passive Server or"+
"\n    after a successful IDENT command execution by the Passive Server, in which"+
"\n    case the Monitor was able to connect to the Passive Server.\n"+ 
"\n    Sends to the Monitor: ALIVE ARG1\n"+
"\n    where ARG1 needs to be the 'cookie' that was given to the Client by the Monitor"+
"\n    as a RESULT: of a previous PASSWORD command.  If ARG1 is not the right 'cookie',"+
"\n    a Command Error will be sent and the Client will not be counted as alive. The"+
"\n    Monitor returns:\n"+
"\n       RESULT: ALIVE Identity has been verified\n"+
"\n    or an error if it is not verified");
      a1.setSize(700,340);
      a1.setVisible(true);
   }
                        
   public void get_game_idents () {
		exit();
      a1 = new Help("GET_GAME_IDENTS Command", false);
      a1.append(
"\n    Get a list of identities of Clients alive in the current contest.  This is useful,"+
"\n    for example, if a Client wants to know who can be traded with.\n"+
"\n    Sends to the Monitor: GET_GAME_IDENTS\n"+
"\n    The Monitor returns:\n"+
"\n        RESULT: GET_GAME_IDENTS ARG1 ... ARGN\n"+
"\n    There are N arguments in the response.  Each argument is the identity of"+
"\n    another Client that is currently alive in the contest.  These N arguments"+
"\n    are placed in a drop down menu below this button.  All commands that"+
"\n    need to identify a contestant use this menu to select that identity.");
      a1.setSize(650,290);
      a1.setVisible(true);
   }
                        
   public void change_password () {
		exit();
      a1 = new Help("CHANGE_PASSWORD Command", false);
      a1.append(
"\n    Request a change in the Client password and the Client-specific 'cookie'.\n"+
"\n    Sends to the Monitor: CHANGE_PASSWORD ARG1 ARG2\n"+
"\n    where ARG1 is the Client's current password, taken from the 'passwd' field."+
"\n    ARG2 is the new password and is generated automatically.  ARG2 replaces"+
"\n    the contents of the 'passwd' field and the argument field next to the"+
"\n    CHANGE_PASSWORD button.  The password is changed only if the Monitor"+
"\n    successfully returns:\n"+
"\n        RESULT: CHANGE_PASSWORD ARG1\n"+
"\n    where ARG1 is a string with no whitespace and is the Client-specific 'cookie'."+
"\n    This new 'cookie' will be different from before.  It must be saved so the"+
"\n    Monitor can verify the Passive Server or Active Client during an ALIVE." );   
      a1.setSize(670,350);
      a1.setVisible(true);
   }
                        
   public void player_status () {
		exit();
      a1 = new Help("PLAYER_STATUS Command", false, "jpg/player-status-help.jpg");
      a1.append(
"\n    Check on the status of the sender's wealth. Click 'Update assets' to"+
"\n    populate the 'have' column.\n"+
"\n    Sends to the Monitor: PLAYER_STATUS\n"+
"\n    The result is\n"+
"\n       RESULT: PLAYER_STATUS STRING\n"+
"\n    which is sent in the message group following the command invocation."+
"\n    The STRING is in the format:\n"+
"\n    ASSET_0 AMOUNT_0 ASSET_1 AMOUNT_1 ... ASSET_9 AMOUNT_9\n"+
"\n    where ASSET_i is the alphanumeric string of one of the 10 assets, and"+
"\n    AMOUNT_i is the amount of that asset the sender currently has.  The"+
"\n    result is displayed in the Client log as well as the above box.");
      a1.setSize(680,680);
      a1.setVisible(true);
   }
                        
   public void status_window () {
		exit();
      a1 = new Help("PLAYER_STATUS Window", false, true);
      a1.append(
"\n    Display the status of a player selected via the menu under the"+
"\n    GET_GAME_IDENTS button on the Command Panel.");
      a1.setSize(550,150);
      a1.setVisible(true);
   }
                        
   public void sign_off () {
		exit();
      a1 = new Help("SIGN_OFF Command", false);
      a1.append(
"\n    Tell the Monitor to 'forget about the Client'.  The Monitor 'forgets about the"+
"\n    Client' in two ways: 1) it forgets what host and port the Client's Passive"+
"\n    Server is on. This means that the HOST_PORT will be REQUIRE: the next"+
"\n    time the Client connects to the Monitor with an Active Client connection;"+
"\n    2) it invalidates the current 'cookie' for the Client.  This means that the"+
"\n    Client will get a PASSWORD command REQUIRE: the next time that the"+
"\n    Client connects to the Monitor with an Active Client connection.  Note that"+
"\n    if a Client executes this command, the Monitor will consider that it is not"+
"\n    alive until a HOST_PORT command is sent.\n"+
"\n    Sends to the Monitor: SIGN_OFF\n"+
"\n    The Monitor returns:\n"+
"\n       RESULT: SIGN_OFF\n"+
"\n    in the message group following the command invocation.");
      a1.setSize(670,380);
      a1.setVisible(true);
   }
                        
   public void quit () {
		exit();
      a1 = new Help("QUIT Command", false);
      a1.append(
"\n    Terminates the current connection.\n"+
"\n    Sends to the Monitor: QUIT\n"+
"\n    The Monitor returns:\n"+
"\n       RESULT: QUIT\n"+
"\n    In addition, note that the message group that follows this command will not"+
"\n    contain the WAITING: directive, as the connection will be closed.");
      a1.setSize(640,240);
      a1.setVisible(true);
   }
                        
   public void make_certificate () {
		exit();
		a1 = new Help("MAKE_CERTIFICATE Command", false);
      a1.append(
"\n    Request to make a certificate for a Client's public key.\n"+
"\n    Sends to the Monitor: MAKE_CERTIFICATE ARG1 ARG2\n"+
"\n    where ARG1 is the Client's pubic key v value (ZKP) or e value (RSA) and ARG2"+
"\n    is the public key n (modulus) value.  The Client makes the public key and"+
"\n    places it in the two fields to the right of the MAKE_CERTIFICATE button.  ARG1"+
"\n    and ARG2 are taken from those two fields.  The Monitor returns:\n"+
"\n       RESULT: CERTIFICATE ARG1 ARG2 ARG3 ARG4\n"+
"\n    where ARG1 is the Client's identity, ARG2 and ARG3 are the Client's public"+
"\n    key, and ARG4 s the certificate signed by the Monitor.\n"+
"\n    A certificate is a SHA-1 digest of the two components of a public key, signed"+
"\n    by the Monitor.");
      a1.setSize(670,350);
      a1.setVisible(true);
   }
                        
   public void player_host_port () {
		exit();
      a1 = new Help("PLAYER_HOST_PORT Command", false);
      a1.append(
"\n    Ask the Monitor for the host port of some given Client's identity. The command"+
"\n    costs one computer, non-refundable.\n"+
"\n    Sends to the Monitor: PLAYER_HOST_PORT ARG1\n"+
"\n    where ARG1 is the identity of the Client for which the host port is requested."+
"\n    The value of ARG1 is obtained from selecting an item in the identities menu"+
"\n    that is immediately below the GET_GAME_IDENTS button.  A Command Error"+
"\n    occurs if the requestor does not have adequate computer resources or if the"+
"\n    identity is not known and/or the requested Client is not currently alive."+
"\n    GET_GAME_IDENTS should be used before invoking PLAYER_HOST_PORT to"+
"\n    populate the identities menu with the identities of currently alive Clients."+
"\n    The Monitor returns:\n"+
"\n       RESULT: PLAYER_HOST_PORT ARG1 ARG2 ARG3\n"+
"\n    where ARG1 isthe identity of the Client, ARG2 is the most recent host on"+
"\n    which that Client has been seen, and ARG3 is the port on which that Client"+
"\n    lives and/or the port where that Client was most recently seen.");
      a1.setSize(670,410);
      a1.setVisible(true);
   }
                        
   public void random_player_host_port () {
		exit();
      a1 = new Help("RANDOM_PLAYER_HOST_PORT Command", false);
      a1.append(
"\n    Ask the Monitor for a host port of some random Client. This is useful when"+
"\n    a Client wishes to go to war with another Client as, in these cases, it is"+
"\n    necessary to know the host and port of the Client to be fought.\n"+
"\n    Sends to the Monitor: RANDOM_PLAYER_HOST_PORT\n"+
"\n    A Command Error occurs if a Client has recently invoked this same command."+
"\n    The Monitor returns:\n"+
"\n       RESULT: RANDOM_PLAYER_HOST_PORT ARG1 ARG2 ARG3\n"+
"\n    where ARG1 is the identity of the Client, ARG2 is the most recent host on"+
"\n    which that Client has been seen, and ARG3 is the port on which that Client"+
"\n    lives and/or the host and port where that Client was most recently seen."+
"\n    The output is displayed in the Client log and an information box.");
      a1.setSize(670,350);
      a1.setVisible(true);
      /*
        System.out.println("RANDOM_PLAYER_HOST_PORT Command.  Format: "+
        "RANDOM PLAYER HOST PORT.  Ask the Monitor for"+
        " a host port of some random identity. This is "+
        "useful when a Client wishes to go to war with"+
        " another Client, as in these cases, it is necessary"+
        " to know the host and port of the Client to be fought."+
        " A COMMAND ERROR occurs if a Client has recently"+
        " invoked this same command.  The result is RESULT"+
        " RANDOM PLAYER HOST PORT ARG1 ARG2 ARG3.   ARG1"+
        " will be the identity of the Client, ARG2 will "+
        "be the most recent host on which that Client"+
        " has been seen, and ARG3 will be the port on which"+
        " that Client lives and/or the host and port where"+
        " that Client was most recently seen.  The output"+
        " is displayed in the Client log");
      */
   }
   
   public void war_declare () {
		exit();
      a1 = new Help("WAR_DECLARE Command", false, "jpg/war-declare-help.jpg");
      a1.append(
"\n    Declare war on another Client.  Click 'Update assets' and 'Update players'"+
"\n    to populate the pull-down menu at the top with Client identities and the"+
"\n    'have' column on left with currently owned assets of the declaring Client."+
"\n    Choose the victim from the pull-down menu.  Choose how many Vehicles"+
"\n    and Weapons are to be committed to the war by repeatedly clicking the"+
"\n    Asset button or by clicking an asset and using the number pad.  The IP"+
"\n    address or URL name of the machine hosting the victim's Passive Server"+
"\n    must be entered in the text field in the upper right section of the window"+
"\n    with the Passive Server's port number.  The port number and IP address"+
"\n    or name must be separated by at least one space.\n"+
"\n    Sends to Monitor: WAR_DECLARE ARG1 ARG2 ARG3 ARG4 ARG5 ARG6 ARG7\n"+
"\n    where ARG1 is the identity of the defender. ARG2 is the host where the"+
"\n    defender lives.  ARG3 is the port on which the defender is listening.  ARG4"+
"\n    is either WEAPONS or VEHICLES. ARG5 is the quantity of assets named in"+
"\n    ARG4 that the aggressor commits to this war, ARG6 is WEAPONS if ARG4 is"+
"\n    VEHICLES or VEHICLES if ARG4 is WEAPONS, and ARG7 is the number of"+
"\n    assets named in ARG6 that the aggressor commits to this war.  If this host"+
"\n    and/or port for the defender is wrong, the aggressor will automatically lose"+
"\n    some percentage of the weapons and vehicles committed, and the war will"+
"\n    not be started (the aggressor must be pretty sure of the whereabouts of"+
"\n    the defender before declaring war).  Finally, the given weapons and vehicles"+
"\n    committed will be tied up in that war until it ends. A Command Error will be"+
"\n    the resulting message group if the defender cannot be found, or if the"+
"\n    aggressor has fewer weapons and/or vehicles than were specified.");
      a1.setSize(670,760);
      a1.setVisible(true);
   }
                        
   public void war_window () {
		exit();
      a1 = new Help("WAR_DECLARE Window", false, true);
      a1.append(
"\n    Declare war on another Client.  Click 'Update assets' and 'Update players'"+
"\n    to populate the pull-down menu at the top with Client identities and the"+
"\n    'have' column on left with currently owned assets of the declaring Client."+
"\n    Choose the victim from the pull-down menu.  Choose how many Vehicles"+
"\n    and Weapons are to be committed to the war by repeatedly clicking the"+
"\n    Asset button or by clicking an asset and using the number pad.  The IP"+
"\n    address or URL name of the machine hosting the victim's Passive Server"+
"\n    must be entered in the text field in the upper right section of the window"+
"\n    with the Passive Server's port number.  The port number and IP address"+
"\n    or name must be separated by at least one space.");
      a1.setSize(690,300);
      a1.setVisible(true);
   }
                        
   public void truce_offer () {
		exit();
      a1 = new Help("WAR_TRUCE_OFFER Window", false, true);
      a1.append(
"\n    Offer a truce to the player that is selected in the pull down menu."+
"\n    A list of assets is displayed on the left.  To the right of each asset"+
"\n    is the current quantity owned by the offerer.  Those quantities are put"+
"\n    there by clicking the 'Update assets' button.  To show the current players"+
"\n    click the 'Update players' button.  Quantities to be offered are entered"+
"\n    either by using the number pad on the right or by repeatedly clicking the"+
"\n    buttons of the assets desired until reaching the desired quantities.  Use"+
"\n    the backspace button on the number pad to erase numbers.");
      a1.setSize(670,250);
      a1.setVisible(true);
   }
                        
   public void truce_offered_window () {
		exit();
      a1 = new Help("WAR_TRUCE_OFFERED Window", false, true);
      a1.append(
"\n    Display an offer that is made by a combatant who wishes to end a war"+
"\n    with a truce.  The quantities of assets offered are displayed to the"+
"\n    left of the assets.  The 'Update assets' button may be clicked to get"+
"\n    current assets held by the Client.  The text field near the top of the"+
"\n    window shows who is offering the truce.  A player responds by accepting"+
"\n    or rejecting the offer.");
      a1.setSize(670,200);
      a1.setVisible(true);
   }
                        
   public void war_truce_offer () {
		exit();
      a1 = new Help("WAR_TRUCE_OFFER Command", false, "jpg/war-truce-help.jpg");
      a1.append(
"\n    Offer a truce to a combatant. Click 'Update players' and 'Update assets' to"+
"\n    populate the 'have' column on the left and combatant menu at the top.  Select"+
"\n    a combatant from the menu and choose quantities of assets that are for offer."+
"\n    Quantities may be chosen by repeated clicking of assets or by clicking an"+
"\n    asset and using the number pad.  Click 'OK' when done.\n"+
"\n    Sends to the Monitor: WAR_TRUCE_OFFER ARG1 to ARG3 ARG4 ARG5 ...\n"+
"\n    where ARG1 is the identity of the offeror, ARG3 is the identity of the opponent,"+
"\n    ARG4 is name of an asset and ARG5 is the quantity of that asset offered, ARG4"+
"\n    and ARG5 pairs are repeated for all assets that are offered.  The Monitor returns:\n"+
"\n       RESULT: WAR_TRUCE_OFFER ARG1\n"+
"\n    where ARG1 is either ACCEPTED, REJECTED, NOT_ALIVE, or WAR_OVER. If ARG1"+
"\n    is ACCEPTED, then the offerer can expect its wealth to be changed to reflect"+
"\n    the truce terms. A Command Error occurs if a war is not active between the"+
"\n    two parties, an invalid asset is named, or if the offeror does not have sufficient"+
"\n    amounts of assets.  Weapons and vehicles already tied up in the war cannot"+
"\n    be used in a war truce agreement.");
      a1.setSize(690,760);
      a1.setVisible(true);
   }
                        
   public void war_status () {
		exit();
      a1 = new Help("WAR_STATUS Command", false, "jpg/war-status-help.jpg");
      a1.append(
"\n    Check the status of a war in progress or a recently fought war.  Click"+
"\n    'Update players' to get a current list of Client identities.  Choose an"+
"\n    identity from the list and click 'OK'.\n"+
"\n    Sends to the Monitor: WAR_STATUS ARG1\n"+
"\n    where ARG1 is the identity of the Client with which the requestor is at"+
"\n    war.  A Command Error is sent in the resulting message group if the"+
"\n    named Client is not at war with the requestor.  The Monitor returns this:\n"+
"\n       RESULT: WAR_STATUS aggressor ARG2 defender ARG4 winner ARG6"+
"\n       battles ARG8 status ARG10 weapons ARG12 vehicles ARG14\n"+
"\n    ARG2, ARG4, ARG6 are the identities of the aggressor, defender, and winner,"+
"\n    respectively.  TRUCE if a truce was negotiated, or NONE if the war continues."+
"\n    ARG8 is the tally of the number of battles fought thus far in this war. ARG10"+
"\n    is COMPLETED if the war is over, or ACTIVE if the war is still active. ARG12 is"+
"\n    the number of weapons that the requestor has committed to this war.  ARG14"+
"\n    is the number of vehicles that the requestor has committed to this war.  The"+
"\n    number of vehicles and weapons committed to wars are not part of the"+
"\n    PLAYER_STATUS total.");
      a1.setSize(670,650);
      a1.setVisible(true);
   }
                        
   public void defend_window () {
		exit();
      a1 = new Help("WAR_DEFEND Window", false, true);
      a1.append(
"\n    Defend against a declaration of war.  The Aggressor text field shows"+
"\n    who is declaring war.  The defender must commit vehicles and weapons"+
"\n    for defense.  The defender may wish to give up instead.  To do so set"+
"\n    quantities of vehicles and weapons to 0.");
      a1.setSize(640,170);
      a1.setVisible(true);
   }
                        
   public void get_certificate () {
		exit();
      a1 = new Help("GET_CERTIFICATE Command", false);
      a1.append(
"\n    Request for the certificate of a Client that is selected in the identities"+
"\n    menu under the GET_GAME_IDENTS button.\n"+
"\n    Sends to the Monitor: GET_CERTIFICATE ARG1\n"+
"\n    where ARG1 is the identity of the Client holding the desired certificate."+
"\n    If ARG1 is 'MONITOR' then the Monitor's certificate is returned.  The result"+
"\n    returned by the Monitor is:\n"+
"\n       RESULT: CERTIFICATE ARG1 ARG2 ARG3 ARG4\n"+
"\n    where ARG1 is the Client identity named in the request, ARG2 and ARG3 are"+
"\n    the named Client's public key, and ARG4 is the certificate signed by the"+
"\n    Monitor.  If ARG1 has not made a certificate the response will be either"+
"\n    'ARG1 has no Certificate registered'"+
"\n    or"+
"\n   'No monitor certificate loaded'.\n"+
"\n   and a red \"Public Key Genuine?\" indicator will flash in the Command Panel"+
"\n   for one second.  If the Monitor returns a valid certificate a green \"Public"+
"\n   Key Genuine?\" indicator will flash for one second.\n"+
"\n    A certificate is a SHA-1 digest of the two components of a public key, signed"+
"\n    by the Monitor.");
      a1.setSize(670,450);
      a1.setVisible(true);
   }
                        
   public void trade_request () {
		exit();
      a1 = new Help("TRADE_REQUEST Command", false, "jpg/trade-request-help.png");
      a1.append(
"\n    Client (called the trade-initiator) requests the Monitor broker and confirm a"+
"\n    trade with another Client or the Monitor (called the subject).  Refresh the 'with'"+
"\n    menu and 'have' column by clicking 'Update assets' and 'Update players'. Then"+
"\n    choose a subject from the menu and establish asset quantities by repeatedly"+
"\n    clicking asset buttons or using the number pad.  Click 'OK' when done.\n"+
"\n    Sends to the Montor: TRADE_REQUEST ARG1 ARG2 ARG3 ... for ARG5 ARG6 ARG7 ...\n"+
"\n    where ARG1 is the identity of the trade-initiator, ARG2 ARG3 ... are the asset"+
"\n    and quantity pairs that the trade-initiator offers to trade.  ARG5 is the identity"+
"\n    of the subject.  ARG6 ARG7 ... are the asset and quantity pairs that the trade-"+
"\n    initiator is asking the subject to trade.  ARG5 may be the Monitor. In that case,"+
"\n    the trade-initiator is requesting a trade with the Monitor itself.  If the Monitor"+
"\n    has the requested asset available in the quantity requested, the Monitor will"+
"\n    approve the trade but only at current market values plus a percentage mark-"+
"\n    up.  The Monitor returns:\n"+
"\n       RESULT: TRADE_REQUEST ARG1\n"+
"\n    where ARG1 is either ACCEPTED, REJECTED, or NOT_ALIVE. All of which refer"+
"\n    to the status of the subject.  If ARG1 is ACCEPTED, then the trade-initiator"+
"\n    can expect its wealth to be changed to reflect the trade.");
      a1.setSize(710,760);
      a1.setVisible(true);
   }
                        
   public void trade_request_window () {
		exit();
      a1 = new Help("TRADE_REQUEST Window", false, true);
      a1.append(
"\n    Client (called the trade-initiator) requests the Monitor broker and confirm"+
"\n    a trade with another Client or the Monitor (called the subject). On the left"+
"\n    is a list of assets with text fields to the left and right of each asset.  On"+
"\n    the right are current asset quantities.  The 'Update assets' button may be"+
"\n    clicked to display current assets if they are not already.  Quantities offered"+
"\n    are entered by the number pad in the center of the window or by repeatedly"+
"\n    clicking the asset's button.  The subject may be selected from the pull down"+
"\n    menu at the top of the window.  If the desired subject does not appear there"+
"\n    the 'Update players' button may be clicked to refresh the list.  On the right"+
"\n    side of the window is a list assets with text fields that indicate what the"+
"\n    initiator wishes to trade for.  Quantities may be entered as in the case of"+
"\n    the left side list of assets.  Use the number pad backspace key to erase"+
"\n    entries");
      a1.setSize(670,340);
      a1.setVisible(true);
   }
                        
   public void DHhalf () {
		exit();
      a1 = new Help("Diffie-Hellman Client Half", false);
      a1.append(
"\n    the Diffie-Hellman half that is used to establish an encrypted connection"+
"\n    with the Monitor.  Diffie-Hellman public key, consisting of generator 'g'"+
"\n    and modulus 'p', is given to all players.  A player chooses a random"+
"\n    number 'r' and computes 'g' raised to power 'r' modulo 'p' as its DH half"+
"\n    The monitor does the same.  Two two halves are combined to get the"+
"\n    shared secret.  This is computed only when encryption is turned on.");
      a1.setSize(660,180);
      a1.setVisible(true);
   }
                        
   public void synthesize () {
		exit();
      a1 = new Help("SYNTHESIZE Command", false, "jpg/synthesize-help.jpg");
      a1.append(
"\n    Synthesize a complex asset.  The complex assets are WEAPONS, COMPUTERS,"+
"\n    and VEHICLES.  Click 'Update assets' to populate the 'have' column with"+
"\n    owned assets.  Repeatedly click on a complex asset to choose the quantity"+
"\n    of that asset to synthesize.  There must be sufficient quntities of raw"+
"\n    assets for this operation to succeed.  Click 'OK' to send the command.\n"+
"\n    Sends to the Monitor: SYNTHESIZE ARG1 ARG2\n"+
"\n    where ARG1 is be the name of the asset to synthesize and ARG2 is the"+
"\n    quantity desired to be synthesized.  See Table 2 in the Economy section"+
"\n    of the manual for more information about what raw assets are needed to"+
"\n    synthesize the complex assets. The Monitor returns\n"+
"\n       RESULT: SYNTHESIZE STRING\n"+
"\n    in the message group following the command invocation. STRING shows the"+
"\n    revised asset holdings. A Command Error is sent if the asset could not be"+
"\n    synthesized for any reason.");
      a1.setSize(680,700);
      a1.setVisible(true);
   }
                        
   public void synthesize_parms () {
		exit();
      a1 = new Help("SYNTHESIZE Window", false, true);
      a1.append(
"\n    Click repeatedly on the Vehicles, Weapons, or Computers button to"+
"\n    set the quantity of synthesized assets desired.  Click the 'Update"+
"\n    assets' button to see what quantities of assets are owned by the"+
"\n    Client.");
      a1.setSize(630,200);
      a1.setVisible(true);
   }
                        
   public void delete_public_key () {
		exit();
      a1 = new Help("Delete Public Key", false);
      a1.append(
"\n    Deletes the file holding the public key and nulls out the text fields"+
"\n    showing the public key that are displayed on the command panel.");
      a1.setSize(570,100);
      a1.setVisible(true);
   }
                        
   public void public_key_rsa () {
		exit();
      a1 = new Help("Public Key RSA", false);
      a1.append(
"\n    Creates a RSA public key and records it in a file displays it on"+
"\n    the command panel.");
      a1.setSize(570,100);
      a1.setVisible(true);
   }
                        
   public void public_key_zkp () {
		exit();
      a1 = new Help("Public Key ZKP", false);
      a1.append(
"\n    Creates a ZKP public key and records it in a file displays it on"+
"\n    the command panel.");
      a1.setSize(570,100);
      a1.setVisible(true);
   }
                        
   public void player_status_crack () {
		exit();
      a1 = new Help("PLAYER_STATUS_CRACK Command", false, "jpg/status-crack-help.jpg");
      a1.append(
"\n    Check the status of a named Client's wealth. Computers are needed for the"+
"\n    crack.  Click 'Update assets' to put the number of computers owned in the"+
"\n    'have' column.  Click 'Update players' to populate the menu at the top with"+
"\n    Client identities.  Choose the number of computers to commit by repeatedly"+
"\n    clicking the 'Computers' button.  Click 'Undo Computers' to reset.  Click 'OK'"+
"\n    when done.  The more computers the greater the chance of success.  The"+
"\n    computers committed are taken by the Monitor no matter what.\n"+
"\n    Sends to the Monitor: PLAYER_STATUS_CRACK NAME COMPUTERS\n"+
"\n    where NAME is the identity of the Client whose wealth information is desired and"+
"\n    COMPUTERS is the number of computers committed.  On success the result is:\n"+
"\n       RESULT: PLAYER_STATUS NAME STRING\n"+
"\n    which is sent in the message group following the command invocation.  On"+
"\n    failure a message indicating such is sent instead.  STRING is in the format:\n"+
"\n    ASSET_0 AMOUNT_0 ASSET_1 AMOUNT_1 ... ASSET_9 AMOUNT_9\n"+
"\n    where ASSET_i is the name of one of the 10 assets, and  AMOUNT_i is the amount"+
"\n    of that asset the sender currently has.  The result is displayed in the Client"+
"\n    log and a box similar to that shown in the help window of PLAYER_STATUS.");
      a1.setSize(700,768);
      a1.setVisible(true);
   }
                        
   public void player_status_crack_window () {
		exit();
      a1 = new Help("PLAYER_STATUS_CRACK Window", false, true);
      a1.append(
"\n    Choose a target to crack from the pull down menu.  If the desired"+
"\n    target is not listed click 'Update players' and try again.  Click"+
"\n    'Update assets' to display the number of computers owned by the "+
"\n    Client to the right of the Computers button.  Repeatedly click the"+
"\n    Computers button to set the desired number of computers to"+
"\n    commit to the crack. Click 'Undo Computers' to reset the number"+
"\n    of computers to commit to 0.");
      a1.setSize(600,250);
      a1.setVisible(true);
   }
                        
   public void username () {
		exit();
      a1 = new Help("Username", false);
      a1.append("\n    The Client's identity is displayed here.  This field is"+
                "\n    not editable");
      a1.setSize(470,90);
      a1.setVisible(true);
   }
   
   public void passwd () {
		exit();
      a1 = new Help("Password", false);
      a1.append("\n    The current password is displayed here.  This field is"+
                "\n    not editable - passwords are set by a random process");
      a1.setSize(490,90);
      a1.setVisible(true);
   }
   
   public void server_ip () {
		exit();
      a1 = new Help("Server Host IP", false);
      a1.append("\n    The IP address or domain name of the machine on which"+
                "\n    the Client's Passive Server is running is displayed here");
      a1.setSize(500,90);
      a1.setVisible(true);
   }
   
   public void server_port () {
		exit();
      a1 = new Help("Server Host Port", false);
      a1.append("\n    The port on which the Client's Passive Server is listening"+
                "\n    is displayed here");
      a1.setSize(500,90);
      a1.setVisible(true);
   }
   
   public void current_password () {
		exit();
      a1 = new Help("Current Password", false);
      a1.append("\n    When a password is changed it is displayed here."+
                "\n    Clicking CHANGE_PASSWORD creates a random"+
                "\n    password that is shown here.");
      a1.setSize(420,100);
      a1.setVisible(true);
   }
   
   public void public_key_d () {
		exit();
      a1 = new Help("Public Key D", false);
      a1.append(
"\n    The Client's public key RSA exponent d or ZKP v.  This is needed to make"+
"\n    a certificate.  Clicking 'MAKE_CERTIFICATE' starts a process that computes"+
"\n    a new key pair.  The result of that process is two public key components -"+
"\n    one component is placed here and the other next to this one.  Then the"+
"\n    MAKE_CERTIFICATE command is executed with arguments that are taken"+
"\n    from these two fields.");
      a1.setSize(650,160);
      a1.setVisible(true);
   }
                        
   public void public_key_modulus () {
		exit();
      a1 = new Help("Public Key Modulus", false);
      a1.append(
"\n    The Client's public key modulus n.  This is needed to make a certificate."+
"\n    Clicking 'MAKE_CERTIFICATE' starts a process that computes a new key pair."+
"\n    The result of that process is two public key components - one component is"+
"\n    placed here and the other next to this one.  Then the MAKE_CERTIFICATE"+
"\n    command is executed with arguments that are taken from these two fields.");
      a1.setSize(670,150);
      a1.setVisible(true);
   }
                        
   public void active_player_list () {
		exit();
      a1 = new Help("List of Players Active", false);
      a1.append(
"\n    The list of current contestants.  Populated by clicking GET_GAME_IDENTS."+
"\n    Several commands such as GET_CERTIFICATE, PLAYER_HOST_PORT, and"+
"\n    others use identities that are selected from this list.");
      a1.setSize(640,120);
      a1.setVisible(true);
   }
                        
   public void authentication () {
		exit();
      a1 = new Help("Authentication", false);
      a1.append(
"\n    When a competitor's public key is delivered to a Client it is accompanied by"+
"\n    a certificate of that public key which is signed by the Monitor.  The Monitor's"+
"\n    public key is given to all Clients at the start of the competition via file 'Parms'."+
"\n    The Client has a process that authenticates the received public key using the"+
"\n    certificate and Monitor public key.  If there is a match between the digest"+
"\n    of the received public key and the decrypted certificate then this field is"+
"\n    colored green.  If there is no match this field is colored red.  Otherwise, say"+
"\n    the 'Parms' file is not found, this field is colored yellow");
      a1.setSize(670,200);
      a1.setVisible(true);
   }

	public void server_connect () {
		exit();
		a1 = new Help("Connect Passive Server", true);
		a1.append("\n    Start the Passive Server on host named in 'server host"+
					 "\n    location' field, listening on port named in 'server host"+
					 "\n    port' field.  Normally, this is started before the Active"+
					 "\n    Client is started.");
		a1.setSize(470,120);
		a1.setVisible(true);
	}

	public void client_connect () {
		exit();
		a1 = new Help("Connect Active Client", true);
		a1.append("\n    Start the Active Client.  The Active Client will connect"+
					 "\n    to the Monitor on the port named in the 'monitor port'"+
					 "\n    field.  This is normally started after the Passive Server.");
		a1.setSize(470,120);
		a1.setVisible(true);
	}

	public void server_disconnect () {
		exit();
		a1 = new Help("Disconnect Passive Server", true);
		a1.append("\n   Stop the Passive Server.  The Passive Server may"+
					 "\n   resume by clicking the 'Connect' button: then it will"+
					 "\n   run on the machine and port named in the two server"+
					 "\n   fields.");
		a1.setSize(470,120);
		a1.setVisible(true);
	}

	public void client_disconnect () {
		exit();
		a1 = new Help("Disconnect Active Client", true);
		a1.append("\n   Stop the Active Client.  The Active Client may resume"+
					 "\n   by clicking the 'Connect' button: then it will attempt"+
					 "\n   to connect to the Monitor on the named monitor fields.");
		a1.setSize(480,110);
		a1.setVisible(true);
	}
		
   public void password () {
		exit();
		a1 = new Help("Password Field", true);
		a1.append("\n   A password may be entered here.  During a competition"+
					 "\n   passwords are provided by the Contest Administrator in"+
					 "\n   a file called 'Parms' and the field will then not be"+
					 " editable.");
		a1.setSize(510,120);
		a1.setVisible(true);
	}

	public void identity () {
		exit();
		a1 = new Help("Client Identity Field", true);
		a1.append("\n   The identity of the Client is here.  This is an edit-"+
					 "\n   able field for local practice but during a contest the"+
					 "\n   Contest Administrator will send this information in a"+
					 "\n   file called 'Parms' and the field will then not be"+
					 " editable.");
		a1.setSize(490,130);
		a1.setVisible(true);
	}

	public void monitorPort () {
		exit();
		a1 = new Help("Monitor Port Field", false);
		a1.append("\n   The port on which the monitor is listening is here"+
					 "\n   This is editable for local practice but during a contest"+
					 "\n   the Contest Administrator will send this information"+
					 "\n   in a file called 'Parms' and then the field will not be"+
					 "\n   editable.");
		a1.setSize(470,160);
		a1.setVisible(true);
	}

	public void serverPort () {
		exit();
		a1 = new Help("Server Port Field", false);
		a1.append("\n   The port on which the Client's Passive Server is"+
					 "\n   listening.  This is editable for local practice but"+
					 "\n   during a contest the Administrator may choose"+
					 "\n   the port and send it via the 'Parms' file in which"+
					 "\n   case it will not be editable.");
		a1.setSize(420,150);
		a1.setVisible(true);
	}		

	public void monitorLocation () {
		exit();
		a1 = new Help("Monitor Location Field", false);
		a1.append("\n   The IP address or the domain name of the machine"+
					 "\n   on which the monitor is running.  During a contest"+
					 "\n   the Contest Administrator sends this to the Client"+
					 "\n   via the file 'Parms' and then the field is not editable.");
		a1.setSize(460,130);
		a1.setVisible(true);
	}

	public void serverLocation () {
		exit();
		a1 = new Help("Server Location Field", false);
		a1.append("\n   The IP address or the domain name of the machine"+
					 "\n   on which the monitor is running.  This is provided"+
					 "\n   by the Contest Administrator via file 'Parms' in which"+
					 "\n   case the field is not editable.  For local practice"+
					 "\n   the field is editable.");
		a1.setSize(450,150);
		a1.setVisible(true);
	}
	
   public void exit () {
      if (a1 != null) a1.dispose();
      a1 = null;
   }
}
