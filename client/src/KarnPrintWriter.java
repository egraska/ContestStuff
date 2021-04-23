/******* Class KarnPrintWriter **********
         Copyright (c) 2004 Coleman Kane

         Written for: John V. Franco
         University of Cincinnati
         Computer Science
         20-ECES-694 Advanced Topics in Computer Science

Karn encryption algorithm
-------------------------
This is a symmetric key encryption system which uses the Diffie-Hellman
shared secret as a key to encrypt on one end (Client or Monitor) and decrypt
on the other end of a connection.  The main tool used by Karn is hashing,
in general, and message digest, in particular.  Both the key and the message
are partitioned into equal size blocks: 64 bytes for the key and 40 bytes
for each message block.  If a message does not end evenly on a 40 byte
boundary, padding is added to the last block so that it contains 40 bytes.
The hash algorithm used by the Monitor described here is SHA-1.  Operations
using SHA-1 are: reset, update, and digest.  The digest operation always
produces a 20 byte output.

To encrypt a plaintext block: divide the block into 2 halves, left and right,
of 20 bytes each; divide the key into two halves, left and right, of 32 bytes
each; reset the hash; update the hash with the left plaintext block; update
the hash with the left key block; xor the digest of the hash with the right
plaintext block to get the right ciphertext block; reset the hash; update the
hash with the right ciphertext block; update the hash with the right key block;
xor the hash digest with the left plaintext block to get the left ciphertext
block; output the left then right ciphertext blocks.

To decrypt two ciphertext halves totaling 40 bytes: reset the hash; update
the hash with the right ciphertext block; update the hash with the right key
block; xor the hash digest with the left ciphertext block to get the left
plaintext block; reset the hash; update the hash with the left plaintext block;
update the hash with the left key block; xor the digest of the hash with the
right ciphertext block to get the right plaintext block.  Assemble the two
plaintext halves for the 40 byte plaintext
block.

Padding: insert a 0 byte at the end of the message in the last plaintext block,
if necessary.  Choose a random number that fills up the bytes that remain in
the last block after the 0.
**/
import java.io.*;
import java.lang.*;
import java.math.*;
import java.security.*;

public class KarnPrintWriter extends PrintWriter {
   /******************************************************
    ** class KarnPrintWriter: Implements an encryptor to
    **   take plaintext from the user and write it to a
    **   PrintWriter stream as output
    ******************************************************/
   final int RADIX=32;
   final int PADSIZE=40;
   int dh_key_len_bytes;
   final int exchange_key_size = 64;

	private void setKarnKeySize() {
      try {
			dh_key_len_bytes =
				Integer.parseInt(MessageParser.getSetup("# Karn Key Size in bytes"));
      } catch(Exception e) {
         dh_key_len_bytes = 64;
      }
   } 

   /** Stores the shared key for the encrypted session **/
   private BigInteger sharedSecret;

   /** MessageDigest common to multiple functions in class,
       implements SHA-1 **/
   private MessageDigest md;

   /** SecureRandom to pad the shorter blocks with random data, and
       not just all zeros **/
   private SecureRandom r;

   /** Initialize a new PrintWriter with the specified OutputStream,
       autoFlush on/off and a designated shared key **/
   public KarnPrintWriter(Writer out, boolean autoFlush, BigInteger sKey)
      throws NoSuchAlgorithmException {
      super (out, autoFlush);
      sharedSecret = sKey;
      r = SecureRandom.getInstance("SHA1PRNG");
      r.setSeed(System.currentTimeMillis()); /* Seed the generator */
      md = MessageDigest.getInstance("SHA");
		setKarnKeySize();
   }

   /** General encryption function called by all print functions **/
   public String encrypt(String msg) {
      String val = "";
      md.reset();
      byte s[];
      byte m[] = new byte[PADSIZE];
      try {
         s = msg.getBytes("ISO-8859-1"); /* Grab as ASCII values */
      } catch(UnsupportedEncodingException uex) {
         return null;
      }
      byte mLeft[] = new byte[PADSIZE/2];
      byte mRight[] = new byte[PADSIZE/2];
      byte key[] = sharedSecret.toByteArray();
      int key_length = key.length;
      if (key_length > exchange_key_size) {
			for(int i=0; i<exchange_key_size; i++) {
				key[i] = key[i+1];
			}
      }
      if (key_length < dh_key_len_bytes) {
          int key_pad_len = dh_key_len_bytes - key_length;
          byte temp_key[] = new byte[dh_key_len_bytes];
          for (int i = 0; i<key_pad_len; i++) {
              temp_key[i] = 0;
          }
          for (int i = key_pad_len; i<dh_key_len_bytes; i++) {
              temp_key[i] = key[i-key_pad_len];
          }
          key = temp_key;
      }
      if (key.length > dh_key_len_bytes) {
			byte temp_key[] = new byte[dh_key_len_bytes];
			for (int i=0 ; i < dh_key_len_bytes; i++) {
				temp_key[i] = key[i];
			}
			key = temp_key;
		}

      byte kLeft[] = new byte[key.length/2];
      byte kRight[] = new byte[key.length/2];

      /* Create the array, padding to the next block size */
      int cSize;
      for (cSize = 1; cSize < msg.length() + 1; cSize += PADSIZE);
      byte c[] = new byte[cSize];
      int cOffset = 0;

      for (; cOffset < s.length; cOffset += PADSIZE) {
         byte padByte[] = new byte[1];
         padByte[0] = 0;
         for (int i = 0; i < PADSIZE; i++) {
            if ((i + cOffset) < s.length) m[i] = s[i + cOffset];
            else {
               m[i] = padByte[0];
               r.nextBytes(padByte);
            }
         }

         for (int i = 0; i < PADSIZE/2; i++) {
            mLeft[i] = m[i];
            mRight[i] = m[i + PADSIZE/2];
         }

         for (int i = 0; i < key.length/2; i++) {
            kLeft[i] = key[i];
            kRight[i] = key[i + key.length/2];
         }

         md.reset();
         md.update(mLeft);
         md.update(kLeft);
         byte tmp[] = md.digest();

         /* Write the right side */
         for (int i = 0; i < PADSIZE/2; i++)
            c[i + PADSIZE/2 + 1 + cOffset] = tmp[i] ^= mRight[i];

         md.reset();
         md.update(tmp);
         md.update(kRight);
         tmp = md.digest();

         /* Write the left side */
         for (int i = 0; i < PADSIZE/2; i++)
            c[i + 1 + cOffset] = (byte)(tmp[i] ^ mLeft[i]);
      }
      c[0] = 42; /* Put the guard byte at the beginning */

      BigInteger message = new BigInteger(c);

      return message.toString(RADIX);
   }

   /** Print a complete encrpyted line (record) **/
   public void println(String msg) {
      super.println(encrypt(msg));
   }
}
