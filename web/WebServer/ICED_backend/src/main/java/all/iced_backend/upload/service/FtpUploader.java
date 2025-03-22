package all.iced_backend.upload.service;

import org.apache.commons.net.ftp.FTP;
import org.apache.commons.net.ftp.FTPClient;
import org.apache.commons.net.ftp.FTPReply;

import java.io.File;
import java.io.FileInputStream;
import java.io.IOException;
import java.io.InputStream;

public class FtpUploader {

    public String uploadFile(String server, int port, String user, String pass,
                             File localFile, String remoteDir) {

        FTPClient ftpClient = new FTPClient();

        try {
            ftpClient.connect(server, port);
            int replyCode = ftpClient.getReplyCode();
            if (!FTPReply.isPositiveCompletion(replyCode)) {
                System.out.println("FTP 서버 연결 실패. 응답 코드: " + replyCode);
                return null;
            }

            if (!ftpClient.login(user, pass)) {
                System.out.println("FTP 로그인 실패");
                return null;
            }

            ftpClient.enterLocalPassiveMode(); // 중요!
            ftpClient.setFileType(FTP.BINARY_FILE_TYPE);
            // 원격 디렉토리로 이동, 없으면 생성 (생성은 서버 설정에 따라 불가할 수 있음)
            if (!ftpClient.changeWorkingDirectory(remoteDir)) {
                // 디렉토리 생성 시도
                if (!ftpClient.makeDirectory(remoteDir)) {
                    System.out.println("원격 디렉토리 생성 실패: " + remoteDir);
                    return null;
                }
                ftpClient.changeWorkingDirectory(remoteDir);
            }

            String remoteFileName = localFile.getName();
            try (InputStream inputStream = new FileInputStream(localFile)) {
                boolean done = ftpClient.storeFile(remoteFileName, inputStream);
                if (done) {
                    System.out.println("FTP 업로드 성공: " + remoteFileName);
                    return remoteDir.endsWith("/")
                            ? remoteDir + remoteFileName
                            : remoteDir + "/" + remoteFileName;
                } else {
                    System.out.println("FTP 업로드 실패");
                    return null;
                }
            }

        } catch (IOException e) {
            e.printStackTrace();
            return null;
        } finally {
            try {
                if (ftpClient.isConnected()) {
                    ftpClient.logout();
                    ftpClient.disconnect();
                }
            } catch (IOException ex) {
                ex.printStackTrace();
            }
        }
    }
}
