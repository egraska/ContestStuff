/*****************************************************************************
 * ZKP - Storage and operation meduim for working with ZKP Secret Keys.
 ****************************************************************************/
/*
  Taken from RSA.java By: Coleman Kane <cokane@cokane.org>
  
  Edited by: Dr. John Franco, University of Cincinnati ECECS Dept.
  
  Copyright(c): 2003, by Coleman Kane, 2019 by John Franco
  
*/
import java.lang.*;
import java.math.*;
import java.util.*;
import java.io.*;
import java.security.*;

public class ZKP implements Serializable {
   private static BigInteger FOUR = new BigInteger("4");
   private static BigInteger TWO = new BigInteger("2");	
   public  static BigInteger E;
	public  static BigInteger D;
	private static BigInteger N;
   private int keySize;
   
   private static SecureRandom r;
   private BigInteger q, p;
   String Description;
   Date created;
   
   public ZKP() {
      if (r == null) r = new SecureRandom();
      keySize = 512;
      gen();
   }
   
   public ZKP(int keyLen) {
      if (r == null) r = new SecureRandom();
      keySize = keyLen;
      gen();
   }

	public ZKP(String enc, String dec, String mod) {
		E = new BigInteger(enc);
		D = new BigInteger(dec);
		N = new BigInteger(mod);
	}

   /** Generate a new set of keys (used for pre-serialization). **/
   public void gen() {
      Description = "ZKP privateKey";
      created = new Date();
      do {
         p = new BigInteger(keySize/2, 100, r);
      } while (p.mod(ZKP.FOUR).intValue() != 3);
      
      do {
         q = new BigInteger(keySize/2, 100, r);
      } while (q.mod(ZKP.FOUR).intValue() != 3);

		N = p.multiply(q);

		do {
			D = new BigInteger(keySize/2, 100, r);
		} while (D.mod(ZKP.FOUR).intValue() != 3);
		
		E = D.modPow(ZKP.TWO,N);
   }
   
   /** Returns a Public key to match this object */
   public PubZKP publicKey() {
      return new PubZKP(E, N);
   }

	public BigInteger privateKey() {
		return D;
	}
}
