import java.io.*;
import java.math.*;
import java.util.*;
import java.security.*;

/**
 * Manages passwords, encryption, and other aspects of security for
 * the IWar player program.
 *
 *@author Greg Larson
 *@version 030104
 *
 * + modified heavily by John Franco, May-July, 2019, all file read/write files
 * + are new.
 */
public class Pitbull {
   private static String password;
   static String newPassword;
   private static String passwordFilePath;
   private static String aliveCookieFilePath;
   private static String checksumFilePath;
   static RSA UserRSAKey = null;
   static ZKP UserZKPKey = null;	
   private static Project iwar = null;

	public static void setProject(Project iwarObject) {
		iwar = iwarObject;
	}

   public static void setFiles(String passwordFile, String aliveCookieFile,
                               String checksumFile) {
      passwordFilePath = passwordFile;
      aliveCookieFilePath = aliveCookieFile;
      checksumFilePath = checksumFile;
   } // end of setFiles

	public static PubRSA createPubMonitorRSA () {
		String enc = null, mod = null, str = null;
		FileInputStream fis;
		try {
         fis = new FileInputStream("Parms");
         BufferedReader br = new BufferedReader(new InputStreamReader(fis));
         while ((str = br.readLine()) != null) {
            if (str.equals("# Monitor Public Key exponent (e)"))
					enc = br.readLine();
         }
         fis.close();
		} catch (Exception e) {
			System.out.println("Parms file not found");
			return null;
		}
		try {
         fis = new FileInputStream("Parms");
         BufferedReader br = new BufferedReader(new InputStreamReader(fis));
         while ((str = br.readLine()) != null) {
            if (str.equals("# Monitor Public/Private Key modulus (n)"))
					mod = br.readLine();
         }
         fis.close();
		} catch (Exception e) { return null; }

		return new PubRSA(new BigInteger(enc), new BigInteger(mod));
	}

   // Read the RSA key from file <user>RSAFile.key
   public static PubRSA readRSAKey(String input_Username) {
      try {
         String filename = input_Username.toUpperCase()+"RSAFile.key";
         FileInputStream fis = new FileInputStream(filename);
			BufferedReader br = new BufferedReader(new InputStreamReader(fis));
			String exp = br.readLine();
			String mod = br.readLine();
			PubRSA rsa = new PubRSA(new BigInteger(exp), new BigInteger(mod));
			fis.close();
         return rsa;
      } catch (Exception e) {
         iwar.status.setText(KeepTwo.rows("Pitbull [readRSAKey]: no file to read"+
														" key from", iwar.status.getText()));
      }
      return null;
   } // end of ReadRSAKey();

   // Read the RSA key from file <user>RSAFile.key
   public static RSA readUserRSAKey() {
      try {
         String filename = iwar.identity.getText().toUpperCase()+"RSAFile.key";
         FileInputStream fis = new FileInputStream(filename);
			BufferedReader br = new BufferedReader(new InputStreamReader(fis));
			String enc = br.readLine();
			String dec = br.readLine();			
			String mod = br.readLine();
			RSA rsa = new RSA(enc, dec, mod);
			fis.close();
         return rsa;
      } catch (Exception e) {
         iwar.status.setText(KeepTwo.rows("Pitbull [readUserRSAKey]: no key file",
														iwar.status.getText()));
      }
      return null;
   } // end of ReadUserRSAKey();

   // Read the ZKP key from file <user>ZKPFile.key
   public static ZKP readUserZKPKey() {
      try {
         String filename = iwar.identity.getText().toUpperCase()+"ZKPFile.key";
         FileInputStream fis = new FileInputStream(filename);
			BufferedReader br = new BufferedReader(new InputStreamReader(fis));
			String enc = br.readLine();
			String dec = br.readLine();			
			String mod = br.readLine();
			ZKP zkp = new ZKP(enc, dec, mod);
			fis.close();
         return zkp;
      } catch (Exception e) {
			iwar.status.setText(KeepTwo.rows("Pitbull [readUserZKPKey]: no key file",
														iwar.status.getText()));
      }
      return null;
   } // end of ReadUserRSAKey();

