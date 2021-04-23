/*****************************************************************************
 * RSA - Storage and operation meduim for working with RSA Secret Keys.
 ****************************************************************************/
/*
  Written By: Coleman Kane <cokane@cokane.org>
  
  Written For: Dr. John Franco, University of Cincinnati ECECS Dept.
  20-ECES-653: Network Security
  
  Copyright(c): 2003, by Coleman Kane
  
  $Id: RSA.java,v 1.1 2008/04/02 12:06:03 franco Exp $
  
  $Log: RSA.java,v $
  Revision 1.1  2008/04/02 12:06:03  franco
  Initial revision

  Revision 1.1  2004/01/22 04:50:56  cokane
  Initial revision
  
  Revision 1.1  2003/11/23 07:46:27  cokane
  Initial revision
  
  
*/
import java.lang.*;
import java.math.*;
import java.util.*;
import java.io.*;
import java.security.*;

public class RSA implements Serializable {
   /** Constants I'll need later */
   private static BigInteger FOUR = new BigInteger("4");
   public  static BigInteger E = new BigInteger("65537");
	public  static BigInteger D;
	private static BigInteger N;
   private int keySize;
   
   private static SecureRandom r;
   private BigInteger q, p;
   String Description;
   Date created;
   
   public RSA() {
      if (r == null) r = new SecureRandom();
      keySize = 1024;
      gen();
   }
   
   public RSA(int keyLen) {
      if (r == null) r = new SecureRandom();
      keySize = keyLen;
      gen();
   }

	public RSA(String enc, String dec, String mod) {
		E = new BigInteger(enc);
		D = new BigInteger(dec);
		N = new BigInteger(mod);
	}

	public RSA(String enc, String mod) {
		E = new BigInteger(enc);
		N = new BigInteger(mod);
		D = new BigInteger("1");
	}

   /** Generate a new set of keys (used for pre-serialization). **/
   public void gen() {
      Description = "RSA secretKey";
      created = new Date();
      p = new BigInteger(keySize/2, 100, r);
      do {
         p = new BigInteger(keySize/2, 100, r);
      } while (p.mod(RSA.FOUR).intValue() != 3);
      
      do {
         q = new BigInteger(keySize/2, 100, r);
      } while (q.mod(RSA.FOUR).intValue() != 3);

		N = p.multiply(q);
		BigInteger x = q.subtract(BigInteger.ONE);
		BigInteger y = p.subtract(BigInteger.ONE);
		D = E.modInverse(x.multiply(y));
   }
   
   /** Returns a signature of m (m^d mod n) */
   public BigInteger encryptNum(BigInteger m) {
      return m.modPow(D, N);
   }

   /** Provide this to make code look more readable **/
   public BigInteger decryptNum(BigInteger m) {
      return m.modPow(E, N);
   }

   /** Returns a Public key to match this object */
   public PubRSA publicKey() {
      return new PubRSA(E, N);
   }

	public BigInteger privateKey() {
		return D;
	}
}
