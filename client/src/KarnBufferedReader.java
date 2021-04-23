/******* Class KarnBufferedReader **********
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

public class KarnBufferedReader extends BufferedReader {
   /*********************
    * The Karn Symmetric Encryption portion of the Monitor
    * client
    ********************/
   final int PADSIZE=40;
   final int RADIX=32;
   private BigInteger sharedSecret;
   private MessageDigest md;
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

   public KarnBufferedReader(Reader in, BigInteger sKey) {
      super(in);
      sharedSecret = sKey;
      try {
         md = MessageDigest.getInstance("SHA");
      } catch(Exception x) {
         System.out.println("KarnBufferedReader [Constructor]: Can't make "+
                            "SHA digests");
         md = null;
      }
		setKarnKeySize();
   }

   public String decrypt(String msg) {
      ByteArrayOutputStream out = new ByteArrayOutputStream();
      String result = "";
      if (msg == null) return "";
      BigInteger msgNum = new BigInteger(msg, RADIX);
      byte m[] = msgNum.toByteArray();
      md.reset();
      int mOffset = 0;
      String res_str = null;

		for ( ; m.length > 1 + mOffset ; ) {
			byte key[] = sharedSecret.toByteArray();
			int key_length = key.length;
			if (key_length > exchange_key_size) {
				for(int i=0 ; i < exchange_key_size; i++) {
					key[i] = key[i+1];
				}
			}
			if (key_length < dh_key_len_bytes) {
				int key_pad_len = dh_key_len_bytes - key_length;
				byte temp_key[] = new byte[dh_key_len_bytes];
				for (int i = 0; i < key_pad_len; i++) {
					temp_key[i] = 0;
				}
				for (int i = key_pad_len; i < dh_key_len_bytes; i++) {
					temp_key[i] = key[i-key_pad_len];
				}
				key = temp_key;
			}
			if (key.length > dh_key_len_bytes) {
				byte temp_key[] = new byte[dh_key_len_bytes];
				for (int i=0 ; i < dh_key_len_bytes ; i++) {
					temp_key[i] = key[i];
				}
				key = temp_key;
			}

			byte mLeft[] = new byte[PADSIZE/2];
			byte mRight[] = new byte[PADSIZE/2];
			byte pLeft[] = new byte[PADSIZE/2];
			byte pRight[] = new byte[PADSIZE/2];
			byte kLeft[] = new byte[key.length/2];
			byte kRight[] = new byte[key.length/2];
			byte tmp[] = new byte[PADSIZE/2];   // temp storage for the hash
			
			for (int i = 0; i < PADSIZE/2; i++) {
				mRight[i] = m[i + 21 + mOffset];
				mLeft[i] = m[i + 1 + mOffset];
			}
			
			for (int i = 0; i < key.length/2; i++) {
				kRight[i] = key[i + key.length/2];
				kLeft[i] = key[i];
			}
			
			md.reset();
			md.update(mRight);
			md.update(kRight);
			tmp = md.digest();
			for (int i = 0 ; i < tmp.length ; i++)
				pLeft[i] = (byte)(tmp[i] ^ mLeft[i]);
			
			md.reset();
			md.update(pLeft);
			md.update(kLeft);
			tmp = md.digest();
			for (int i = 0; i < tmp.length; i++)
				pRight[i] = (byte)(tmp[i] ^ mRight[i]);
			
			out.write(pLeft,0,PADSIZE/2);
			out.write(pRight,0,PADSIZE/2);
			mOffset += 40;
		}
		res_str = StripPadding(out.toByteArray());
      return res_str;
   }
	
   public String readLine() throws IOException {
      String line = decrypt(super.readLine());
      if (line.indexOf(0) > -1)
         line = line.substring(0, line.indexOf(0));
      return line;
   }
	
   private String StripPadding (byte input[]) {
      ByteArrayOutputStream buffer = new ByteArrayOutputStream();
      for (int i=0 ; i < input.length && input[i] != 0 ; i++)
         buffer.write(input[i]);
      return (new String(buffer.toByteArray()));
   }
}