   // Read the RSA certificate from file <user>RSAFile.crt
   public static String readRSACert(String input_Username) {
      try {
         String filename = input_Username.toUpperCase()+"RSAFile.crt";
         FileInputStream fis = new FileInputStream(filename);
			BufferedReader br = new BufferedReader(new InputStreamReader(fis));
         String cert = br.readLine();
			fis.close();
         return cert;
      } catch (Exception e) {
         iwar.status.setText(KeepTwo.rows("Pitbull [readRSACert]: no certificate"+
														" file",	iwar.status.getText()));
      }
      return null;
   } // end of ReadRSACert();

   // Write the RSA key to file <user>RSAFile.key
   public static void writeRSAKey(String input_Username) {
      try {
         String filename = input_Username.toUpperCase()+"RSAFile.key";
         FileOutputStream fos = new FileOutputStream(filename);
			PrintWriter pw = new PrintWriter(fos, true);
			String exp = UserRSAKey.publicKey().getExponent().toString();
			String mod = UserRSAKey.publicKey().getModulus().toString();
			pw.println(exp);
			pw.println(mod);
         fos.close();
      } catch(Exception e) {
         iwar.status.setText(KeepTwo.rows("Pitbull [writeRSAKey]: cannot write"+
														" key file", iwar.status.getText()));
      }
   } // end of writeRSAKey
    
   // Write the RSA key to file <user>RSAFile.key
   public static void writeUserRSAKey(String name, String enc, String dec,
												  String mod) {
      try {
         String filename = name.toUpperCase()+"RSAFile.key";
         FileOutputStream fos = new FileOutputStream(filename);
			PrintWriter pw = new PrintWriter(fos, true);
			pw.println(enc);
			pw.println(dec);
			pw.println(mod);
         fos.close();
      } catch(Exception e) {
         iwar.status.setText(KeepTwo.rows("Pitbull [writeUserRSAKey]: cannot "+
														"write key file",iwar.status.getText()));
      }
   } // end of writeUserRSAKey
    
   // Write the ZKP key to file <user>ZKPFile.key
   public static void writeUserZKPKey(String name, String enc, String dec,
												  String mod) {
      try {
         String filename = name.toUpperCase()+"ZKPFile.key";
         FileOutputStream fos = new FileOutputStream(filename);
			PrintWriter pw = new PrintWriter(fos, true);
			pw.println(enc);
			pw.println(dec);
			pw.println(mod);
         fos.close();
      } catch(Exception e) {
         iwar.status.setText(KeepTwo.rows("Pitbull [writeUserZKPKey]: cannot "+
														"write key file",iwar.status.getText()));
      }
   } // end of writeUserZKPKey
    
   // Write the RSA certificate to file <user>RSAFile.crt
   public static void writeRSACert(String input_Username, String cert) {
      try {
			String filename = input_Username.toUpperCase()+"RSAFile.crt";
         FileOutputStream fos = new FileOutputStream(filename);
			PrintWriter pw = new PrintWriter(fos, true);
			pw.println(cert);
         fos.close();
      } catch(Exception e) {
         iwar.status.setText(KeepTwo.rows("Pitbull [writeRSACert]: cannot write "+
														"certificate file",
														iwar.status.getText()));
      }
   } // end of writeRSACert
    
   // Create and write RSA certificate from public key exp and mod to
	// <user>RSAFile.crt
   public static void createRSACert(String username, String exp, String mod) {
      try {
         MessageDigest md = MessageDigest.getInstance("SHA"); 
         byte[] expArr = exp.getBytes();
         byte[] modArr = mod.getBytes();
         md.update(expArr);
         md.update(modArr);
         BigInteger big = new BigInteger(1,md.digest());

         String filename = username.toUpperCase()+"RSAFile.crt";
         FileOutputStream fos = new FileOutputStream(filename);
			PrintWriter pw = new PrintWriter(fos, true);
         pw.println(big.toString(16));
         fos.close();
      } catch(Exception e) {
         iwar.status.setText(KeepTwo.rows("Pitbull [createRSACert]: cannot write "+
														"certificate file",
														iwar.status.getText()));
      }
   } // end of createRSACert
    
