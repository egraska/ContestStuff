import java.net.URL;
import javax.sound.sampled.*;

public class PlayClip extends Thread {
	Clip audClip;
	String clip;

	public PlayClip (String c) {  clip = c; }

	public void run() {
		try {
			URL snd = getClass().getClassLoader().getResource("snd/"+clip);
			Line.Info linfo = new Line.Info(Clip.class);
			Line line = AudioSystem.getLine(linfo);
			audClip = (Clip)line;
			AudioInputStream ais = AudioSystem.getAudioInputStream(snd);
			audClip.open(ais);
			audClip.setFramePosition(0);
			audClip.start();
		} catch (Exception e) {
			System.out.println("PlayClip:"+e.toString());
		}
	}
}
