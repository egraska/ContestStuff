import java.math.*;
import java.awt.*;

public class Trade {
	String name = null;  // Client identity of trader
	String exp  = null;  // Trader public key 
	String mod  = null;  // Trader public/private key modulus
	String dec  = null;  // Trader private key
	String cert = null;  // Trader certificate
	String auth_type = null;
	String trade = null;
	public String keyType = null;
	String rounds = null;
	RSA rsa = null;
	public BigInteger[] S;   // Receive the subset_j and subset_k  <- verifier
	public BigInteger[] R;   // Make the authorize set             <- verifier
	public BigInteger[] RR;  // Receive the authorize set          <- prover
	public boolean[] A;      // A[i] = true iff the ith # in authorize set is
	                         // identified in subset_a command
	
	public Trade () { }
	public void addKey (String t) {
		keyType = t;
	}
	
	public void addKey (String t, String e, String m) {
		keyType = t;
		exp = e;
		mod = m;
		if (keyType.equals("RSA"))
			rsa = new RSA(exp, mod);  // provides ability to use PK to unsign
	}
	public void addKey (String t, String e, String m, String d) {
		keyType = t;
		exp = e;
		mod = m;
		dec = d;
		if (keyType.equals("RSA"))	rsa = new RSA(exp, mod); // provides use of PK to unsign
	}
	public String getExp () { return exp; }
	public String getDec () { return dec; }	
	public void addCert (String c) { cert = c; }
	public void addName (String n) { name = n; }
	public String getName () { return name; }
	public String getTrade () { return trade; }
	public RSA getRSA () { return rsa; }
	public boolean getKeyType (String t) {	return keyType.equals(t); }
	public void setAuthType (String a) { auth_type = a; }
	public void addTrade (String t) { trade = t; }
	public void addRounds (String r) {
		rounds = r;
		RR = new BigInteger[Integer.parseInt(r)];
		R = new BigInteger[Integer.parseInt(r)];
		A = new boolean[Integer.parseInt(r)];
	}
	public String getRounds () { return rounds; }
	public String getMod() { return mod; }
	public boolean complete () {
		if (name != null && exp != null && mod != null && cert != null)
			return true;
		else
			return false;
	}
	
	public Color check () {
		if (keyType.equals("RSA")) {
			return Color.yellow;
		} else {
			int rnds = Integer.parseInt(rounds);
			BigInteger modulus = new BigInteger(mod);
			BigInteger pubkey = new BigInteger(exp);
			
			for (int i=0 ; i < rnds ; i++) {
				if (A[i] && !(S[i].modPow(new BigInteger("2"),modulus)).equals(RR[i].multiply(pubkey).mod(modulus))) return Color.red;
				else if (!A[i] && !(S[i].modPow(new BigInteger("2"),modulus)).equals(RR[i].mod(modulus))) return Color.red;
			}
			return Color.green;
		}
	}
}