   // compare RSA certificate from Monitor with digest of public key exp and mod 
   public static boolean checkRSACert(String cert, String exp, String mod) {
      try {
         MessageDigest md = MessageDigest.getInstance("SHA"); 
         byte[] expArr = exp.getBytes();
         byte[] modArr = mod.getBytes();
         md.update(expArr);
         md.update(modArr);
         BigInteger big = new BigInteger(1,md.digest());
			BigInteger gtr = iwar.monitorPK.encrypt(new BigInteger(cert));
			if (big.equals(gtr)) return true;
      } catch(Exception e) {
         iwar.status.setText(KeepTwo.rows("Pitbull [checkRSACert]: message digest"+
														" failure",	iwar.status.getText()));
      }
      return false;
   } // end of checkRSACert
    
   // An RSA key pair is created or retrieved.  If one already exists it is in
   // <user>RSAFile.key - in that case it is fetched and the public key is placed
   // in TextFields arg[7] (exponent) and arg[8] (modulus).  Otherwise, a new key
	// is created and public key is placed in the TextFields above.  If t is true
	// then a new key will always be made and saved.
   public static int initializeUserRSAKey(String username, boolean t) {
      RSA UserPrivateKey;
      if (t || (UserPrivateKey = readUserRSAKey()) == null ||
			 iwar.command.arg[7].getText().equals("") ||   // can't read
			 iwar.command.arg[8].getText().equals("")) {   // or fields gone
         if (iwar != null) {
            iwar.status.setText(KeepTwo.rows("Pitbull [initRSAKey]: made RSA key",
															iwar.status.getText()));
         }
			// Could not read from file so make a new key
         UserRSAKey = new RSA(256);
         String enc = UserRSAKey.publicKey().getExponent().toString();
         String dec = UserRSAKey.privateKey().toString();
         String mod = UserRSAKey.publicKey().getModulus().toString();
         writeUserRSAKey(username, enc, dec, mod);
         iwar.command.arg[7].setText(enc);
         iwar.command.arg[8].setText(mod);
			iwar.command.privKey = dec;
         return 1;
      } else { // end of if(ReadRSAKey()...)
			iwar.status.setText(KeepTwo.rows("Pitbull [initRSAKey]: read RSA key"+
														" from file", iwar.status.getText()));
         UserRSAKey = UserPrivateKey;
         String exp = UserRSAKey.publicKey().getExponent().toString();
         String dec = UserRSAKey.privateKey().toString();			
         String mod = UserRSAKey.publicKey().getModulus().toString();
         iwar.command.arg[7].setText(exp);
         iwar.command.arg[8].setText(mod);
			iwar.command.privKey = dec;			
         return 0;
      }
   } // end of intializeUserRSAKey()

   // A ZKP key pair is created or retrieved.  If one already exists it is in
   // <user>ZKPFile.key - in that case it is fetched and the public key is placed
   // in TextFields arg[7] (exponent) and arg[8] (modulus).  Otherwise, a new key
	// is created and public key is placed in the TextFields above.  If t is true
	// then a new key will always be made and saved.
   public static int initializeUserZKPKey(String username, boolean t) {
      ZKP UserPrivateKey;
      if (t || (UserPrivateKey = readUserZKPKey()) == null ||
			 iwar.command.arg[7].getText().equals("") ||   // can't read
			 iwar.command.arg[8].getText().equals("")) {   // or fields gone
			iwar.status.setText(KeepTwo.rows("Pitbull [initZKPKey]: made ZKP key",
														iwar.status.getText()));
			// Could not read from file so make a new key
         UserZKPKey = new ZKP(256);
         String enc = UserZKPKey.publicKey().getExponent().toString();
         String dec = UserZKPKey.privateKey().toString();
         String mod = UserZKPKey.publicKey().getModulus().toString();
         writeUserZKPKey(username, enc, dec, mod);
			iwar.command.arg[7].setText(enc);
			iwar.command.arg[8].setText(mod);
			iwar.command.privKey = dec;
         return 1;
      } else { // end of if (ReadZKPKey()...)
         if (iwar != null) {
            iwar.status.setText(KeepTwo.rows("Pitbull [initZKPKey]: read ZKP key"+
															" from file", iwar.status.getText()));
         }
         UserZKPKey = UserPrivateKey;
         String exp = UserZKPKey.publicKey().getExponent().toString();
         String dec = UserZKPKey.privateKey().toString();			
         String mod = UserZKPKey.publicKey().getModulus().toString();
         iwar.command.arg[7].setText(exp);
         iwar.command.arg[8].setText(mod);
			iwar.command.privKey = dec;
         return 0;
      }
   } // end of intializeUserZKPKey()

