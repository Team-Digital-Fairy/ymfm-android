package team.digitalfairy.xcel.ymfm_android_3;

import java.io.File;
import java.io.FileInputStream;
import java.io.FileNotFoundException;
import java.io.IOException;
import java.nio.file.Files;

public class VGMParser implements Runnable {
    private long wait;
    private byte[] vgm;
    private long current_offset;

    private int B32(int a, int b, int c, int d) {
        //return
        return 0;
    }

    public static boolean checkVGM(File f) {
        byte buffer[] = new byte[32];
        try(FileInputStream fis = new FileInputStream(f)) {
            // if it's start with "Vgm", return true
            // if not, check if it's gzip
            fis.read(buffer,0,32);
            // TODO: Add support for VGZ
            /*
            if(buffer[0] == 0x1f && buffer[1] == 0x8b) {

            }
            */

            if(buffer[0] == 'V' && buffer[1] == 'g' && buffer[2] == 'm' && buffer[3] == ' ') {
                return true;
            }

        } catch (IOException e) {
            e.printStackTrace();
            return false;
        }
        return false;
    }


    public VGMParser(File f) {
        try {
            vgm = Files.readAllBytes(f.toPath());
        } catch (IOException e) {
            e.printStackTrace();
        }

        // Read some clock informations; VGM Data offset and such


    }

    @Override
    public void run() {
        if(wait > 0) {
            wait--;
            return;
        }




    }
}
