import java.util.*;

/**
 * Adds useful methods to the StringTokenizer class. Included are
 * methods to insert and delete Strings in a variety of ways, in each
 * case returning an altered version of the <code>String</code>
 * originally passed to the constructor. The basic methods are
 * insertString and removeTokens. The getRemaining method facilitates
 * the common task of retrieving the remainder of a
 * <code>String</code> once leading tokens have been removed. The
 * getSubString method allows a new <code>String</code> to be
 * assembled from a range of Tokens. This method is overloaded so that
 * two ranges of tokens may be combined to return a new subString. The
 * removeSubString method removes a range of tokens from the original
 * <code>String</code> and returns the new subString, and this method
 * is also overloaded for two ranges.
 *
 * @author Greg Larson
 * @version 1.0
 */
public class GLStringTokenizer extends StringTokenizer {
   /**
    * Takes one <code>String</code> argument and tokenizes it with
    * the default delimiters " \t\n\r\f".
    *
    * @param inputString The <code>String</code> to be tokenized into words.
    */
   public GLStringTokenizer(String inputString) {  super(inputString); }

   /**
    * Takes one <code>String</code> argument and tokenizes it with
    * the specified <code>String</code> of delimiters as the
    * separators between words.
    *
    * @param inputString The <code>String</code> to be tokenized into words.
    * @param delimiters The characters which separate words in 
    * inputString.
    */
   public GLStringTokenizer(String inputString, String delimiters) {
      super(inputString, delimiters);
   }

   /**
    * Inserts the <code>String</code> input after the specified
    * token, then returns the altered <code>String</code>. The
    * delimiter is assumed to be a space " ".
    *
    * @param insertionString The <code>String</code> to insert into
    * tokens. There is no need to include spaces before and after
    * this <code>String</code>, since the method does this
    * automatically.
    * @param insertionPoint The position at which insertionString 
    * will be placed. This is an int, and is zero-based counting 
    * (e.g., the first token is at position 0, etc.).
    *
    * @return a <code>String</code> value
    */
   public String insertString(String insertionString, int insertionPoint) {
      
      int numberOfTokens = this.countTokens();
      StringBuffer buffer = new StringBuffer();
      
      if (insertionPoint < 0) {
         System.err.println("GLStringTokenizer, method insertString: "
                            + "Negative insertion-point error.");
         return null;
      }

      // if the insertion-point is too large, simply append the String
      if (insertionPoint > numberOfTokens) insertionPoint = numberOfTokens;
      
      for (int i = 0; i < insertionPoint; ++i) {
         buffer.append(this.nextToken());
         buffer.append(" ");
      }
      buffer.append(insertionString);
      buffer.append(" ");
      if (this.hasMoreTokens()) buffer.append(this.getRemaining());

      String output = buffer.toString();
      // get rid of starting/ending spaces and return
      return output.trim();
   } // end insertString

   /**
    * Returns a <code>String</code> made up of all tokens remaining in a
    * StringTokenizer. The delimiter is assumed to be a space " ".
    * If there are no tokens remaining, returns <code>null</code>.
    * @return a <code>String</code> value
    */
   public String getRemaining() {
      int numberOfTokens = this.countTokens();
      StringBuffer buffer = new StringBuffer();
      if (numberOfTokens > 0) {
         for (int i = 0 ; i < numberOfTokens ; ++i) {
            buffer.append(this.nextToken());
            buffer.append(" ");
         }
         String output = buffer.toString();
         // get rid of starting/ending spaces and return
         return output.trim();
      } else 
         return null;
   } // end getRemaining
} // end class GLStringTokenizer