   // Create a random (non-alphanumeric) password.
   public static String makeRandomPassword( int numChars ){
      char[] pw = new char[numChars];
      for(int i = 0; i < numChars; ++i)
         pw[i] = (char)(33 + ((int)(Math.random() * 85) % (125-33)));
      return new String(pw);
   } // end of makeRandomPassword

	// Record password in file and in GUI
   public static void setPassword (String pwd) {
      File passwordFile = new File(passwordFilePath);
      writeCookie(pwd,passwordFilePath);
      iwar.command.arg[2].setText(pwd);
      iwar.passwd.setText(pwd);
   } // end of setPassword

   // Retrieve the current password from file -- if the file doesn't
   // exist, generate a password, write it to file, then return it.
   public static String getPassword() {
      String pwd = null;
      File passwordFile = new File(passwordFilePath);
      // if the password for this user has already been stored, read it.
      // otherwise, write it to the file.
      try {
         if (passwordFile.createNewFile()) {
            if (iwar.passwd.getText().equals("")) 
               pwd = makeRandomPassword(Globals.PASSWORD_SIZE);
            else 
               pwd = iwar.passwd.getText();
            writeCookie(pwd, passwordFilePath);
            iwar.command.arg[2].setText(pwd);
            iwar.passwd.setText(pwd);
         } else {
            pwd = readCookie(passwordFilePath);
            iwar.passwd.setText(pwd);
            iwar.command.arg[2].setText(pwd);
         }
      } catch(IOException ioExc) {
         iwar.status.setText(KeepTwo.rows("Pitbull [getPassword]: IO exception",
														iwar.status.getText()));
			return null;
      }
      return pwd;
   } // end of getPassword

   // Generate a new password and store it in memory (don't write it
   // to file until the Monitor confirms that the password has been
   // received).
   public static String newPassword() {
      newPassword = makeRandomPassword(Globals.PASSWORD_SIZE);
      return newPassword;
   } // end of newPassword

   public static void passwordChangeConfirmed() {
      password = newPassword;
      writeCookie(password, passwordFilePath);
      iwar.command.arg[2].setText(password);
      iwar.passwd.setText(password);
   } // end of passwordChangeConfirmed

	// Return ALIVE cookie from file
   public static String getAliveCookie() {
      return readCookie(aliveCookieFilePath);
   } // end of getAliveCookie
   
   public static void setAliveCookie(String aliveCookie) {
      writeCookie(aliveCookie, aliveCookieFilePath);
   } // end of setAliveCookie

	// Read cookie from file and return
   public static String readCookie(String cookieFile) {
      String cookie = null;
      try {
         FileReader fr = new FileReader(cookieFile);
         BufferedReader cookieReader = new BufferedReader(fr);
         try {
            cookie = cookieReader.readLine();
         } catch (IOException readExc) {
            iwar.status.setText(KeepTwo.rows("Pitbull [readCookie]: IO exception",
															iwar.status.getText()));
         }
      } catch (FileNotFoundException fileExc) {
         iwar.status.setText(KeepTwo.rows("Pitbull [readCookie]: file not found",
														iwar.status.getText()));
      }
      return cookie.trim();
   } // end of readCookie

	// write cookie to file at specified path
   public static boolean writeCookie (String cookie, String filePath) {
      try {
         FileWriter fw = new FileWriter(filePath, false);
         PrintWriter cookieWriter = new PrintWriter(new BufferedWriter(fw));
         cookieWriter.println(cookie);
         cookieWriter.close();
         cookieWriter = null;
      } catch (IOException cookieExc) {
         iwar.status.setText(KeepTwo.rows("Pitbull [writeCookie]: IO exception",
														iwar.status.getText()));
      }
      return true;
   } // end of writeCookie
}
