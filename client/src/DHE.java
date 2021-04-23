// DHE.java                                              -*- Java -*-
//    The engine for Diffie-Hellman
//
// Copyright(C) 1998 Robert Sexton
// You can do anything you want with this, except pretend you
// wrote it.
//
// Written   :   Robert Sexton         University of Cincinnati
//   By          
//
// Written   :   John Franco
//   For         Special Topics: Java Programming
//               15-625-595-001, Fall 1998
// RCS       :
//
// $Revision: 1.1 $
// $Date: 2009/04/22 11:37:35 $
//
// $Log: DHE.java,v $
// Revision 1.1  2009/04/22 11:37:35  franco
// *** empty log message ***
//
// Revision 1.1.1.1  2008/04/16 13:16:49  franco
//
//
// Revision 0.2  1998/11/30 18:59:03  bkuhn
//   -- latest changes from Robert
//
// Revision 1.2  1998/11/30 18:49:25  robert
// Added Wrapper for the KEy Object
//
// Revision 1.1  1998/11/30 13:51:55  robert
// Initial revision
//
// Revision 0.1  1998/11/30 04:57:28  bkuhn
//   # initial version
//

import java.io.*;
import java.util.Date;
import java.math.BigInteger;
import java.security.*;
 
/*
 * class DHE 
 * Heres where you will find all of the Diffie-Hellman bits.
 * You can generate DH Public Keys if you like, but you may get bored.
 * Did I mention that it takes a very long time?
 * Setting up the SecureRandom generator is time consuming, but you never
 * need more than one of them.
 * DH Key exchange produces about 512 bits worth of secret key.
 * You may opt to use only some of that.
 *
 * You may wish to remove the print statements from the Constructor
 *
 * How to use:
 *  a.  Instantiate one. - Feed it the offical p and g from file Parms 
 *  b.  Send the other side the string you get back from getExchangeKey();
 *  c.  Get the exchange key from the other guy,
 *      feed it to setExchangeKey();
 *
 *  OPTIONAL: do b & c in reverse order, as needed.
 *
 *  d.  Retrieve the shared secret key with getSharedKey();  
 */

class DHE {
   static SecureRandom sr = null; 

   final int keysize = 512;           /* Default size, in bits */
   final int ARBITRARY_CONSTANT = 80;
   final int RADIX = 10;              /* All keys are output base 32 */

   private BigInteger x;           /* This is our personal secret Key */
   private BigInteger x_pub;       /* This is our exchange Key */
   private BigInteger p, g;
   private BigInteger s_secret;

   /* Use this one */
   DHE (BigInteger p, BigInteger g) {
      if (sr == null) sr = new SecureRandom();

      this.p = p;
      this.g = g;
      x = new BigInteger(keysize,sr); /* Generate our secret Key */
      x_pub = g.modPow(x,p);
      s_secret = BigInteger.valueOf(0); 
   }
   
   /* This is where you get your key to send the other side */
   public String getExchangeKey () {  return(x_pub.toString(RADIX));  }

   /* Feed their key into this routine. */
   public boolean setExchangeKey (String their_key) {
      try {
         BigInteger them = new BigInteger (their_key,RADIX);
         s_secret = them.modPow(x,p);
         return(true);
      } catch (NumberFormatException e) {
         System.err.println("Malformed DH Key");
         return(false);
      }
   }

   /* When you are done, retrieve the shared secret key here */
   BigInteger getSharedKey() {  return s_secret;  }

   String getSharedSecret() { return s_secret.toString(RADIX); }

   public String toString() {
      StringBuffer scratch = new StringBuffer();
      scratch.append("Secret Key(x): " + x.toString(RADIX) + "\n" );
      scratch.append("Public Key(X): " + x_pub.toString(RADIX) + "\n" );
      scratch.append("Shared Key   : " + s_secret.toString(RADIX) );
      return scratch.toString();
   }
}
