/***************************************************************************
 * PubRSA - Store an RSA Public key, for distribution to other entities.  *
 **************************************************************************/
/*
  Written By: Coleman Kane <cokane@cokane.org>
  
  Written For: Dr. John Franco, University of Cincinnati ECECS Dept.
  20-ECES-653: Network Security
  
  Copyright(c): 2003, by Coleman Kane
  
  $Id: PubRSA.java,v 1.1 2008/04/02 12:06:03 franco Exp $
  
  $Log: PubRSA.java,v $
  Revision 1.1  2008/04/02 12:06:03  franco
  Initial revision

  Revision 1.1  2004/01/22 04:50:56  cokane
  Initial revision
  
  Revision 1.2  2003/11/25 23:17:37  cokane
  Add support to read the e,n values from the key.
  
  Revision 1.1  2003/11/23 07:46:41  cokane
  Initial revision
  
  
*/
import java.math.BigInteger;
import java.io.Serializable;

public class PubZKP implements Serializable {
   /** The components of the public key */
   BigInteger v, n;

   /** Takes the value (v) and the modulus (nVal) as the two
       parameters of the ZKP secret key. nVal was contructed from
       two primes (p,q) previously, and nVal = pq */
   public PubZKP(BigInteger vVal, BigInteger nVal) {
      v = vVal;
      n = nVal;
   }
   
   /** Return the public exponenet. **/
   public BigInteger getExponent() {
      return v;
   }
   
   /** Return Modulus. **/
   public BigInteger getModulus() {
      return n;
   }
}
